from ctypes import *
import pathlib
import time
import matplotlib.pyplot as plt
import numpy as np
from numpy.ctypeslib import ndpointer, load_library, as_ctypes_type, as_ctypes

if __name__ == "__main__":
    libname = pathlib.Path().absolute() / "libsandboxfann.so"
    c_lib = load_library("libsandboxfann", pathlib.Path().absolute())

    #define the list type here first
    #x = np.array([[1,2], [2,3]], dtype=np.float32)
    #y = np.array([0.0,1.0], dtype=np.float32)
    x = np.array([[-1,-1],[-1,1],[1,-1],[1,1]], dtype = np.float32)
    y = np.array([[-1],[1],[1],[-1]], dtype = np.float32)
    floatpp = ndpointer(dtype=np.float32, ndim=2)

    c_lib.sandboxed_train_and_test.argtypes = [floatpp, floatpp, c_int, c_int, c_int, floatpp, floatpp, c_int]
    nrows = len(x)
    if nrows > 0:
        ncols = len(x[0])
    else:
        ncols = 0
    if len(y) == 0:
        nout = 0
    else:
        nout = len(y[0])
    print("Training:")
    c_lib.sandboxed_train_and_test.restype = c_float
    mse = c_lib.sandboxed_train_and_test(x, y, nrows, ncols, nout, x, y, nrows)
    print("Mse:", mse)
