## Material and code reading

## Preface
Before reading the code, please read carefully the following descriptions of the `LoopSearch` and `Mem2Reg` sections, and the final section requires answers to **thinking questions** in the lab report.

The `Control Flow Graph` (hereafter referred to as `CFG`) is a way to describe the process of program execution in terms of a directed graph, which is a linkage established for the control flow between program blocks. Among them, the nodes in the graph represent the basic block (`basic block`), in which, except for the last instruction, the rest of the instructions are executed sequentially without branching jumps; the edges in the graph represent possible control transfers.


By analyzing the `CFG`, many important structural information of the code can be found, such as loops. So `CFG` has an important meaning for code optimization, and the optimization Pass of this experiment is all based on the analysis and modification of `CFG`. In the previous experiments, students called LightIR's interface by calling (refer to [Introduction to LightIR](./common/LightIR.md)) to complete the intermediate code generation, but also through the core class to build and maintain the intermediate code `CFG`, the core class, `BasicBlock` can be regarded as the basic block of `CFG`, the class members `pre_bbs_` and `succ_bbs_` can record the predecessor and successor of each block. By recursively accessing the above two members of each block, the entire `CFG` can be traversed. In this code reading lab section, students should read the sample code carefully to master how to call the `LightIR` interface to develop optimized Pass.

## LoopSearch
#### Introduction
LoopSearch is a type of **Analysis Pass** that is used to obtain the necessary information about the `CFG` for the subsequent Optimization Pass without changing the `CFG` itself; the main purpose of LoopSearch is to find all the loops in the program and the entry blocks of these loops (i.e., the conditional blocks of the loops). ** This analysis of Pass is also the basis for the subsequent optimization of loop invariant outer lift. **
#### Code Reading Instructions
1. the two code related classes are `LoopSearch` and `LoopSearch`
2. loop search will use the graph of strong connected component search algorithm - Tarjan algorithm, if you encounter problems when reading the code can find the relevant information to assist reading.
3. In order to facilitate reading and subsequent experimental debug convenience, the module is also designed to visualize all the loops found module. The following points should be noted when using this module.
    * Make sure that Graphviz is installed in your environment, if not you can install it using the command.
    ```bash
    sudo apt-get install graphviz
    ```
    * Each BasicBlock needs to be named, and the naming needs to conform to the specification.
    * The interface is controlled by the dump parameter in the constructor of `LoopSearch` whether to print the loop found, it will be printed when dump is true, otherwise it will not be printed, the released code is **not printed by default**.
    ```cpp
    explicit LoopSearch(Module* m, bool dump=false) : Pass(m), dump(dump){}
    ```
    * This function outputs a png image file in the current directory. The file has two kinds of naming.
        a. `function_name`.png: indicates all the basic block connections within a function.
        b. `function_name`_`index`.png: indicates the `index` loop in a function.
4. Important member variables and function descriptions (which will be used in subsequent experiments and need to be read with emphasis on understanding the code).
    * `loop_set`: the set of all loops found by the algorithm
    * `func2loop`: records the correspondence between functions and loops
    * `base2loop`: records the correspondence between base blocks and loops
    * `loop2base`: records the correspondence between base blocks and loops
    * `bb2base`: records the mapping of a block in a loop to the entry of the loop (innermost loop) in which this block is located.
    * `get_loop_base`: gets the entry block of the loop
    * `get_inner_loop`: get the lowest level loop where bb is located 
    * `get_parent_loop`: get the loop at the outer level of the input loop, or return null if there is none
    * `get_loops_in_func`: get all the loops inside a function


#### Thinking Questions
1. How is the entry point of a loop determined? Is it possible to have more than 1 loop entry point?

2. Briefly describe how the algorithm solves the case of nested loops.


## Mem2Reg

#### Introduction

The `Mem2Reg Pass` constructs the SSA format (static single assignment format) of LLVM IR. What SSA is, why it is constructed, and how it is constructed are mentioned in the attached material, or you can learn about it by searching the material yourself.

#### code description

1. the code is related in `src/ir-optimizer/chocopy_optimization.cpp` and `include/ir-optimizer/chocopy_optimization.hpp`.
2. where `Dominators` is to generate domination tree information, called by `Mem2Reg`.
3. The execution flow and algorithm of `Mem2Reg Pass` **Pseudo-code details can be found in the attachment**: (Hint: pay attention to understand the phi function node in it)

#### Thinking Questions: (Just describe clearly, you don't need to write a lot of words)

1. Please briefly describe the concept of dominating boundaries.

2. Please briefly describe the concept of `phi` node and the significance of its existence.

3. describe the change of `ir` before and after the execution of `Mem2Reg Pass`, briefly.

   before `Mem2Reg`.

   ```c
   ; ModuleID = 'ChocoPy'
   source_filename = "complex3.py"
   
   declare i32 @input()
   
   declare void @output(i32)
   
   declare void @outputFloat(float)
   
   declare void @neg_idx_except()
   
   define i32 @gcd(i32 %arg0, i32 %arg1) {
   label_entry:
     %op2 = alloca i32
     store i32 %arg0, i32* %op2
     %op3 = alloca i32
     store i32 %arg1, i32* %op3
     %op4 = load i32, i32* %op3
     %op5 = icmp eq i32 %op4, 0
     %op6 = zext i1 %op5 to i32
     %op7 = icmp ne i32 %op6, 0
     br i1 %op7, label %label8, label %label10
   label8: ; preds = %label_entry
     %op9 = load i32, i32* %op2
     ret i32 %op9
   label10: ; preds = %label_entry
     %op11 = load i32, i32* %op3
     %op12 = load i32, i32* %op2
     %op13 = load i32, i32* %op2
     %op14 = load i32, i32* %op3
     %op15 = sdiv i32 %op13, %op14
     %op16 = load i32, i32* %op3
     %op17 = mul i32 %op15, %op16
     %op18 = sub i32 %op12, %op17
     %op19 = call i32 @gcd(i32 %op11, i32 %op18)
     ret i32 %op19
   }
   define void @main() {
   label_entry:
     %op0 = alloca i32
     %op1 = alloca i32
     %op2 = alloca i32
     %op3 = call i32 @input()
     store i32 %op3, i32* %op0
     %op4 = call i32 @input()
     store i32 %op4, i32* %op1
     %op5 = load i32, i32* %op0
     %op6 = load i32, i32* %op1
     %op7 = icmp slt i32 %op5, %op6
     %op8 = zext i1 %op7 to i32
     %op9 = icmp ne i32 %op8, 0
     br i1 %op9, label %label10, label %label14
   label10: ; preds = %label_entry
     %op11 = load i32, i32* %op0
     store i32 %op11, i32* %op2
     %op12 = load i32, i32* %op1
     store i32 %op12, i32* %op0
     %op13 = load i32, i32* %op2
     store i32 %op13, i32* %op1
     br label %label14
   label14: ; preds = %label_entry, %label10
     %op15 = load i32, i32* %op0
     %op16 = load i32, i32* %op1
     %op17 = call i32 @gcd(i32 %op15, i32 %op16)
     store i32 %op17, i32* %op2
     %op18 = load i32, i32* %op2
     call void @output(i32 %op18)
     ret void
   }
   ```
      After `Mem2Reg`：

   ```c
   ; ModuleID = 'ChocoPy'
   source_filename = "complex3.py"
   
   declare i32 @input()
   
   declare void @output(i32)
   
   declare void @outputFloat(float)
   
   declare void @neg_idx_except()
   
   define i32 @gcd(i32 %arg0, i32 %arg1) {
   label_entry:
     %op5 = icmp eq i32 %arg1, 0
     %op6 = zext i1 %op5 to i32
     %op7 = icmp ne i32 %op6, 0
     br i1 %op7, label %label8, label %label10
   label8:                                                ; preds = %label_entry
     ret i32 %arg0
   label10:                                                ; preds = %label_entry
     %op15 = sdiv i32 %arg0, %arg1
     %op17 = mul i32 %op15, %arg1
     %op18 = sub i32 %arg0, %op17
     %op19 = call i32 @gcd(i32 %arg1, i32 %op18)
     ret i32 %op19
   }
   define void @main() {
   label_entry:
     %op3 = call i32 @input()
     %op4 = call i32 @input()
     %op7 = icmp slt i32 %op3, %op4
     %op8 = zext i1 %op7 to i32
     %op9 = icmp ne i32 %op8, 0
     br i1 %op9, label %label10, label %label14
   label10:                                                ; preds = %label_entry
     br label %label14
   label14:                                                ; preds = %label_entry, %label10
     %op19 = phi i32 [ %op3, %label10 ], [ undef, %label_entry ]
     %op20 = phi i32 [ %op4, %label_entry ], [ %op3, %label10 ]
     %op21 = phi i32 [ %op3, %label_entry ], [ %op4, %label10 ]
     %op17 = call i32 @gcd(i32 %op21, i32 %op20)
     call void @output(i32 %op17)
     ret void
   }
   ```

4. How does the algorithm use the information from the dominant tree when placing the phi nodes?

5. How does the algorithm select `value` (the latest value of the variable) to replace the `load` instruction? (Describe the data structure and maintenance methods)
