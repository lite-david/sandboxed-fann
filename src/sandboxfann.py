from ctypes import *
import pathlib
import time
import matplotlib.pyplot as plt

if __name__ == "__main__":
    libname = pathlib.Path().absolute() / "libsandboxfann.so"
    c_lib = CDLL(libname)
    c_lib.sandboxed_train()