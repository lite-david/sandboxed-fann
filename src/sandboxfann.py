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
    x = np.array([[1.0, 2.0], [2.2, 3.3]], dtype=np.float32)
    print(x)
    floatpp = ndpointer(dtype=np.float32, ndim=2)

    c_lib.sandboxed_train.argtypes = [floatpp]
    
    c_lib.sandboxed_train(x)
