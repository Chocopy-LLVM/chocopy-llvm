# 文件结构介绍

整个`repo`的结构如下

```
.
├── CMakeLists.txt
├── web    <- 网页端对拍
├── doc    <- 实验文档说明
├── test   <- 测例说明
├── READMD.md
├── reports
│   ├── PA1
│   │   └── report.md  <- lab1所需提交的实验报告（你需要在此提交实验报告）
│   ├── PA2
│   │   └── report.md  <- lab2所需提交的实验报告（你需要在此提交实验报告）
│   ├── PA3
│   │   └── report.md  <- lab3所需提交的实验报告（你需要在此提交实验报告）
│   └── PA4
│       └── report.md  <- lab4所需提交的实验报告（你需要在此提交实验报告）
├── include <- 实验所需的头文件
│   ├── cgen
│   │   ├── InstGen.hpp
│   │   ├── RiscVBackEnd.hpp
│   │   └── chocopy_cgen.hpp
│   ├── ir-optimizer
│   │   ├── BasicBlock.hpp
│   │   ├── Class.hpp
│   │   ├── Constant.hpp
│   │   ├── Function.hpp
│   │   ├── GlobalVariable.hpp
│   │   ├── IRBuilder.hpp
│   │   ├── IRprinter.hpp
│   │   ├── Module.hpp
│   │   ├── Optimization.hpp
│   │   ├── Type.hpp
│   │   ├── User.hpp
│   │   ├── Value.hpp
│   │   └── chocopy_llvm.hpp
│   ├── parser
│   │   ├── chocopy_ast.hpp
│   │   ├── chocopy_logging.hpp
│   │   └── chocopy_parse.hpp
│   ├── semantic
│   │   ├── ClassDefType.hpp
│   │   ├── FunctionDefType.hpp
│   │   ├── SymbolTable.hpp
│   │   ├── SymbolType.hpp
│   │   ├── ValueType.hpp
│   │   └── chocopy_semant.hpp
│   └── win_extra <- MSVC兼容层
│       └── FlexLexer.h
├── src  <- 源代码
│   ├── cgen
│   │   ├── CMakeLists.txt
│   │   ├── chocopy_cgen.cpp <- codegen逻辑文件，lab4 bonus所需完善的文件
│   │   └── stdlib <- 标准库所在目录
│   │       ├── CMakeLists.txt
│   │       ├── abort.s
│   │       ├── alloc.s
│   │       ├── alloc2.s
│   │       ├── chocopy_stdlib.S <- 链接所需文件，由CMake生成
│   │       ├── concat.s
│   │       ├── conslist.s
│   │       ├── heap.init.s
│   │       ├── input.s
│   │       ├── len.s
│   │       ├── object.__init__.s
│   │       ├── preprocess
│   │       ├── print.s
│   │       ├── strcat.s
│   │       ├── streql.s
│   │       └── strneql.s
│   ├── ir-optimizer
│   │   ├── BasicBlock.cpp
│   │   ├── CMakeLists.txt
│   │   ├── Class.cpp
│   │   ├── Constant.cpp
│   │   ├── Function.cpp
│   │   ├── GlobalVariable.cpp
│   │   ├── IRprinter.cpp
│   │   ├── Instruction.cpp
│   │   ├── Module.cpp
│   │   ├── Optimization.cpp <- lightir-opt文件，lab3 bonus所需完善的文件
│   │   ├── Type.cpp
│   │   ├── User.cpp
│   │   ├── Value.cpp
│   │   └── chocopy_llvm.cpp <- lightir逻辑文件，lab3所需完善的文件
│   ├── parser
│   │   ├── CMakeLists.txt
│   │   ├── chocopy.l <- flex文件，lab1所需完善的文件
│   │   ├── chocopy.y <- bison文件，lab1所需完善的文件
│   │   ├── chocopy_ast.cpp
│   │   ├── chocopy_logging.cpp
│   │   └── chocopy_parse.cpp
│   └── semantic
│       ├── CMakeLists.txt
│       └── chocopy_semant.cpp <- semantic逻辑文件，lab2所需完善的文件
└── tests	<- 测试文件
│   ├── pa1
│   │   └── sample  <- lab1的测试与参考结果
│   ├── pa2
│   │   └── sample  <- lab2的测试与参考结果
│   ├── pa3
│   │   └── sample  <- lab3的测试与参考结果
│   └── pa4
│       └── sample  <- lab4的测试与参考结果
```

