# 材料与代码阅读

## 前言
阅读代码前请仔细阅读下面关于`LoopSearch`和`Mem2Reg`部分的说明，最后该部分需要在实验报告中写下**思考题**的答案。

`Control Flow Graph`(以下简称为`CFG`)是一种用有向图来描述程序执行过程的方式，它是对程序块之间的控制流建立的联系。其中，图中的节点表示基本块(`basic block`)，基本块中，除了最后一条指令，其余的指令都是顺序执行不会产生分支跳转；图中的边则表示可能的控制转移。


通过对`CFG`的分析，可以找到代码的许多重要结构信息，比如循环。所以`CFG`对于代码优化有着重要的意义，而本次实验的优化Pass，都是建立在对`CFG`的分析与修改上的。在之前的实验中，同学们通过调用LightIR的接口(参考[LightIR介绍](../common/LightIR.md))完成了中间代码生成，同时也通过核心类建立和维护着中间代码的`CFG`，核心类中，`BasicBlock`就可以看作是`CFG`的基本块，类中的成员`pre_bbs_`与`succ_bbs_`就可以记下每个块的前驱后继，通过递归地访问每个块的上述两个成员，就可以遍历整个`CFG`。在本次代码阅读实验部分，同学们请仔细阅读样例代码，掌握如何去调用`LightIR`的接口来开发优化Pass。

## LoopSearch
#### 简介
循环查找是属于一种**分析类Pass**，该类Pass是用来为之后的优化Pass获取`CFG`的必要信息，而不会对`CFG`本身造成改变；循环查找的主要目的就是为了找到程序当中的所有循环，以及这些循环的入口块（即循环的条件块）等信息。**该分析Pass也是后续的循环不变式外提优化的基础。**
#### 代码阅读说明
1. 代码相关的两个类分别是`LoopSearch`以及`LoopSearch`
2. 循环查找会用到图强连通分量搜索算法--Tarjan algorithm，如果代码阅读时遇到问题可以自行查找相关资料辅助阅读。
3. 为了方便阅读以及后续实验debug方便，该模块还设计了一个将找到的所有循环可视化的模块。使用该模块有以下几点需要注意：
    * 确保环境中安装了Graphviz，如果没有可以使用命令安装：
    ```bash
    sudo apt-get install graphviz
    ```
    * 每个BasicBlock都需要命名，且命名要符合规范。
    * 该接口是通过`LoopSearch`的构造函数中的dump参数来控制是否打印找到的循环，dump为true时会打印，否则将不打印，发布的代码中是**默认不打印的**：
    ```cpp
    explicit LoopSearch(Module* m, bool dump=false) : Pass(m), dump(dump){}
    ```
    * 该功能会在当前目录下输出png图片文件，文件有两种命名：
        a. `function_name`.png：表示某个函数内所有的basic block连接。
        b. `function_name`_`index`.png：表示在某函数中第`index`个循环。
4. 重要的成员变量和函数说明（后续实验会用到，阅读代码需要着重理解）：
    * `loop_set`：算法找到的所有循环的集合
    * `func2loop`：记录函数与循环的对应关系
    * `base2loop`：记录base块与循环的对应关系
    * `loop2base`：记录base块与循环的对应关系
    * `bb2base`：记录循环中的一个块到这个块所在循环（最内层循环）的入口的映射关系。
    * `get_loop_base`：得到循环的入口块
    * `get_inner_loop`：得到bb所在最低层次的loop 
    * `get_parent_loop`：得到输入loop的外一层的循环，如果没有则返回空
    * `get_loops_in_func`：得到某个函数内的所有循环


#### 思考题
1. 循环的入口如何确定？循环的入口的数量可能超过1嘛？

2. 简述一下算法怎么解决循环嵌套的情况。


## Mem2Reg

#### 简介

`Mem2Reg Pass`构造了LLVM IR 的SSA格式(静态单赋值格式)。关于SSA是什么，为什么要构建SSA格式，以及如何构建附件材料中均有提及，也可以通过自行搜索材料了解。

#### 代码说明

1. 代码相关在`src/ir-optimizer/chocopy_optimization.cpp` 以及`include/ir-optimizer/chocopy_optimization.hpp`
2. 其中`Dominators`是生成支配树信息，被`Mem2Reg`调用。
3. `Mem2Reg Pass`的执行流程与算法**伪代码详见附件**：(提示：注意了解其中的phi函数节点)

#### 思考题：（描述清楚即可，不需要写很多字数）

1. 请简述支配边界的概念。

2. 请简述`phi`节点的概念，与其存在的意义。

3. 请描述`Mem2Reg Pass`执行前后的`ir`的变化, 简述一下。

   before `Mem2Reg`：

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
   label8:                                                ; preds = %label_entry
     %op9 = load i32, i32* %op2
     ret i32 %op9
   label10:                                                ; preds = %label_entry
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
   label10:                                                ; preds = %label_entry
     %op11 = load i32, i32* %op0
     store i32 %op11, i32* %op2
     %op12 = load i32, i32* %op1
     store i32 %op12, i32* %op0
     %op13 = load i32, i32* %op2
     store i32 %op13, i32* %op1
     br label %label14
   label14:                                                ; preds = %label_entry, %label10
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

4. 在放置phi节点的时候，算法是如何利用支配树的信息的？

5. 算法是如何选择`value`(变量最新的值)来替换`load`指令的？（描述数据结构与维护方法）
