#include "fann_pylib.h"
#include "fannwasm.h"
#include "sandbox_fann.h"

#define RLBOX_SINGLE_THREADED_INVOCATIONS
// All calls into the sandbox are resolved statically.
#define RLBOX_USE_STATIC_CALLS() rlbox_wasm2c_sandbox_lookup_symbol

#include <stdio.h>
#include <cassert>
#include <rlbox/rlbox.hpp>
#include "rlbox_wasm2c_sandbox.hpp"

using namespace std;
using namespace rlbox;

RLBOX_DEFINE_BASE_TYPES_FOR(fannlib, wasm2c);

float sandboxed_train_and_test(float* train_data, float* train_labels, int train_nrow, int ncol, int nout,
                              float* test_data, float* test_labels, int test_nrow) {
    rlbox_sandbox<rlbox_wasm2c_sandbox> sandbox;
    sandbox.create_sandbox("wasmsandbox");
    auto dataSize = sizeof(float)*train_nrow*ncol;
    auto labelSize = sizeof(float)*train_nrow*nout;

    auto testDataSize = sizeof(float)*test_nrow*ncol;
    auto testLabelSize = sizeof(float)*test_nrow*nout;
    tainted_fannlib<float *> taintedData = sandbox.malloc_in_sandbox<float>(dataSize);
    std::memcpy(taintedData.unverified_safe_pointer_because(dataSize, "writing to region"),
        train_data, dataSize);
    tainted_fannlib<float *> taintedLabels = sandbox.malloc_in_sandbox<float>(labelSize);
    std::memcpy(taintedLabels.unverified_safe_pointer_because(labelSize, "writing to region"),
        train_labels, labelSize);
    tainted_fannlib<float *> taintedTestData = sandbox.malloc_in_sandbox<float>(testDataSize);
    std::memcpy(taintedTestData.unverified_safe_pointer_because(testDataSize, "writing to region"),
        test_data, testDataSize);
    tainted_fannlib<float *> taintedTestLabels = sandbox.malloc_in_sandbox<float>(testLabelSize);
    std::memcpy(taintedTestLabels.unverified_safe_pointer_because(testLabelSize, "writing to region"),
        test_labels, testLabelSize);
    float res = sandbox.invoke_sandbox_function(fann_train_test_pylib, taintedData, taintedLabels, train_nrow, ncol, nout, taintedTestData, taintedTestLabels, test_nrow).copy_and_verify([](float mse) {
                    return mse;
                });
    sandbox.destroy_sandbox();
    return res;
}
