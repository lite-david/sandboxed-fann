#pragma once

#ifdef __cplusplus
extern "C" {
#endif
    int sandboxed_train(float **data);
    int sandboxed_test();
#ifdef __cplusplus
}
#endif