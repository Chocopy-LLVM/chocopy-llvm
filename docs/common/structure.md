# Introduction to the file structure

The structure of the entire `repo` is as follows

```
.
├── CMakeLists.txt
├── web    <- Web-side pair shot
├── doc    <- Experimental documentation
├── test   <- Test case description
├── READMD.md
├── reports
│   ├── PA1
│   │   └── report.md  <- Lab1 lab report (you need to submit the lab report here)
│   ├── PA2
│   │   └── report.md  <- Lab2 lab report (you need to submit the lab report here)
│   ├── PA3
│   │   └── report.md  <- lab3 required lab report (you need to submit the lab report here)
│   └── PA4
│       └── report.md  <- Lab report required for lab4 (you need to submit the lab report here)
├── include <- Required header files for the experiment
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
│   └── win_extra <- MSVC compatible layer
│       └── FlexLexer.h
├── src  <- Source code
│   ├── cgen
│   │   ├── CMakeLists.txt
│   │   ├── chocopy_cgen.cpp <- codegen logic files, lab4 bonus required for refinement
│   │   └── stdlib <- directory where the standard library is located
│   │       ├── CMakeLists.txt
│   │       ├── abort.s
│   │       ├── alloc.s
│   │       ├── alloc2.s
│   │       ├── chocopy_stdlib.S <- Files required for linking, generated by CMake
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
│   │   ├── Optimization.cpp <- lightir-opt file, lab3 bonus required for refinement
│   │   ├── Type.cpp
│   │   ├── User.cpp
│   │   ├── Value.cpp
│   │   └── chocopy_llvm.cpp <- lightir logic file, required for lab3
│   ├── parser
│   │   ├── CMakeLists.txt
│   │   ├── chocopy.l <- flex file, the file required by lab1
│   │   ├── chocopy.y <- bison files, files for lab1 to refine
│   │   ├── chocopy_ast.cpp
│   │   ├── chocopy_logging.cpp
│   │   └── chocopy_parse.cpp
│   └── semantic
│       ├── CMakeLists.txt
│       └── chocopy_semant.cpp <- semantic logic files, lab2 refinements
└── tests	<- Test files
│   ├── pa1
│   │   └── sample  <- lab1 test and reference results
│   ├── pa2
│   │   └── sample  <- lab2 test and reference results
│   ├── pa3
│   │   └── sample  <- lab3's test and reference results
│   └── pa4
│       └── sample  <- Test and reference results of lab4
```

