#pragma once

#ifdef __cplusplus
extern "C" {
#endif
    struct fann_train_data* sandboxed_create_train_array(float* data, 
                                float* labels, int nrow, int ncol, int nout);
    void sandboxed_train_on_data(struct fann* ann, struct fann_train_data *arr, 
                                unsigned int max_epochs, 
                                unsigned int epochs_between_reports, 
                                float desired_error);
    float sandboxed_test_data(struct fann* ann, struct fann_train_data* arr);
    void sandboxed_train(float* data, float* labels, int nrow, int ncol, int nout);
    float sandboxed_test(float* data, float* labels, int nrow, int ncol, int nout);
    void ref_train_test();
#ifdef __cplusplus
}
#endif