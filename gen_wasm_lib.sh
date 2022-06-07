wasi-sdk-12.0/bin/clang --sysroot wasi-sdk-12.0/share/wasi-sysroot/ rlbox_wasm2c_sandbox/c_src/wasm2c_sandbox_wrapper.c -c -o wasm2c_sandbox_wrapper.o
wasi-sdk-12.0/bin/clang --sysroot wasi-sdk-12.0/share/wasi-sysroot/ -Isrc/include ./src/fann.c -c -o fann.o 
wasi-sdk-12.0/bin/clang --sysroot wasi-sdk-12.0/share/wasi-sysroot/ -Isrc/include ./src/fann_io.c -c -o fann_io.o 
wasi-sdk-12.0/bin/clang --sysroot wasi-sdk-12.0/share/wasi-sysroot/ -Isrc/include ./src/fann_train.c -c -o fann_train.o 
wasi-sdk-12.0/bin/clang --sysroot wasi-sdk-12.0/share/wasi-sysroot/ -Isrc/include ./src/fann_train_data.c -c -o fann_train_data.o 
wasi-sdk-12.0/bin/clang --sysroot wasi-sdk-12.0/share/wasi-sysroot/ -Isrc/include ./src/fann_error.c -c -o fann_error.o 
wasi-sdk-12.0/bin/clang --sysroot wasi-sdk-12.0/share/wasi-sysroot/ -Isrc/include ./src/fann_cascade.c -c -o fann_cascade.o 
wasi-sdk-12.0/bin/clang --sysroot wasi-sdk-12.0/share/wasi-sysroot/ -Isrc/include ./src/fann_pylib.c -c -o fann_pylib.o 
wasi-sdk-12.0/bin/clang --sysroot wasi-sdk-12.0/share/wasi-sysroot/ wasm2c_sandbox_wrapper.o fann_pylib.o fann.o fann_io.o fann_train.o fann_train_data.o fann_error.o fann_cascade.o -Wl,--export-all -Wl,--no-entry -Wl,--growable-table -o fann.wasm -lstdc++ -lm -v
wasm2c_sandbox_compiler/build/wasm2c -o fannwasm.c fann.wasm
mv fannwasm.c src/fannwasm.c
mv fannwasm.h src/fannwasm.h
#gcc -I wasm2c_sandbox_compiler/wasm2c/ -shared -fPIC -O3 -o libWasmFoo.so toylibwasm.c wasm2c_sandbox_compiler/wasm2c/wasm-rt-impl.c wasm2c_sandbox_compiler/wasm2c/wasm-rt-os-unix.c wasm2c_sandbox_compiler/wasm2c/wasm-rt-wasi.c

