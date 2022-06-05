from ctypes import *
import pathlib
import time
import matplotlib.pyplot as plt

if __name__ == "__main__":
    libname = pathlib.Path().absolute() / "libsandboxfann.so"
    c_lib = CDLL(libname)

    #define the list type here first


    c_lib.sandboxed_train.argtypes = []
    c_lib.sandboxed_train()