# Programing Assignment II 文档

<!-- TOC -->

- [Programing Assignment II 文档](#programing-assignment-ii-文档)
  - [0. 基础知识](#0-基础知识)
    - [0.1 Declaration 检查](#01-declaration-检查)
      - [0.1.1 全局变量检查](#011-全局变量检查)
      - [0.1.2 for/while变量检查](#012-forwhile变量检查)
      - [0.1.3 函数内变量检查](#013-函数内变量检查)
      - [0.1.4 Class变量检查](#014-class变量检查)
      - [0.1.5 List变量检查](#015-list变量检查)
  - [0.2 Type Checker 检查](#02-type-checker-检查)
    - [0.2.1 类型语义](#021-类型语义)
    - [0.2.2 类型推导](#022-类型推导)
  - [0.3 错误检测](#03-错误检测)
    - [0.3.1 语义检测](#031-语义检测)
    - [0.3.3 类型检测](#033-类型检测)
  - [1. 实验要求](#1-实验要求)
    - [1.0.1 主要工作](#101-主要工作)
    - [1.0.2 提示](#102-提示)
      - [1.0.2.1 注意两者的区别](#1021-注意两者的区别)
      - [1.0.2.2 推荐流程](#1022-推荐流程)
    - [1.1 代码结构](#11-代码结构)
      - [1.1.1 `SymbolTable`](#111-symboltable)
      - [1.1.2 `SymbolType` 和 `ValueType`](#112-symboltype-和-valuetype)
    - [1.2 Bonus](#12-bonus)
    - [1.3 编译、运行和验证](#13-编译运行和验证)
    - [1.4 WriteUp](#14-writeup)
      - [1.4.1 Compilation error](#141-compilation-error)
      - [1.4.2 Neat Code](#142-neat-code)
      - [1.4.3 Design pattern](#143-design-pattern)
      - [1.4.4 Memory Control](#144-memory-control)
      - [1.4.5 Misc and another part if needed.](#145-misc-and-another-part-if-needed)
    - [1.5 提供可用的测试用例](#15-提供可用的测试用例)
    - [1.6 评分](#16-评分)

<!-- /TOC -->

本次实验是组队实验，请仔细阅读组队要求，并合理进行分工合作。本实验中需要使用`Visitor Pattern`完成对程序的 Declaration Analysis 和 Type Checker Analysis。Declaration的结果以 Symbol table 的形式传给Type Checker继续检查。从而使 `Chocopy` 的LSP没有语义错误。

注意：组队实验意味着合作，但是小组间的交流是受限的，且严格**禁止**代码的共享。除此之外，如果小组和其它组进行了交流，必须在根目录 `README.md`中记录下来交流的小组和你们之间交流内容。同时，需要在此文件中更新两位同学的邮箱和代码WriteUp，WriteUp要求详见[1.4](#14-writeup)。

## 0. 基础知识

### 0.1 Declaration 检查

`DeclarationAnalyzer` 检查是一个申明作用域环境的检查，在python中的变量有四个作用域

| 作用域            | 英文解释                      | 英文简写 |
|----------------|---------------------------|------|
| 局部作用域（函数内）     | Local(function)           | L    |
| 外部嵌套函数作用域      | Enclosing function locals | E    |
| 函数定义所在模块作用域    | Global(module)            | G    |
| python内置模块的作用域 | Builtin                   | B    |

在访问变量时，先查找本地变量，然后是包裹此函数外部的函数内的变量，之后是全局变量 最后是內建作用域内的变量 即： L –> E -> G -> B

```python
class Real(object):
    value: int = 0
    def __init__(self: "Object", val: int) -> object:
        self.value = val
    def __add__(self: "Object", other: "Object") -> Real:
        return Real(self.value + other.value)

x: int = 0
y: int = 1

def P() -> int:
    x: bool = True
    a: bool = False

    def Q() -> Real:
        x: Real = None
        x = Real(1)
        y: Real = None
        y = Real(1)
        return x + y

    return Q().value
```

<img src="./symbol_table.png" alt="symbol_table" style="zoom:33%;" />

#### 0.1.1 全局变量检查

函数内定义的变量，在函数内部可以访问，但是不能访问函数外部的变量。global 关键字可以用来声明一个全局变量。

```python
x: int = 0

def f():
    x = 1  # 需要在内部声明 global x 才能获得外部变量。
    print(x)
```

#### 0.1.2 for/while变量检查

对于for/while循环，在循环体内部定义的变量，在循环体内部可以访问，但是不能访问循环体外部的变量。in 关键字可以用来声明一个循环变量。in 前的循环变量可以与符号表的变量重名，在循环内是c++ rewrite的语义。

```python
def baz(self: "bar", xx: [int]) -> str:
    global count
    x: int = 0
    y: int = 1

    def qux(y: int) -> object:
        nonlocal x
        if x > y:
            x = -1

    for x in xx:  # 此处的 x 在循环内部对外部变量无效
        self.p = x == 2
    qux(0)  # Yay! ChocoPy
    count = count + 1
    while x <= 0:
        if self.p:
            xx[0] = xx[1]
            self.p = not self.p
            x = x + 1
        elif foo("Long"[0]) == 1:
            self.p = self is None
    return "Nope"
```

#### 0.1.3 函数内变量检查

Python 支持嵌套定义函数，每次进入函数时需要进入函数本地变量的 scope，同时对外部的 E/G/B 所在定义 aware，不能重名，如有调用需要指向外部的 symbol。

```python
x: int = 0

def crunch(zz: [[int]]) -> object:
    z: [int] = None
    global x

    def make_z() -> object:
        # 需要添加 nonlocal z 若没有此行需要报错
        for z in zz:  # 嵌套函数 z为左值未定义，zz为调用右值，直接访问
            pass  # Set z to last element in zz

    make_z()
    for x in z:
        pass  # Set x to last element in z


crunch([[1, 2], [2, 3], [4, 5], [6, 7]])
print(x)  # 嵌套内部的变量修改在外部能看到 类似 c++ [&] lambda Semantic
```

#### 0.1.4 Class变量检查

需要先定义Class，后声明，再定义变量。

```python
class animal(object):
    makes_noise: bool = False

    def make_noise(self: "animal") -> object:
        if (self.makes_noise):
            print(self.sound())

    def sound(self: "animal") -> str:
        return "???"


class cow(animal):
    def __init__(self: "cow"):
        self.makes_noise = True

    def sound(self: "cow") -> str:
        return "moo"


c: animal = None
c = cow()
c.make_noise()  # Prints "moo"
```

Predefined classes 有 object, int, bool, str, list. 都有 `__init__` 方法，以及attribute。自定义 class 可以自定义 `__init__` 方法，继承会先放入被继承 class 的 attribute 和 methods，再放入自己的。

```python
class A(object):
    a: int = 42

    def foo(self: "A", ignore: object) -> int:
        return self.a

    def bar(self: "A") -> int:
        print("A")
        return 0


class B(A):
    b: bool = True

    def __init__(self: "B"):
        print("B")

    def bar(self: "B") -> int:
        print("B")
        return 0


class C(B):
    c: bool = True

    def __init__(self: "C"):
        print("C")

    def foo1(self: "C") -> int:
        print("B")
        return 0

    def bar(self: "C") -> int:
        print("C")
        return 0


def t():
    def f():
        return 0

    return 0


d: str = input()
a: A = None
if d == "sb":
    a = C()
else:
    a = A()

print(a.bar())
```

声明时可以声明 father class，动态决定实际 class。这时就需要动态 typeclass 来得到 methods。

#### 0.1.5 List变量检查

List 实现是一个 class，所以定义的时候和 class 实例化一样。初始化会调用 `conslist()`，在 riscv 汇编中是放在 __len__ 后面的一个 attribute，是一个 list/bool/int/str/object/classes 的数组。

```bash
.globl $.list$prototype
$.list$prototype:
  .word -1                                 # Type tag for class: .list
  .word 4                                  # Object size
  .word 0                                  # Pointer to dispatch table
  .word 0                                  # Initial value of attribute: __len__
  .align 2
```

声明如下：

```python
x: [[bool]] = None
y: str = "Hello"
z: [int] = None
i: int = 0

x: [[bool]] = [[True, False], [False, True]]
z = [1, 2, 3]

for i in z:
    print(i)

while i < len(z):
    print(z[i][1])
    print(y[i])
    i = i + 1
```

List 和 str 都有 `len()` method 获得长度，都可以用 index access 获取元素，for 语句相当于 `while i<len(*)`.

## 0.2 Type Checker 检查

此项实现在 `TypeChecker` 类中。

### 0.2.1 类型语义

每个类型都有 type tag，预定义如下。

```cpp
enum type { LIST=-1, OBJECT, INT, BOOL, STRING, CLASS };

constexpr bool is_list_type() const { return tid_ == type::LIST; }
constexpr bool is_void_type() const { return tid_ == type::VOID; }
constexpr bool is_integer_type() const { return tid_ == type::INT; }
constexpr bool is_bool_type() const { return tid_ == type::BOOL; }
constexpr bool is_string_type() const { return tid_ == type::STRING;}
constexpr bool is_value_type() const { return is_bool_type() ||  
             is_string_type() || is_integer_type(); }
constexpr bool is_class_type() const { return tid_ >= type::CLASS; }
```

int/bool/str 不能被 inherit，所有定义的 class 都是继承 object。同时有两个辅助类型 `<None>`,`<Empty>`。type tag 会在刚进入 `TypeChecker` 时算出来，因为此时 `DeclarationAnalyzer` 把 SymbolTable 已经求出来了。

```cpp
/** set up default class hierarchy
 * <None> <= object
 * <Empty> <= object
 * <None> <= <None>
 * <Empty> <= <Empty>
 */
map<string, string> super_classes = {{"int", "object"},  {"bool", "int"},  
                                     {"none", "object"},  {"empty", "object"},
                                     {"<None>", "object"},  {"<Empty>", "object"}};
```

以下是基本的类型传播：

1. $T_{1} \leq T_{2}$ (i.e., ordinary subtyping)
2. $T_{1}$ is <None $>$ and $T_{2}$ is not int, bool, or str.
3. $T_{2}$ is a list type $[T]$ and $T_{1}$ is <Empty>.
4. $T_{2}$ is a list type $[\mathrm{T}]$ and $T_{1}$ is $[\langle$ None $\rangle]$, where $\langle$ None $\rangle
   \leq_ a T$

最后两项的定义是为了防止 x:[A]= [None, None]，x:[ [A ] ]=[ [None ] ] 这两种情况，同时 List 若内部的 type 不一样是无法传播的。

对于其他 op 的类型传播可以用 Least Common Ancestor 来刻画。

1. 如果$A\leq_{a} B$，那么$A\sqcup B=B\sqcup A=B$
2. 否则，$A\sqcup B$只是$A$和$B$在由$\leq$定义的树状类型层次中的最小共同祖先。

### 0.2.2 类型推导

此部分详见[ChocoPy Language Reference](../chocopy_language_reference.pdf)的 Section 5. 首先需要定义现在所在的 Type Environment，由一个四元组定义$<O,M,C,R>$,定义了 `TypeChecker` 的检查环境，$O$由全局 `SymbolTable` 决定，$M$由 `ClassDefType` 决定，$C$由局部 `SymbolTable`决定，$R$由 `FunctionDefType` 决定。以 `is` 关键词为例。

$$O, M, C, R \vdash e_{1}: T_{1}\\ O, M, C, R \vdash e_{2}: T_{2}\\ \frac{T_{1}, T_{2} \text { are not one of } i n t, s t r, \text { bool }}{O, M, C, R \vdash e_{1} \text { is } e_{2}: \text { bool }}$$

规则需要写在 `BinaryExpr` 中，若 `operator` 是 `is`， 判断是否存在 subclass 关系，判断错误，返回 bool。

另一个 `IfStmt` 的例子

$$O, M, C, R \vdash e_{0}: bool\\ O, M, C, R \vdash b_{0}\\ O, M, C, R \vdash e_{1}: bool\\ O, M, C, R \vdash b_{1}\\
\vdots\\ O, M, C, R \vdash e_{n}: bool\\ O, M, C, R \vdash b_{n}\\ n \geq 0\\ O, M, C, R \vdash b_{n+1}\\ \overline{O,
M, C, R \vdash \text { if } e_{0}: b_{0} \text { elif } e_{1}: b_{1} \ldots \text { elif } e_{n}: b_{n} \text { else }:
b_{n+1}}$$

需要检查所有 condition return type 是不是 bool，同时对于定义有最小公公祖先的关系。

对于 Function Invoke 的例子，所有 argument 都是定义的 subclass。

$$O, M, C, R \vdash e_{1}: T_{1}^{\prime \prime}\\ O, M, C, R \vdash e_{2}: T_{2}^{\prime \prime}\\ \vdots\\ O, M, C, R
\vdash e_{n}: T_{n}^{\prime \prime}\\ n \geq 0\\ O(f)=\left\{T_{1} \times \cdots \times T_{n} \rightarrow T_{0} ; x_{1},
\ldots, x_{n} ; v_{1}: T_{1}^{\prime}, \ldots, v_{m}: T_{m}^{\prime}\right\}\\ \forall 1<i<n: T_{i}^{\prime \prime}<_{q}
T_{i}\\ \overline{O, M, C, R \vdash f\left(e_{1}, e_{2}, \ldots, e_{n}\right): T_{0}}$$

其他规则请同学自行查阅并实现。

## 0.3 错误检测

所有官方需要报的错误在[pa2](../../tests/pa2/sample)下以**bad**打头。

1. [bad_duplicate_global.py](../../tests/pa2/sample/bad_duplicate_global.py)/[bad_duplicate_local.py](../../tests/pa2/sample/bad_duplicate_local.py) 需要给出 Duplicate declaration of identifier in
   same scope。
2. [bad_expr_binary.py](../../tests/pa2/sample/bad_expr_binary.py)/[bad_expr_unary.py](../../tests/pa2/sample/bad_expr_unary.py) 需要给出 BinaryExpr/UnaryExpr 的 type 不可cast。
3. [bad_func_def_return.py](../../tests/pa2/sample/bad_func_def_return.py)/[bad_func_def_call.py](../../tests/pa2/sample/bad_func_def_call.py)/[bad_return_missing.py](../../tests/pa2/sample/bad_return_missing.py)/[bad_return_top.py](../../tests/pa2/sample/bad_return_top.py) 需要给出 Function 的 type checking 未过。
4. [bad_list_assign.py](../../tests/pa2/sample/bad_list_assign.py)/[bad_list_index.py](../../tests/pa2/sample/bad_list_index.py) 需要给出 List 的 type checking 未过。
5. [bad_nonlocal_global.py](../../tests/pa2/sample/bad_nonlocal_global.py)/[bad_shadow_local.py](../../tests/pa2/sample/bad_shadow_local.py)/[bad_shadow_local_2.py](../../tests/pa2/sample/bad_shadow_local_2.py) 需要给出 Scope 未过。
6. [bad_*_assign.py](../../tests/pa2/sample/bad_*_assign.py)/[bad_type_*.py](../../tests/pa2/sample/bad_type_*.py) 需要给出 Assign Variable 未过。

语义分析阶段会检测两种类型的错误：语义错误和类型检查错误。语义错误是对上述所列语义规则的违反。类型检查错误是对ChocoPy语言参考手册中所列类型规则的违反。如果输入的程序包含语义错误，那么语义分析阶段的输出预计将是一个语义错误信息的列表，以及它们的来源位置。只有在没有语义错误的情况下才会报告类型检查错误。如所述，类型检查错误与类型化的AST一起被在线报告。

### 0.3.1 语义检测

您的实现应当能够从语义错误恢复，并继续分析程序的其余部分，以便尽可能多地报告语义错误。与从解析错误中恢复不同，语义分析中的错误恢复执行起来要简单得多，因为你可以简单地报告一个错误并继续分析AST的其余部分。

autograder将使用以下规则来评估包含语义错误的输入上的实现：只有当参考实现所报告的所有语义错误也被您提交的实现所报告时，测试才能通过。也即是参考实现所报告的语义错误应该是实现所报告错误的一个子集。

### 0.3.3 类型检测

只有在输入程序中不存在语义错误的情况下，才会报告类型检查错误。在没有语义错误的情况下，语义分析阶段应该输出一个JSON格式的类型化AST，而不管程序是否有良好的类型化。如果程序中的一个AST节点未能进行类型检查，那么就会在相应Node的typeError属性中插入一条错误信息。让我们把这样的节点称为
`ill-typed` 节点。

1. `-1` 可以被看成 `UnaryExpr` 或 int 类型。
2. 当对变量、属性和列表元素的赋值表达式进行类型检查时，在一个类型良好的表达式中推断出的类型通常是右值类型。然而，如果右边的表达式不符合被赋值的位置的类型，那么分析应该插入一个错误信息，并代替推断赋值左边的类型。当然，如果左侧不是一个有效的变量、属性或列表元素，那么分析必须求助于推断整个赋值表达式的对象。
3. 在对二进制运算符$+$进行类型检查时，出现了一个模糊不清的问题，因为一个类型良好的$+$表达式的推断类型是不同的，取决于它的两个操作数是int类型、str类型还是list类型。这个经验法则并没有为一个操作数是int而另一个操作数是str的情况提供唯一的解决方案。分析应该以下面的方式处理类型不良的$+$表达式：如果至少一个操作数是int类型，那么推断为int；否则，推断为object。在这两种情况下，必须在类型错误的表达式处插入一个适当的错误信息。

## 1. 实验要求

本实验的输出可以实现对语义检查的要求，建立在语法没有错误的基础上，同样可以输出高亮在IDE中。

本次实验需要各位同学根据`ChocoPy`的语义补全[chocopy_semant.cpp](./src/semantic/chocopy_semant.cpp)
文件，完成完整的语法分析器，能够输出识别出语义错误的位置。

### 1.0.1 主要工作

1. 了解 `visitor` 模式基础知识和理解 ChocoPy 语义（重在了解如何在 `visitor` 模式下）
2. 阅读 `./src/semantic/chocopy_ast.cpp`以及 `./src/semantic/chocopy_semant.cpp`（重在理解分析树的使用）
3. 了解 `./src/semantic/chocopy_semant.cpp` 中的语义检查，并完成语义检查
4. 补全 `./src/semantic/chocopy_semant.cpp` 文件，可以添加其他 `Analyzer` ，以及如果你需要其他改写的代码可以自行改写。
5. 在 `README.md` 中解释你们的设计，遇到的困难和解决方案

### 1.0.2 提示

文本输入：

```c
a: int = 1
```

则对应语义检查结果应为：

```json
{
  "kind" : "Program",
  "location" : [ 1, 1, 1, 11 ],
  "declarations" : [ {
    "kind" : "VarDef",
    "location" : [ 1, 1, 1, 10 ],
    "var" : {
      "kind" : "TypedVar",
      "location" : [ 1, 1, 1, 6 ],
      "identifier" : {
        "kind" : "Identifier",
        "location" : [ 1, 1, 1, 1 ],
        "name" : "a"
      },
      "type" : {
        "kind" : "ClassType",
        "location" : [ 1, 4, 1, 6 ],
        "className" : "int"
      }
    },
    "value" : {
      "kind" : "IntegerLiteral",
      "location" : [ 1, 10, 1, 10 ],
      "value" : 1
    }
  } ],
  "statements" : [ ],
  "errors" : {
    "errors" : [ ],
    "kind" : "Errors",
    "location" : [ 0, 0, 0, 0 ]
  }
}
```

对语义的抽象语法树输出文件如下，注意Type Error与Declaration Error输出位置的区别。

```json
{
  "kind" : "Program",
  "location" : [ 1, 1, 1, 11 ],
  "declarations" : [ {
    "kind" : "VarDef",
    "location" : [ 1, 1, 1, 10 ],
    "var" : {
      "kind" : "TypedVar",
      "location" : [ 1, 1, 1, 6 ],
      "identifier" : {
        "kind" : "Identifier",
        "location" : [ 1, 1, 1, 1 ],
        "name" : "a"
      },
      "type" : {
        "kind" : "ClassType",
        "location" : [ 1, 4, 1, 6 ],
        "className" : "int"
      }
    },
    "value" : {
      "kind" : "IntegerLiteral",
      "location" : [ 1, 10, 1, 10 ],
      "inferredType" : {
        "kind" : "ClassValueType",
        "className" : "int"
      },
      "value" : 1
    }
  } ],
  "statements" : [ ],
  "errors" : {
    "errors" : [ ],
    "kind" : "Errors",
    "location" : [ 0, 0, 0, 0 ]
  }
}
```

#### 1.0.2.1 注意两者的区别

1. 添加了新的对象类型**ValueType**、**ClassValueType**、**ListValueType**。这些将被用来存储类型检查后推断出的程序表达式的类型信息。注意，这些类型几乎与**TypeAnnotation**
   和它的两个子类型完全相似。
   ![extend_node](./extend_node.png)
   **TypeAnnotation**和**ValueType**之间的区别在于，后者没有扩展Node；因此，ValueType对象没有位置属性。这应该是有道理的，因为在语义分析期间分配的类型实际上并不存在于源代码中。
   2.
   类型Expr有一个新的属性：inferredType，它可以是null。在分析器产生的AST中，这个属性对每个表达式都是空的。语义分析为每个可以求值的程序表达式推断类型。具体来说，推断类型（inferredType）属性只对以下情况保持空。
    1. 直接出现在**FuncDef**、 **ClassDef**、 **TypedVar**、 **GlobalDecl**、 **NonlocalDecl**、 **VarAssignExpr**、  **VarAssignStmt** 、 **MemberExpr**、 **ForStmt **或 **CallExpr** 属性中的标识符对象
    2. 紧接着 **MethodCallExpr**、 **MemberAssignExpr** 或 **MemberAssignStmt** 中包含的 **MemberExpr**
    3. 紧接着 **IndexAssignExpr** 或 **IndexAssignStmt** 中包含的 **IndexExpr**。注意，空值属性在JSON表示中被简单省略是可以接受的。
3. 如果有语法错误，对应的errors为`CompilerError`且`syntax=true`。对语法接收的errors体现为`Error`，检测程序不会检查错误信息与个数，所以不需要考虑错误的贪心信息，可以找到一个不可接收的程序直接报错返回。
    1. Node种类有一个新的属性：**typeError**。在解析器产生的AST中，这个属性对每个节点都是空的。如果在对一个节点进行类型检查时出现了错误，那么该节点的**typeError**
       将为非空。对于一个类型良好的ChocoPy程序来说，在语义分析阶段的输出中，每个节点的**typeError**属性都将为空。注意，空值属性在JSON表示中被简单省略是可以接受的。
     2. 除了**SyntaxError**之外，还增加了一个新的**SemanticError**种类。如果输入的AST对应于含有语义错误的ChocoPy程序，那么语义分析阶段的输出应当是一个**Errors**类型的JSON对象，其中将包含一个或多个**SemanticError**类型的节点。请注意，**SemanticError**（语义错误）对象确实继承了一个locations（位置）属性。该属性将包含与分析导致错误的AST节点相对应的源代码位置。

#### 1.0.2.2 推荐流程

这项任务可能比上一项任务大得多。然而，这项任务让你的设计决策提供了更多的空间，使其能够采取灵活的实施策略。我已经在启动代码中以 skeleton 实现的形式提供了一些指导，以下是流程建议。然而，你最终会做到哪一步，主要取决于你自己。

1. Tree Traversal 写完基本的 `visit()`。
2. Type Hierarchy 把符号表写完。
3. Type Checking 实现所有 type 检查。

**具体的需识别的语法推导错误参考[所有错误](#03-错误检测)，需要实现的抽象语法树参考[chocopy_ast.hpp](../../include/parser/chocopy_ast.hpp)**

> 注意，你所需修改的文件应仅有[chocopy_semant.cpp](../../src/semantic/chocopy_semant.cpp)和[chocopy_parse.cpp](../../src/parser/chocopy_parse.cpp)，后者用于修改输出，如果发现其他bug，请开分支只commit你认为的bug并提交PR。关于`visitor pattern`用法已经在[visitor.md](./visitor.md)中进行简短的介绍，更高阶的用法请参考谷歌和StackOverflow。

### 1.1 代码结构

详见[common/structure.md](./doc/common/structure.md)

<img src="./visitor_graph.png" alt="visitor_image" style="zoom:33%;" />

#### 1.1.1 `SymbolTable`
```cpp
class SymbolTable {
public:
    /** A table representing a region nested in that represented by PARENT. */
    explicit SymbolTable(SymbolTable *parent0) { this->parent = (SymbolTable *)parent0; }
    SymbolTable() { this->parent = nullptr; }
    /** Returns the mapping in this scope or in the parent scope using a recursive traversal */
    template <typename T> T get(const string &name);
    /** Adds a new mapping in the current scope, possibly shadowing mappings in the parent scope. */
    template <typename T> SymbolTable *put(const string &name, T value);
    map<string, SymbolType *> *tab = new map<string, SymbolType *>();
    SymbolTable *parent;
    map<string, int> class_tag_;
}
```
通过一个 Map 存名称到 Type的映射，不同的 class 有不同的 tag，由 parent 形成树状结构维护，可以通过 `get<T>` 和 `put<T>` 方法获取和添加符号表中的符号。`debug_sym()` 和 `debug_nested_func()` 可以遍历最上层符号表获得所有信息。 `template <class Key, class T> set<string> key_to_set(const std::map<Key, T> &map, std::set<Key> &set);` 可以把 `Analyzer` 类中的 set 变成 map。

#### 1.1.2 `SymbolType` 和 `ValueType`
```cpp
class SymbolType {
public:
    virtual constexpr bool is_value_type() { return false; }
    virtual constexpr bool is_list_type() { return false; }
    virtual constexpr bool is_func_type() { return false; }
    virtual constexpr int *is_none_type() { return new int[4]; }
    virtual constexpr bool is_special_type() { return false; }

    virtual string get_name() = 0;
    virtual void set_name(string_view className) = 0;
    virtual string get_type() const = 0;

    template <typename _Ty> bool eq(const _Ty &_Value);
    template <typename _Ty> bool neq(const _Ty &_Value);
};
```
`SymbolType` 是一个纯虚类，用于标记所有 type，实现了 `eq` 和 `neq`。`ValueType` 和 `FunctionDefType` 都继承自 `SymbolType`。

```cpp
class ValueType : public SymbolType {
public:
    ValueType() = default;
    bool is_value_type() override { return true; }
    static ValueType *annotate_to_val(parser::TypeAnnotation *annotation);
    string get_name() override;
};
```

`ClassValueType` 和 `ListValueType` 都继承自 `ValueType`。可以通过 `is_*_type()` 来判断类型，`annotate_to_val()` 从 `TypeAnnotation` 转换到 `Type`.

### 1.2 Bonus

在正确作出所有给出语法推导的情况下报出所有给出案例的错误[10pts]

### 1.3 编译、运行和验证

* 编译

  若编译成功，则将在 `./[build_dir]/` 下生成 `semantic` 命令。

* 运行

  本次实验的 `semantic` 命令使用 shell 的输入重定向功能，即程序本身使用标准输入输出（stdin 和 stdout），但在 shell 运行命令时可以使用 `<` `>` 和 `>>` 灵活地自定义输出和输入从哪里来。

  ```shell
  $ cd chocopy
  $ ./build/semantic               # 交互式使用（不进行输入重定向）
  <在这里输入 ChocoPy代码，如果遇到了错误，将程序将报错并退出。>
  <输入完成后按 ^D 结束输入，此时程序将输出解析json。>
  $ ./build/semantic < test.py # 重定向标准输入
  <此时程序从 test.py 文件中读取输入，因此不需要输入任何内容。>
  <如果遇到了错误，将程序将报错并输出错误json；否则，将输出解析json。>
  $ ./build/semantic test.py  # 不使用重定向，直接从 test.py 中读入
  $ ./build/semantic < test.py > out
  <此时程序从 test.py 文件中读取输入，因此不需要输入任何内容。>
  ```

  通过灵活使用重定向，可以比较方便地完成各种各样的需求，请同学们务必掌握这个 shell 功能。

* 验证

  本次试验测试案例较多，为此我们将这些测试分为两类：

    1. sample: 这部分测试均比较简单且单纯，适合开发时调试。
    2. fuzz: 由fuzzer生成的正确的python文件，此项不予开源。
    3. student: 这部分由同学提供。

  我们使用python中的 `json.load()` 命令进行验证。将自己的生成结果和助教提供的 `xxx.typed.ast` 进行比较，location部分可以有1-2位置误差。

  ```shell
  $ python3 ./duipai.py --pa 2
  # 如果结果完全正确，则全 PASS，且有分数提示，一个正确的case 1 pts，此项评分按比例算入总评。选择chocopy的同学会在project部分分数上*1.2计入总评。
  # 如果有不一致，则会汇报具体哪个文件哪部分不一致，且有详细输出。
  ```

  **请注意助教提供的`testcase`并不能涵盖全部的测试情况，完成此部分仅能拿到基础分，请自行设计自己的`testcase`进行测试。**

### 1.4 WriteUp

小组需要提供design doc for WriteUp，需要提供以下部分。

#### 1.4.1 Compilation error
in tag xxx, how you fix the compilation error, using gdb, Valgrind, or by other people.

#### 1.4.2 Neat Code
in the final tag, how do you change the pointer to a smart pointer to save the residual code.

#### 1.4.3 Design pattern
What design pattern do you utilize when you organize your type semantics?

#### 1.4.4 Memory Control
How do you manage your memory?

#### 1.4.5 Misc and another part if needed.
You can simply answer my questions asked when committing you submit here.

### 1.5 提供可用的测试用例
对于每组，需要在资源库的根目录下创建一个名为 `tests/pa2/student/` 的文件夹，并放置20个有意义的 `*.py` 测试案例，其中10个将通过所有的编译，另外10个将不通过编译，但测试你代码的错误报告系统。请注意，你的测试案例将被用来评估所有4个项目中其他人的代码，所以要有耐心，并对你的同学狠一点。你的最终成绩将在所有学生都提交了测试案例后重新计算。这一部分占项目部分的 [6 pts] ，但你可以降低其他学生的成绩。

### 1.6 评分

1. 基本测试样例[55pts]
2. Fuzzer 测试[10pts]
3. Student 测试[20pts]
4. 提供TestCase[5pts]
5. 报告[10pts]
