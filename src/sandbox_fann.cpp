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

int sandboxed_train() {
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

    fann_save(ann, "xor_float.net");

    fann_destroy(ann);
    return 0;
}
