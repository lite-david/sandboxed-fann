#pragma once

#ifdef __cplusplus
extern "C" {
#endif
    void sandboxed_train(float* data, float* labels, int nrow, int ncol, int nout);
    float sandboxed_test(float* data, float* labels, int nrow, int ncol, int nout);
    void ref_train_test();
#ifdef __cplusplus
}
#endif