#pragma once

#ifdef __cplusplus
extern "C" {
#endif
    float sandboxed_train_and_test(float* train_data, float* train_labels, int train_nrow, int ncol, int nout,
                              float* test_data, float* test_labels, int test_nrow);
#ifdef __cplusplus
}
#endif