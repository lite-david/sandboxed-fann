#include "fann_pylib.h"
#include "fann.h"
float fann_train_test_pylib(float* train_data, float* train_labels, int train_nrow, int ncol, int nout, 
                      float* test_data, float* test_labels, int test_nrow){
    const unsigned int num_input = 2;
    const unsigned int num_output = nout;
    const unsigned int num_layers = 3;
    const unsigned int num_neurons_hidden = 3;
    const float desired_error = (const float) 0.001;
    const unsigned int max_epochs = 500000;
    const unsigned int epochs_between_reports = 1000;
    float result;
    struct fann *ann = fann_create_standard(num_layers, num_input,
        num_neurons_hidden, num_output);
    fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
    fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);
    struct fann_train_data *train_arr = fann_create_train_array( train_nrow, ncol, train_data, nout, train_labels);
    struct fann_train_data *test_arr = fann_create_train_array( test_nrow, ncol, test_data, nout, test_labels);
    fann_train_on_data(ann, train_arr, max_epochs, epochs_between_reports, desired_error);
    printf("%f \n", fann_get_MSE(ann));
    result = fann_test_data(ann, test_arr);
    fann_destroy(ann);
    return result;
}