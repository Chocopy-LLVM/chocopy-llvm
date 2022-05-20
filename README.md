# ChocoPy PA
[Online Doc](https://chocopy-llvm.github.io/chocopy-llvm)|[中文文档](https://chocopy-llvm.github.io/chocopy-llvm/#/zh-cn/)

The design of the lightir and optimization refer to [USTC](https://github.com/mlzeng/CSC2020-USTC-FlammingMyCompiler)'s design and the spec of chocopy is from [UC Berkeley](https://inst.eecs.berkeley.edu/~cs164/sp19/index.html).

## PA1
Compile the python 3.6 front-end code to AST tree. [Start from using docker](common/build.md) and see the [doc](PA1/README.md).

## PA2
Compile the python 3.6 front-end code to type-checked AST tree. See the [doc](PA2/README.md).

## PA3
Compile type-checked AST tree to LightIR code. See the [doc](PA3/README.md).

For bonus project, do optimization pass on LightIR code.  [Start learning basics of lightIR](common/lightir.md) and see the [doc](PA3/README.md). Note that there have been changes on [stdlib](common/stdlib.md) after the publication of the PAs.

## PA4
Compile the LightIR code to RiscV Assembly. See the [doc](PA4/README.md).
