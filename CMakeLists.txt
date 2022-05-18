cmake_minimum_required(VERSION 3.7.1)

project(ChocoPy-Compiler)

# required package flex bison llvm
find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)
find_package(FMT REQUIRED)
find_package(cJSON REQUIRED)
# find_package(LLVM REQUIRED CONFIG)
include_directories(${PROJECT_SOURCE_DIR}/src/)

# llvm environment set
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})
message(STATUS "Using cJSON ${CJSON_INCLUDE_DIRS} with ${CJSON_LIBRARIES}")
include_directories(${CJSON_INCLUDE_DIRS})
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

#find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

# Workaround for using clion+wsl
if ($ENV{CLION_IDE_WSL})
    include_directories("/usr/include/llvm-${LLVM_VERSION_MAJOR}")
endif ()

# set the directory output
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
SET(CMAKE_CXX_STANDARD 20)

# Add LLVM toolchain support
if (${APPLE})
    SET(CMAKE_C_FLAGS "-std=c99")
    SET(CMAKE_C_FLAGS_DEBUG "-g")
    SET(CMAKE_C_FLAGS_MINSIZEREL "-Os -DNDEBUG")
    SET(CMAKE_C_FLAGS_RELEASE "-O4 -DNDEBUG")
    SET(CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g")
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
        message("Using AppleClang")
        SET(CMAKE_CXX_FLAGS "-D_GLIBCXX_USE_CXX11_ABI=0")
    else ()
        SET(CMAKE_CXX_FLAGS "")
    endif ()
    SET(CMAKE_CXX_FLAGS_DEBUG "-g")
    SET(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG")
    SET(CMAKE_CXX_FLAGS_RELEASE "-O4 -DNDEBUG")
    SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
    # SET(CMAKE_OSX_ARCHITECTURES arm64)
    # msvc support, using winflexbison and clang-cl
elseif (MSVC)
    include_directories(include/win_extra)
    SET(CMAKE_CXX_STANDARD 23)

    set(CMAKE_CXX_FLAGS
            "${CMAKE_CXX_FLAGS} /Zc:__cplusplus /std:c++latest /MP /Z7 /D \"_CRT_SECURE_NO_WARNINGS\" /D \"_ENABLE_EXTENDED_ALIGNED_STORAGE\"")
else ()
    set(CMAKE_C_FLAGS "-std=c99")
    set(CMAKE_C_FLAGS_DEBUG "-g")
    set(CMAKE_C_FLAGS_MINSIZEREL "-Os -DNDEBUG")
    set(CMAKE_C_FLAGS_RELEASE "-O4 -DNDEBUG")
    set(CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g")
    set(CMAKE_CXX_FLAGS "")
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "-O4 -DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
endif ()

# Add block directories
add_subdirectory(tools)
add_subdirectory(src/cgen/stdlib)
add_subdirectory(src/parser)
add_subdirectory(src/semantic)
add_subdirectory(src/cgen)
add_subdirectory(src/ir-optimizer)
