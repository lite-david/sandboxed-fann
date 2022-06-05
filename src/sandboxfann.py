from ctypes import *
import pathlib
import time
import matplotlib.pyplot as plt
import numpy as np
from numpy.ctypeslib import ndpointer

if __name__ == "__main__":
    libname = pathlib.Path().absolute() / "libsandboxfann.so"
    c_lib = CDLL(libname)

    #define the list type here first
    x = np.array([[1,2],[3,4]])
    floatpp = ndpointer(dtype=np.float64, ndim=2, flags='C_CONTIGUOUS')

    c_lib.sandboxed_train.argtypes = [floatpp]
    
    c_lib.sandboxed_train(x)