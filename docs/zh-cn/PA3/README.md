# Programing Assignment III 文档

<!-- TOC -->

- [Programing Assignment III 文档](#programing-assignment-iii-文档)
- [lab3-实验文档](#lab3-实验文档)
  - [0. 基础知识](#0-基础知识)
    - [0.1 Light IR 类型设计](#01-light-ir-类型设计)
      - [0.1.1 Light IR Constant/Global Variable](#011-light-ir-constantglobal-variable)
      - [0.1.2 Light IR Function](#012-light-ir-function)
        - [普通 Function](#普通-function)
        - [嵌套 Function](#嵌套-function)
        - [Function Call](#function-call)
      - [0.1.3 Light IR Class](#013-light-ir-class)
        - [Class 定义](#class-定义)
        - [Method Invocation](#method-invocation)
        - [Method Dispatched](#method-dispatched)
    - [0.2 Light IR stdlib 调用](#02-light-ir-stdlib-调用)
  - [1. 实验要求](#1-实验要求)
    - [1.1 代码结构](#11-代码结构)
    - [1.2 主要工作](#12-主要工作)
      - [1.2.1  几点说明](#121--几点说明)
    - [1.3 Bonus](#13-bonus)
    - [1.4 编译、运行和验证](#14-编译运行和验证)
    - [1.5 评分](#15-评分)

<!-- /TOC -->

# lab3-实验文档

本次实验是组队实验，请仔细阅读组队要求，并合理进行分工合作。 本次实验和 Lab3 一样，需要使用 `LightIR` 框架自动产生 `ChocoPy` 语言的LLVM IR，实现需要参考 `ChocoPy` 的 Operational Semantic 部分。 建议在实验前队伍内进行讨论，以确保成员都理解 Lab3 中的问题。

相信大家已经掌握了 LightIR 的使用，并且对于 LLVM IR 也有了一定理解。在本次实验中，我们要使用访问者模式来实现 IR 自动生成。 对于产生的IR， 我们可以调用 clang 生成可执行文件，这样一个初级的 ChocoPy 编译器就完成啦！

注意：组队实验意味着合作，但是小组间的交流是受限的，且严格**禁止**代码的共享。除此之外，如果小组和其它组进行了交流，必须在根目录 `README.md` 中记录下来交流的小组和你们之间交流内容。同时，需要在此文件中更新两位同学的邮箱和代码WriteUp，详见[WriteUp要求](../PA2/README.md#14-writeup)。

## 0. 基础知识

### 0.1 Light IR 类型设计

首先在 `LightIR` 上有 `LLVM` 的 primitive type: i8/i32/arr/ptr. 在此之上定义了 str 为 i8*。prototype 被定义为 class。完整的类型定义在 `Type.hpp` 中。所有的 `Type` 都可以通过工厂模式 `::get()` 得到。

```c++
enum type { CLASS_ANON = -7, LABEL, FUNC, VOID, UNION, VECTOR, LIST, OBJECT, INT, BOOL, STRING, CLASS };
```
类型间可以通过三目表达式 `<=>` 比较大小和判断 subclass/subtype。 `FunctionType` 可以得到 Argument/Return 的类型。 `ArrayType` 可以得到元素类型。`VectorType` 是生成向量化代码的中间 kernel 传递形式。`Union` 是定义范性传参的类型。`ListType` 用于对 `$.list$prototype` 的定义，它是继承 `Class`，需要单独定义是因为生成的时候还是需要每个内部类型 list 定义一个 `$strlist$prototype_type` 用于穿参。

每个 `Value` 都会有一个 `Type`。

#### 0.1.1 Light IR Constant/Global Variable

Constant 和 Global Variable 都会被定义在 `Constant*` 或 `GlobalVariable` 中，前者可以作为后者的初始化工具。在声明以后，会在 header 部分给出定义。

```c++
GlobalVariable::create(fmt::format("const_{}", const_id), &*module, new ConstantStr(STR_T, node.value, const_id));
```

#### 0.1.2 Light IR Function


##### 普通 Function
先参考 Function Definition 定义。

$$g_{1}, \ldots, g_{L}\text{ are the variables explicitly declared as global in }f\\
y_{1}=e_{1}, \ldots, y_{k}=e_{k}\text{ are the local variables and nested functions defined in }f\\
E_{f}=E\left[G\left(g_{1}\right) / g_{1}\right] \ldots\left[G\left(g_{L}\right) / g_{L}\right]\\ \hline
v=\left(x_{1}, \ldots, x_{n}, y_{1}=e_{1}, \ldots, y_{k}=e_{k}, b_{\text {body }}, E_{f}\right)G, E, S \vdash def f\left(x_{1}: T_{1}, \ldots, x_{n}: T_{n}\right) [ \rightarrow T_{0} ]^{?}: b: v, S,_{-}$$

定义
```c++
auto func_type = FunctionType::get(string_to_type(return_type), tmp_func_params);
auto function Function::create(func_type, name, module.get());
```
Function 可以定义在 main 之前调用，只需要初始化 `is_ctor` 变量。
##### 嵌套 Function
嵌套 function 虽然在上面有所定义，即被 `E` 所捕获的变量也应该传入参数，但是在 `LightIR` 上的定义更像是 `C++` lambda `[&]{}` 的定义，即捕获所有变量的引用，统一通过 `$class.anon` 传参，放入第一个参数传递。

如
```python
x:int = 0
def crunch(zz:[[int]]) -> object:
    z:[int] = None
    global x
    def make_z() -> object:
        nonlocal z
        for z in zz:
            pass # Set z to last element in zz

    make_z()
    for x in z:
        pass # Set x to last element in z

crunch([[1,2],[2,3],[4,5],[6,7]])
print(x) 
```
需要被定义成下面
```llvm
define void @$crunch(%$.list$prototype_type* %arg0) {

label1:
  %op2 = alloca i32
  %op3 = alloca %$class.anon_make_z, align 4
  %op4 = getelementptr %$class.anon_make_z, %$class.anon_make_z* %op3, i32 0, i32 0
  store i32* %op2, i32** %op4
  %op5 = getelementptr %$class.anon_make_z, %$class.anon_make_z* %op3, i32 0, i32 1
  store %$.list$prototype_type* %arg0, %$.list$prototype_type** %op5
  call void @$crunch.make_z(%$class.anon_make_z* %op3)
  %op6 = load i32, i32*@x
  br label %label7

label7:                                                ; preds = %label1, %label13
  %op8 = phi i32 [ %op6, %label1 ], [ %op11, %label13 ]
  %op9 = icmp ne i32 %op6, %op8
  %op10 = getelementptr i32, i32* %op2, i32 %op8
  %op11 = add i32 %op8, 1
  %op12 = load i32, i32* %op10
  br label %label13

label13:                                                ; preds = %label7
  br  i1 %op9, label %label7, label %label14

label14:                                                ; preds = %label13
  ret void
}
define void @$crunch.make_z(%$class.anon_make_z* %arg0) {

label1:
  %op2 = alloca i32
  store i32 0, i32* %op2
  %op3 = bitcast %$.list$prototype_type* %arg0 to %$union.len*
  %op4 = call i32 @$len(%$union.len* %op3)
  br label %label5

label5:                                                ; preds = %label1, %label14
  %op6 = phi i32 [ %op4, %label1 ], [ %op9, %label14 ]
  %op7 = icmp ne i32 %op4, %op6
  %op8 = getelementptr %$.list$prototype_type, %$.list$prototype_type* %arg0, i32 0, i32 4
  %op9 = add i32 %op6, 1
  %op10 = load %$union.conslist*, %$union.conslist** %op8
  %op11 = bitcast %$union.conslist* %op10 to i32*
  %op12 = getelementptr i32, i32* %op11, i32 %op6
  %op13 = load i32, i32* %op12
  br label %label14

label14:                                                ; preds = %label5
  br  i1 %op7, label %label5, label %label15

label15:                                                ; preds = %label14
  ret void
}
```
##### Function Call
与 Method Call 一致。

#### 0.1.3 Light IR Class

Class 定义同时继承 `Type` 和 `Value`。初始化默认有 typetag 作为第一个类元素，attribute 大小为第二个。默认只有一个 methods，即 `__init__`，如果是被继承关系，需要在 methods 算出是调用哪个最近的祖先定义。
##### Class 定义
先参考 Object Instantialization 定义。
$$
class(T)=\left(a_{1}=e_{1}, \ldots, a_{m}=e_{m}\right) \quad m \geq 1 \\
l_{a 1}, \ldots, l_{a m}= newloc (S, m) \\
v_{0}=T\left(a_{1}=l_{a i}, \ldots, a_{m}=l_{a m}\right) \\
G, G, S \vdash e_{1}: v_{1}, S,_{-} \\
G, G, S \vdash e_{2}: v_{2}, S,{ }_{-} \\
\vdots \\
G, G, S \vdash e_{m}: v_{m}, S,{ }_{-} \\
S_{1}=S\left[v_{1} / l_{a 1}\right] \ldots\left[v_{m} / l_{a m}\right] \\
l_{\text {init }}=l_{a i} \text{ such that } a_{i}=\_\_init\_\_\\
S_{1}\left(l_{\text {init }}\right)=\left(x_{0}, y_{1}=e_{1}^{\prime}, \ldots, y_{k}=e_{k}^{\prime}, b_{b o d y}, E_{f}\right) \quad k \geq 0 \\
l_{x 0}, l_{y 1}, \ldots, l_{y k}=n e w l o c\left(S_{1}, k+1\right) \\
E^{\prime}=E_{f}\left[l_{x 0} / x_{0}\right]\left[l_{y 1} / y_{1}\right] \ldots\left[l_{y k} / y_{k}\right] \\
G, E, S_{1} \vdash e_{1}^{\prime}: v_{1}^{\prime}, S_{1},_- \\
\vdots \\
G, E, S_{1} \vdash e_{k}^{\prime}: v_{k}^{\prime}, S_{1},_- \\
S_{2}=S_{1}\left[v_{0} / l_{x 0}\right]\left[v_{1}^{\prime} / l_{y 1}\right] \ldots\left[v_{k}^{\prime} / l_{y k}\right] \\
\frac{G, E^{\prime}, S_{2} \vdash b_{b o d y}:-, S_{3},, 
}{G, E, S \vdash T(): v_{0}, S_{3},{ }_{-}}$$

这个规则执行了以下操作，都需要在 `LightIR` 层生成。首先，通过为类$T$定义或继承的每个 Attribute 和 Method 分配位置，创建一个类$T$的新对象$v_{0}$。第二，使用全局环境对属性初始化器和方法定义进行评估；这一区别很重要，因为方法定义并没有捕捉到正在构建对象的环境$E$。第三，通过为$v_{0}$新分配的属性和方法的映射来修改当前的存储空间$S_{1}$，创建一个新的存储空间。最后，对象 $v_{0}$ 的 `__init__`.方法通过动态调度被调用。调用这个方法所需的步骤与一般的动态 Dispatch 相似，但例外的是 `__init__`.方法除了被调用的对象外不接受任何参数。

##### Method Invocation

$$S_{0}(E(f))=\left(x_{1}, \ldots, x_{n}, y_{1}=e_{1}^{\prime}, \ldots, y_{k}=e_{k}^{\prime}, b_{b o d y}, E_{f}\right)\\
n, k \geq 0\\
G, E, S_{0} \vdash e_{1}: v_{1}, S_{1},_{-}\\
\vdots\\
G, E, S_{n-1} \vdash e_{n}: v_{n}, S_{n},_-\\
l_{x 1}, \ldots, l_{x n}, l_{y 1}, \ldots, l_{y k}=n e w l o c\left(S_{n}, n+k\right)\\
E^{\prime}=E_{f}\left[l_{x 1} / x_{1}\right] \ldots\left[l_{x n} / x_{n}\right]\left[l_{y 1} / y_{1}\right] \ldots\left[l_{y k} / y_{k}\right]\\
G, E^{\prime}, S_{n} \vdash e_{1}^{\prime}: v_{1}^{\prime}, S_{n},_-\\
\vdots\\
G, E^{\prime}, S_{n} \vdash e_{k}^{\prime}: v_{k}^{\prime}, S_{n},_-\\
S_{n+1}=S_{n}\left[v_{1} / l_{x 1}\right] \ldots\left[v_{n} / l_{x n}\right]\left[v_{1}^{\prime} / l_{y 1}\right] \ldots\left[v_{k}^{\prime} / l_{y k}\right]\\
\frac{G, E^{\prime}, S_{n+1} \vdash b_{b o d y}:_-, S_{n+2}, R \\
R^{\prime}=\left\{\begin{array}{l}N o n e, \text { if } R \text { is } \\ R, \text { otherwise }\end{array} \quad\right. }{ G, E, S_{0} \vdash f\left(e_{1}, \ldots, e_{n}\right): R^{\prime}, S_{n+2},_-}$$

首先，函数的值从当前存储空间中获取。其次，函数调用的参数按从左到右的顺序被评估。然后，为函数的形式参数、局部变量和嵌套函数分配新的位置。一个新的环境$E^{\prime}$为函数调用创建，它将形式参数、局部变量和嵌套函数的名称映射到它们相应的位置。存储器$S_{n+1}$将这些位置分别映射到它们相应的参数、初始值和函数值。最后，用这个新环境$E^{prime}$和初始状态$S_{n+1}$来评估函数的主体。函数调用表达式被评估为由函数主体返回的值，如果函数主体被完全评估而没有遇到返回语句，则为None值。

##### Method Dispatched
$$G, E, S \vdash e_{0}: v_{0}, S_{0},- \\
v_{0}=X\left(a_{1}=l_{1}, \ldots, f=l_{f}, \ldots, a_{m}=l_{m}\right) \\
S_{0}\left(l_{f}\right)=\left(x_{0}, x_{1}, \ldots, x_{n}, y_{1}=e_{1}^{\prime}, \ldots, y_{k}=e_{k}^{\prime}, b_{\text {body }}, E_{f}\right) \\
n, k \geq 0 \\
G, E, S_{0} \vdash e_{1}: v_{1}, S_{1},- \\
\vdots \\
G, E, S_{n-1} \vdash e_{n}: v_{n}, S_{n},- \\
l_{x 1}, \ldots, l_{x n}, l_{y 1}, \ldots, l_{y k}=n e w l o c\left(S_{n}, n+k\right) \\
E^{\prime}=E_{f}\left[l_{x 0} / x_{0}\right] \ldots\left[l_{x n} / x_{n}\right]\left[l_{y 1} / y_{1}\right] \ldots\left[l_{y k} / y_{k}\right] \\
G, E^{\prime}, S_{n} \vdash e_{1}^{\prime}: v_{1}^{\prime}, S_{n},- \\
\vdots \\
G, E^{\prime}, S_{n} \vdash e_{k}^{\prime}: v_{k}^{\prime}, S_{n},- \\
S_{n+1}=S_{n}\left[v_{0} / l_{x 0}\right] \ldots\left[v_{n} / l_{x n}\right]\left[v_{1}^{\prime} / l_{y 1}\right] \ldots\left[v_{k}^{\prime} / l_{y k}\right] \\
G, E^{\prime}, S_{n+1} \vdash b_{\text {body }}:-, S_{n+2}, R \\
\frac{R^{\prime}=\left\{\begin{array}{l}\text { None, if } R \text { is } \\
R, \text { otherwise }\end{array}\right. }{ G, E, S \vdash e_{0} \cdot f\left(e_{1}, \ldots, e_{n}\right): R^{\prime}, S_{n+2},-}$$

Dispatch 需要先求解 obj expression，object是其第一个被传的参数。
### 0.2 Light IR stdlib 调用
此代码部分参考 [src/cgen/stdlib](../../src/cgen/stdlib) 目录下的代码。需要 call stdlib 中的函数，只需要 `create_call` 即可，注意需要传参。同学也可以自定义需要重复调用的 function 在 stdlib 中。

## 1. 实验要求

### 1.1 代码结构

详见[common/structure.md](./docs/common/structure.md)

本次实验使用了由C++编写的 LightIR 来生成 LLVM IR。

为了便于大家进行实验，该框架自动完成了语法树到 C++ 上的抽象语法树的转换。 我们可以使用访问者模式来设计抽象语法树中的算法。

![](../../PA3/lightwalker.png)

IRBuilder 提供函数的生成，例如 `builder->create_clloca()` 即可生成一条语句。`set_insert_point()` 用于切换插入代码位置。插入后自动算出 CFG 和 Use-Def List.

![](../../PA3/irbuilder.png)

在 `include/ir-optimizer/chocopy_optimization.hpp` 中，我还定义了一个用于存储作用域的类Scope。它的作用是辅助我们转换 `SymbolTable` 到 `Scope` ，管理不同作用域中的变量。它提供了以下接口：

```c++
// 进入一个新的作用域
void enter();
// 退出一个作用域
void exit();
// 往当前作用域插入新的名字->值映射
bool push(std::string name, Value *val);
// 根据名字，寻找到值
Value* find(std::string name);
// 根据名字，在 global 作用域寻找到值
Value* find_in_global(std::string name);
// 根据名字，在 nonlocal 作用域寻找到值，返回 load 好的值。
Value* find_in_nonlocal(std::string name, IRBuilder *builder);
// 判断当前是否在全局作用域内
bool in_global();
```

### 1.2 主要工作

1. 阅读[LightIR 核心类介绍](../common/LightIR.md)
2. 阅读[实验框架](#1-实验框架)，理解如何使用框架以及注意事项
3. 修改 [chocopy_lightir.cpp](../../src/ir-optimizer/chocopy_lightir.cpp) 来实现自动 IR 产生的算法，使得它能正确编译任何合法的 ChocoPy 程序
4. 在 `README.md` 中解释你们的设计，遇到的困难和解决方案

#### 1.2.1  几点说明
由于 LLVM IR 是强类型的，以及无法接收生成 fp action record 的参数，有很多部分和伯克利生成的代码不太一样

1. char* 还是 .string
2. activation record 不使用 frame pointer 回滚 function local 变量。
3. 几种 call convention，本 project 完全采用 c calling convention.
```bash
 *******************
 * CALL CONVENTION *
 *******************
NAME        CLEAN       PARA_DIRECTION
---------------------------------------------
cdecl       caller      <-
stdcall     callee      <-
pascal      callee      ->
fastcall    callee      2reg, <-
USING **pascal**
```
4. 为了适配范型，很多地方采用 union type 传参，llvm union 定义必须为 指针类型，所以我在 `$.list$prototype_type` 中存放的也是指针，而非 VLA(Variable length array).

### 1.3 Bonus

1. 完成基本Pass[60 pts]
2. 完成优化Pass[$\infty$ pts]

### 1.4 编译、运行和验证

* 编译

  若编译成功，则将在 `./[build_dir]/` 下生成 `ir-optimizer` 命令。

* 运行

  本次实验的 `ir-optimizer` 命令使用命令行参数来完成编译和运行。

  ```shell
  $ cd chocopy
  $ ./build/ir-optimizer test.py -run # 直接用clang编译器编译到elf给qemu执行。
  $ ./build/ir-optimizer test.py -emit # 输出。
  $ ./build/ir-optimizer test.py -assem # 输出汇编。
  $ ./build/ir-optimizer test.py -pass [PassName] # 运行pass。
  <以上的选项可以同时使用>
  ```

  通过灵活使用重定向，可以比较方便地完成各种各样的需求，请同学们务必掌握这个 shell 功能。

* 验证

  本次试验测试案例较多，为此我们将这些测试分为两类：

    1. sample: 这部分测试均比较简单且单纯，适合开发时调试。
    2. fuzz: 由fuzzer生成的正确的python文件，此项不予开源。
    3. student: 这部分由同学提供。

  我们使用python中的 `diffutil` 工具进行验证。将自己的生成结果和助教提供的 `xxx.typed.ast.result` 进行比较。

  ```shell
  $ python3 ./duipai.py --pa 3
  # 如果结果完全正确，则全 PASS，且有分数提示，一个正确的case 1 pts，此项评分按比例算入总评。选择chocopy的同学会在project部分分数上*1.2计入总评。
  # 如果有不一致，则会汇报具体哪个文件哪部分不一致，且有详细输出。
  ```

  **请注意助教提供的`testcase`并不能涵盖全部的测试情况，完成此部分仅能拿到基础分，请自行设计自己的`testcase`进行测试。**

### 1.5 评分

1. 基本测试样例[80 * 3/4 pts]
2. Fuzzer 测试[10 pts]
3. Student 测试[10 pts]
4. 提供TestCase[10 pts]
5. 报告[10 pts]
