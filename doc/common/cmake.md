# cmake 工具使用

## 编译系统的简单介绍

详情可以参考这个[ppt](https://www.victoryang00.cn/BuildSystem.pdf)

## CMake 的简单介绍

CMake 是一个简易地跨平台编译系统，能自动识别输入的操作系统及编译器并生成平台可用的编译文件。如 Windows 通过 `-G` 可以指定 `Visual Studio 16`， `Ninja` 或者 `msys2`， MacOS
通过 `-G` 可以指定 `XCode`。同时CMake还支持不同语言，如`C++`,`Fortran`,`Cuda`等。

CMake 通过 `add_excutable()` 指定编译生成可执行文件， `add_library()` 指定编译生成库。通过 `set(CMAKE_CXX_FLAGS)` 可以指定当前文件的编译选项。

CMake 本身是一个语言，可以简单的写一个 [Quick Sort](https://zhuanlan.zhihu.com/p/121079042) ，也可以做字符串替换，拷贝文件名，但一般所有的可改变文件的作用域在子文件夹内。

```cmake
cmake_minimum_required(VERSION 3.11)

project(CMakeHeap)

function(heap_adjust heap_ index n)
    set(heap__ "${${heap_}}")
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
    set(heap__ "${${heap_}}")
    list(LENGTH heap__ n)
    math(EXPR i "${n} / 2 - 1")
    while (i GREATER_EQUAL 0)
        heap_adjust(heap__ ${i} ${n})
        math(EXPR i "${i} - 1")
    endwhile ()
    set(${heap_} ${heap__} PARENT_SCOPE)
endfunction()

function(heap_sort heap_)
    set(heap__ "${${heap_}}")
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

对于不同的编译器，CMake 内部会实现很多可用的函数，需要通过 `find_library()` 找到其对应的`FMT-Config.cmake`，比如 `FLEX` 和 `BISON`
可以通过 `ADD_FLEX_BISON_DEPENDENCY()` 及 `flex_target()` `bison_target()` 完成。 `LLVM` 则会提供很多宏如 `${LLVM_DEFINITIONS_LIST}`.

考虑不同体系结构及操作系统的问题时，可以通过 `if($ENV{CLION_IDE_WSL})` 或者 `elseif (MSVC)` 来知道并自行定义 C++ 可知的宏及编译器/编译选项。

对于多目录 CMake 构建系统，仅需在每个文件夹中创建 CMakeList.txt，在上层目录的 CMakeList.txt 中添加 `add_subdirectory()` 即可，有 `${PROJECT_CURRENT_DIR}`
来访问当前目录，同时 `${PROJECT_SOURCE_DIR}` 用于访问根源码目录， `${PROJECT_BINARY_DIR}` 用于访问构建所在目录 `[build_dir]`。