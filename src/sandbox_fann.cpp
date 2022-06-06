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
RLBOX_DEFINE_BASE_TYPES_FOR(toylib, noop);

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

    // dont need to sandbox this
    struct fann *ann = fann_create_standard(num_layers, num_input,
        num_neurons_hidden, num_output);

    //don't need to sandbox this
    fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
    fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);

    //need to pass input data into struct fann_train_data
    //into function
    //fann_type is a float, was imported from fann.h
    //uint num_data, uint num_input, fann_type* input, 
    //uint num_output, fann_type* output

    //sandbox fann_create_train_array, fann_train_on_data()
    //assume that labels are 1 dimensional output
    //very reasonable assumption
    struct fann_train_data *arr = 
        fann_create_train_array(nrow, ncol, data, nout, labels);
    
    rlbox_sandbox_toylib sandbox;
    sandbox.create_sandbox();

    

    // printf("%d \n", fann_length_train_data(arr));

    //call sandboxed train beforehand
    //create sandbox, taint input data, 
    //use rlbox to call fann_train_on_data
    // fann_train_on_data(ann, arr, uint max_epochs, uint e_bet_reports, 
    //    float desired_error)
    fann_train_on_data(ann, arr, max_epochs,
        epochs_between_reports, desired_error);
    printf("%f \n", fann_get_MSE(ann));

    //retrieve tainted values and save it to ann

    fann_save(ann, "xor_float.net");

    fann_destroy(ann);

}


//create sandbox_test here and in
float sandboxed_test(float* data, float* labels, int nrow, int ncol, int nout) {
    //sandbox fann_test_data, fann_run?

    fann_type *calc_out;
    fann_type input[2];

    struct fann *ann = fann_create_from_file("xor_float.net");

    input[0] = -1;
    input[1] = 1;
    calc_out = fann_run(ann, input);

    printf("xor test (%f,%f) -> %f\n", input[0], input[1], calc_out[0]);
    struct fann_train_data *arr = 
        fann_create_train_array(nrow, ncol, data, nout, labels);

    float mse = fann_test_data(ann, arr);
    printf("%f \n", mse);
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
