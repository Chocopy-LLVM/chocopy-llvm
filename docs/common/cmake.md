# cmake tool usage

## A brief introduction to the compilation system

For more details, please refer to this [ppt](https://www.victoryang00.cn/BuildSystem.pdf)

## A brief introduction to CMake

CMake is a simple cross-platform compilation system that automatically recognizes the input operating system and compiler and generates platform-ready compiled files. For example, Windows can specify `Visual Studio 16`, `Ninja` or `msys2` with `-G`, MacOS
`-XCode` can be specified with `-G`. CMake also supports different languages such as `C++`, `Fortran`, `Cuda`, etc.

CMake is compiled with `add_excutable()` to generate executables and `add_library()` to generate libraries. The compilation options for the current file can be specified with `set(CMAKE_CXX_FLAGS)`.

CMake itself is a language that can simply write a [Quick Sort](https://zhuanlan.zhihu.com/p/121079042), or do string substitution and copy filenames, but generally all changeable files have scopes within subfolders.

```cmake
cmake_minimum_required(VERSION 3.11)

project(CMakeHeap)

function(heap_adjust heap_ index n)
    set(heap__ "${${heap_}}}")
    list(APPEND heap__ 0)
    math(EXPR n2 "${n} / 2 - 1")
    while (index LESS_EQUAL n2)
        set(next ${index})
        math(EXPR l "${index} * 2 + 1")
        list(GET heap__ ${next} x)
        list(GET heap__ ${l} y)
        if (x LESS y)
            set(next ${l})
        endif ()
        math(EXPR r "${index} * 2 + 2")
        if (r LESS n)
            list(GET heap__ ${next} x)
            list(GET heap__ ${r} y)
            if (x LESS y)
                set(next ${r})
            endif ()
        endif ()
        if (next EQUAL index)
            break()
        endif ()
        list(GET heap__ ${index} x)
        list(GET heap__ ${next} y)
        list(REMOVE_AT heap__ ${next})
        list(REMOVE_AT heap__ ${index})
        list(INSERT heap__ ${index} ${y})
        list(INSERT heap__ ${next} ${x})
        set(index ${next})
    endwhile ()
    list(REMOVE_AT heap__ -1)
    set(${heap_} ${heap__} PARENT_SCOPE)
endfunction()

function(heap_make heap_)
    set(heap__ "${${heap_}}}")
    list(LENGTH heap__ n)
    math(EXPR i "${n} / 2 - 1")
    while (i GREATER_EQUAL 0)
        heap_adjust(heap__ ${i} ${n})
        math(EXPR i "${i} - 1")
    endwhile ()
    set(${heap_} ${heap__} PARENT_SCOPE)
endfunction()

function(heap_sort heap_)
    set(heap__ "${${heap_}}}")
    list(LENGTH heap__ n)
    list(APPEND heap__ 0)
    math(EXPR i "${n} - 1")
    while (i GREATER 0)
        list(GET heap__ 0 x)
        list(GET heap__ ${i} y)
        list(REMOVE_AT heap__ ${i})
        list(REMOVE_AT heap__ 0)
        list(INSERT heap__ 0 ${y})
        list(INSERT heap__ ${i} ${x})
        heap_adjust(heap__ 0 ${i})
        math(EXPR i "${i} - 1")
    endwhile ()
    list(REMOVE_AT heap__ -1)
    set(${heap_} ${heap__} PARENT_SCOPE)
endfunction()


set(heap 22 39 35 29 6 25 16 20 34 21 33 30 11 14 10 15 31 42 32 8 4 41 18 5 37 24 23 12 27 38 40 13 7 36 19 2 17 1 3 26 9 28)
message("Input: ${heap}")

heap_make(heap)
message("Heap: ${heap}")

heap_sort(heap)
message("Output: ${heap}")
```

For different compilers, CMake implements many functions internally, you need to find the corresponding `FMT-Config.cmake` with `find_library()`, for example `FLEX` and `BISON`.
can be done with `ADD_FLEX_BISON_DEPENDENCY()` and `flex_target()` `bison_target()`. `LLVM` provides many macros such as `${LLVM_DEFINITIONS_LIST}`.

When considering different architectures and operating systems, you can use `if($ENV{CLION_IDE_WSL})` or `elseif (MSVC)` to know and define your own C++-aware macros and compiler/compiler options.

For a multi-directory CMake build system, simply create CMakeList.txt in each folder, add `add_subdirectory()` to CMakeList.txt in the upper directory, have `${PROJECT_CURRENT_DIR}`
to access the current directory, `${PROJECT_SOURCE_DIR}` to access the root code directory, and `${PROJECT_BINARY_DIR}` to access the build directory `[build_dir]`.