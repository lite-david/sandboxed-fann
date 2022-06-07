FIND_PACKAGE(OpenMP)
IF(OPENMP_FOUND)
  OPTION(DISABLE_PARALLEL_FANN "Disable parallel fann functions" OFF)
ENDIF(OPENMP_FOUND)

IF(NOT OPENMP_FOUND OR DISABLE_PARALLEL_FANN)
  ADD_DEFINITIONS(-DDISABLE_PARALLEL_FANN)
ELSE()
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
ENDIF(NOT OPENMP_FOUND OR DISABLE_PARALLEL_FANN)

ADD_SUBDIRECTORY( include )

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)
ADD_DEFINITIONS(-D_REENTRANT)
if (WIN32)
  ADD_DEFINITIONS(-DFANN_DLL_EXPORTS)
endif (WIN32)
########### next target ###############

SET(floatfann_LIB_SRCS
        floatfann.c
        )

ADD_LIBRARY(floatfann SHARED ${floatfann_LIB_SRCS})
ADD_LIBRARY(floatfann_static STATIC ${floatfann_LIB_SRCS})

SET_TARGET_PROPERTIES(floatfann PROPERTIES VERSION ${FANN_VERSION_STRING} SOVERSION ${FANN_VERSION_MAJOR})
SET_TARGET_PROPERTIES(floatfann_static PROPERTIES VERSION ${FANN_VERSION_STRING} SOVERSION ${FANN_VERSION_MAJOR})
if (UNIX)
  SET_TARGET_PROPERTIES(floatfann_static PROPERTIES OUTPUT_NAME floatfann)
endif(UNIX)
INSTALL(TARGETS floatfann floatfann_static LIBRARY DESTINATION ${LIB_INSTALL_DIR}
        ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
        RUNTIME DESTINATION ${BIN_INSTALL_DIR} )


########### next target ###############

SET(doublefann_LIB_SRCS
        doublefann.c
        )

ADD_LIBRARY(doublefann SHARED ${doublefann_LIB_SRCS})
ADD_LIBRARY(doublefann_static STATIC ${doublefann_LIB_SRCS})

SET_TARGET_PROPERTIES(doublefann PROPERTIES VERSION ${FANN_VERSION_STRING} SOVERSION ${FANN_VERSION_MAJOR})
SET_TARGET_PROPERTIES(doublefann_static PROPERTIES VERSION ${FANN_VERSION_STRING} SOVERSION ${FANN_VERSION_MAJOR})
if (UNIX)
  SET_TARGET_PROPERTIES(doublefann_static PROPERTIES OUTPUT_NAME doublefann)
endif(UNIX)
INSTALL(TARGETS doublefann doublefann_static LIBRARY DESTINATION ${LIB_INSTALL_DIR}
        ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
        RUNTIME DESTINATION ${BIN_INSTALL_DIR} )


########### next target ###############

SET(fixedfann_LIB_SRCS
        fixedfann.c
        )

ADD_LIBRARY(fixedfann SHARED ${fixedfann_LIB_SRCS})
ADD_LIBRARY(fixedfann_static STATIC ${fixedfann_LIB_SRCS})

if(NOT MSVC)
TARGET_LINK_LIBRARIES(fixedfann m)
TARGET_LINK_LIBRARIES(fixedfann_static m)
endif(NOT MSVC)

SET_TARGET_PROPERTIES(fixedfann PROPERTIES VERSION ${FANN_VERSION_STRING} SOVERSION ${FANN_VERSION_MAJOR})
SET_TARGET_PROPERTIES(fixedfann_static PROPERTIES VERSION ${FANN_VERSION_STRING} SOVERSION ${FANN_VERSION_MAJOR})
if (UNIX)
  SET_TARGET_PROPERTIES(fixedfann_static PROPERTIES OUTPUT_NAME fixedfann)
endif(UNIX)
INSTALL(TARGETS fixedfann fixedfann_static LIBRARY DESTINATION ${LIB_INSTALL_DIR}
        ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
        RUNTIME DESTINATION ${BIN_INSTALL_DIR} )


########### next target ###############

SET(fann_LIB_SRCS
        floatfann.c
        )

ADD_LIBRARY(fann SHARED ${fann_LIB_SRCS})
ADD_LIBRARY(fann_static STATIC ${fann_LIB_SRCS})

if(NOT MSVC)
TARGET_LINK_LIBRARIES(fann m)
TARGET_LINK_LIBRARIES(fann_static m)
endif(NOT MSVC)

SET_TARGET_PROPERTIES(fann PROPERTIES VERSION ${FANN_VERSION_STRING} SOVERSION ${FANN_VERSION_MAJOR})
SET_TARGET_PROPERTIES(fann_static PROPERTIES VERSION ${FANN_VERSION_STRING} SOVERSION ${FANN_VERSION_MAJOR})
if (UNIX)
  SET_TARGET_PROPERTIES(fann_static PROPERTIES OUTPUT_NAME fann)
endif(UNIX)
INSTALL(TARGETS fann fann_static LIBRARY DESTINATION ${LIB_INSTALL_DIR}
        ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
        RUNTIME DESTINATION ${BIN_INSTALL_DIR} )


################ compile wasm sandboxed fann ###########
SET(fann_LIB_SRCS sandbox_fann.cpp)

find_package(rlbox-sandboxing-api CONFIG REQUIRED)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
add_library(fannwasm fannwasm.c)
add_library(wasm-rt-impl ${CMAKE_SOURCE_DIR}/wasm2c_sandbox_compiler/wasm2c/wasm-rt-impl.c)
add_library(wasm-rt-wasi ${CMAKE_SOURCE_DIR}/wasm2c_sandbox_compiler/wasm2c/wasm-rt-wasi.c)
add_library(wasm-rt-os-unix ${CMAKE_SOURCE_DIR}/wasm2c_sandbox_compiler/wasm2c/wasm-rt-os-unix.c)
ADD_LIBRARY(sandboxfann SHARED ${fann_LIB_SRCS})
target_include_directories(fannwasm PRIVATE ${CMAKE_SOURCE_DIR}/wasm2c_sandbox_compiler/wasm2c/)
target_include_directories(fannwasm PRIVATE ${CMAKE_SOURCE_DIR}/rlbox_wasm2c_sandbox/include/)
target_include_directories(sandboxfann PRIVATE ${CMAKE_SOURCE_DIR}/wasm2c_sandbox_compiler/wasm2c/)
target_include_directories(sandboxfann PRIVATE ${CMAKE_SOURCE_DIR}/rlbox_wasm2c_sandbox/include/)
target_include_directories(sandboxfann PRIVATE /usr/local/include/rlbox)
target_link_libraries(fannwasm rlbox-sandboxing-api wasm-rt-impl wasm-rt-wasi wasm-rt-os-unix)
target_link_libraries(sandboxfann fannwasm)
