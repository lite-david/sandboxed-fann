# fetch source files for rlbox wasm2c sandboxing
git clone https://github.com/PLSysSec/rlbox_wasm2c_sandbox

# fetch wasi-sdk-12.0 which is compatible with the modified wasm2c compiler
# This has the clang compiler to compile C -> WASM
wget https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-12/wasi-sdk-12.0-linux.tar.gz
tar -xvzf wasi-sdk-12.0-linux.tar.gz

# setup wasm2c modified compiler 
# this will be used for compiling WASM -> C
git clone https://github.com/PLSysSec/wasm2c_sandbox_compiler/
cd wasm2c_sandbox_compiler
mkdir build
cd build
cmake ../ -DBUILD_TESTS=OFF
make wasm2c

