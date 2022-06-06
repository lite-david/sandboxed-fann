#include "fann.h"
#include "sandbox_fann.h"

#define RLBOX_SINGLE_THREADED_INVOCATIONS
// All calls into the sandbox are resolved statically.
#define RLBOX_USE_STATIC_CALLS() rlbox_noop_sandbox_lookup_symbol

#include <stdio.h>
#include <cassert>
#include <rlbox/rlbox.hpp>
#include <rlbox/rlbox_noop_sandbox.hpp>

using namespace std;
using namespace rlbox;

// Define base type for mylib using the noop sandbox
RLBOX_DEFINE_BASE_TYPES_FOR(fannlib, noop);

struct fann_train_data* sandboxed_create_train_array(float* data, float* labels, int nrow, int ncol, int nout) {
    rlbox_sandbox_fannlib sandbox;
    sandbox.create_sandbox();
    auto dataSize = sizeof(float)*nrow*ncol;
    auto labelSize = sizeof(float)*nrow*nout;
    tainted_fannlib<float *> taintedData = sandbox.malloc_in_sandbox<float>(dataSize);
    std::memcpy(taintedData.unverified_safe_pointer_because(dataSize, "writing to region"),
        data, dataSize);
    tainted_fannlib<float *> taintedLabels = sandbox.malloc_in_sandbox<float>(labelSize);
    std::memcpy(taintedLabels.unverified_safe_pointer_because(labelSize, "writing to region"),
        labels, labelSize);

    unsigned long p = sandbox.invoke_sandbox_function(
        fann_create_train_array, nrow, ncol, taintedData,
        nout, taintedLabels).copy_and_verify_address([] (unsigned long p) {
            return p;
        });
    struct fann_train_data *arr = reinterpret_cast<struct fann_train_data *>(p);
    sandbox.destroy_sandbox();
    return arr;
}

void sandboxed_train_on_data(struct fann* ann, struct fann_train_data *arr, unsigned int max_epochs, 
                            unsigned int epochs_between_reports, float desired_error) {
    rlbox_sandbox_fannlib sandbox;
    sandbox.create_sandbox();
    auto annSize = sizeof(struct fann);
    auto arrSize = sizeof(struct fann_train_data);
    tainted_fannlib<struct fann*> taintedAnn = sandbox.malloc_in_sandbox<struct fann>(annSize);
    std::memcpy(taintedAnn.unverified_safe_pointer_because(annSize, "writing to region"),
        ann, annSize);
    tainted_fannlib<struct fann_train_data*> taintedArr = sandbox.malloc_in_sandbox<struct fann_train_data>(arrSize);
    std::memcpy(taintedArr.unverified_safe_pointer_because(arrSize, "writing to region"),
        arr, arrSize);

    sandbox.invoke_sandbox_function(fann_train_on_data, taintedAnn, taintedArr,
        max_epochs, epochs_between_reports, desired_error);
    
    //need to write taintedAnn values back into ann
    struct fann* tempAnn = reinterpret_cast<struct fann*>(taintedAnn.unverified_safe_pointer_because(annSize, "writing to region"));
    std::memcpy(ann, tempAnn, annSize);
    sandbox.destroy_sandbox();
}

float sandboxed_test_data(struct fann* ann, struct fann_train_data* arr) {
    rlbox_sandbox_fannlib sandbox;
    sandbox.create_sandbox();
    auto annSize = sizeof(struct fann);
    auto arrSize = sizeof(struct fann_train_data);
    tainted_fannlib<struct fann*> taintedAnn = sandbox.malloc_in_sandbox<struct fann>(annSize);
    std::memcpy(taintedAnn.unverified_safe_pointer_because(annSize, "writing to region"),
        ann, annSize);
    tainted_fannlib<struct fann_train_data*> taintedArr = sandbox.malloc_in_sandbox<struct fann_train_data>(arrSize);
    std::memcpy(taintedArr.unverified_safe_pointer_because(arrSize, "writing to region"),
        arr, arrSize);

    float mse = sandbox.invoke_sandbox_function(fann_test_data, taintedAnn, 
                taintedArr).copy_and_verify([](float mse) {
                    return mse;
                });

    //need to write taintedAnn values back into ann
    struct fann* tempAnn = reinterpret_cast<struct fann*>(taintedAnn.unverified_safe_pointer_because(annSize, "writing to region"));
    std::memcpy(ann, tempAnn, annSize);
    sandbox.destroy_sandbox();
    return mse;
}

void sandboxed_train(float* data, float* labels, int nrow, int ncol, int nout) {
    // for (int i = 0; i < nrow; i++) {
    //     for (int j = 0; j < ncol; j++) {
    //         printf("%f \n", data[i*nrow + j]);
    //     }
    //     printf("%f \n", labels[i]);
    // }
    const unsigned int num_input = 2;
    const unsigned int num_output = nout;
    const unsigned int num_layers = 3;
    const unsigned int num_neurons_hidden = 3;
    const float desired_error = (const float) 0.001;
    const unsigned int max_epochs = 500000;
    const unsigned int epochs_between_reports = 1000;
    struct fann *ann = fann_create_standard(num_layers, num_input,
        num_neurons_hidden, num_output);
    fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
    fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);

    struct fann_train_data *arr = sandboxed_create_train_array(data, labels, nrow, ncol, nout);

    //printf("%d \n", fann_length_train_data(arr));

    sandboxed_train_on_data(ann, arr, max_epochs, epochs_between_reports, desired_error);

    printf("%f \n", fann_get_MSE(ann));

    fann_save(ann, "xor_float.net");

    fann_destroy(ann);
}


//create sandbox_test here and in
float sandboxed_test(float* data, float* labels, int nrow, int ncol, int nout) {
    //sandbox fann_test_data

    struct fann *ann = fann_create_from_file("xor_float.net");

    // fann_type *calc_out;
    // fann_type input[2];
    // input[0] = -1;
    // input[1] = 1;
    // calc_out = fann_run(ann, input);
    // printf("xor test (%f,%f) -> %f\n", input[0], input[1], calc_out[0]);
    
    struct fann_train_data *arr = 
        sandboxed_create_train_array(data, labels, nrow, ncol, nout);

    float mse = sandboxed_test_data(ann, arr);
    return mse;
}


void ref_train_test() {
    //train
    const unsigned int num_input = 2;
    const unsigned int num_output = 1;
    const unsigned int num_layers = 3;
    const unsigned int num_neurons_hidden = 3;
    const float desired_error = (const float) 0.001;
    const unsigned int max_epochs = 500000;
    const unsigned int epochs_between_reports = 1000;

    struct fann *ann = fann_create_standard(num_layers, num_input,
        num_neurons_hidden, num_output);

    fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
    fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);

    fann_train_on_file(ann, "xor.data", max_epochs,
        epochs_between_reports, desired_error);

    fann_save(ann, "xor_float.net");
    printf("%f \n", fann_get_MSE(ann));

    fann_destroy(ann);

    // test
    fann_type *calc_out;
    fann_type input[2];

    struct fann *annn = fann_create_from_file("xor_float.net");

    input[0] = -1;
    input[1] = 1;
    calc_out = fann_run(annn, input);

    printf("xor test (%f,%f) -> %f\n", input[0], input[1], calc_out[0]);
    // printf("%f \n", fann_get_MSE(annn));

    fann_destroy(annn);

}
