# Programing Assignment III Bonus 实验文档

- [Programing Assignment III Bonus 实验文档](#programing-assignment-iii-bonus-实验文档)
  - [0. 前言](#0-前言)
    - [主要工作](#主要工作)
      - [代码与材料阅读](#代码与材料阅读)
      - [开发基本优化Pass](#开发基本优化pass)
      - [Bonus：选做优化Pass](#bonus选做优化pass)
      - [PAIII Bonus代码与实验报告提交](#paiii-bonus代码与实验报告提交)
  - [1. 实验框架](#1-实验框架)
  - [2. 运行与调试](#2-运行与调试)
    - [运行 chocopy](#运行-chocopy)
    - [自动测试](#自动测试)
    - [logging](#logging)
    - [建议](#建议)
  - [3. 提交要求](#3-提交要求)
    - [目录结构](#目录结构)
    - [提交要求和评分标准](#提交要求和评分标准)

## 0. 前言

本次实验是组队实验，请仔细阅读组队要求，并合理进行分工合作。

**Pass概念**：接受一个`module`作为参数，经过 PA III 大家可以知道，`module`是IR最上层的结构，而`Pass`则是遍历`module`内的结构，分析出信息(例如对活跃变量的分析 `Pass`)，或者是对`module`内的指令和bb做一些变换(例如本次实验中的常量传播和循环不变式外提 `Pass`)。

经过 PA III，相信大家已经掌握了 LightIR 的结构，并且对于 LLVM IR 也有了更深的理解。在本次实验中，我们要在理解SSA（静态单赋值）格式的基础上，实现三个简单的块内优化Pass与分析Pass：常量传播，循环不变式外提，活跃变量分析。

值得一提的是`LightIR`中的[LightIR核心类介绍.md](../common/LightIR.md)中的User类中`operands_`成员也就是操作数列表，以及Value类的`use_list_`成员，这两个链表描述了指令间的数据依赖关系，请注意查看。

### 主要工作

#### 代码与材料阅读
具体要求与说明参考：[reading.md](./reading.md)

1. 阅读 `Compiler_USTC.pdf` 中优化部分，尤其是静态单赋值和活跃变量部分。
2. 阅读`Mem2Reg`与`LoopSearch`两个优化Pass的代码，能够描述优化的基本流程，并且回答思考题。
3. 通过阅读代码，掌握如何开发基于LightIR的优化Pass

#### 开发基本优化Pass

1. **常量传播**
   能够实现在编译优化阶段，能够计算出结果的变量，就直接替换为常量；补充以下几点需要注意的地方：
    a. 只需要考虑过程内的常量传播，可以不用考虑数组，**全局变量只需要考虑块内的常量传播**，这里举个例子来说明常量传播：
    ```cpp
    %a = 1 + 1；
    %b = %a + %c;
    ```
    那么首先我们可以将`%a=1+1`折叠成`%a=2`，然后我们发现`%b=%a+%c`这条指令用到了`%a`，那么我们就可以将`%b=%a+%c`中的`%a`直接替换成常量2，代码转化为：
    ```cpp
    %a = 2;
    %b = 2 + %c;
    ```
    当然本次实验还需要额外做一步：将`%a=2`这条无用语句删掉，因为`%a`是常量，并且已经传播给了使用它的地方，那么这条赋值语句就可以删掉了（由于我们的IR是SSA形式，所以不用担心%a被重新赋值）：
    ```cpp
    %b=2+%c;
    ```
    b. 整形浮点型都需要考虑，
    d. 对于`a=1/0`的情形，可以不考虑，即可以做处理也可以不处理。
    c. 做到删除无用的分支将条件跳转变换为强制性跳转，比如下面的语句中，else部分就可以删除
   
    ```cpp
    int a;
    a=1;
    if(a){...}
    else{...}
    ```
    同时对于分支嵌套的情况都能够删除掉无用的分支，这一步之后对于可能出现的无法到达的块都需要进行删除，而至于只含有跳转指令的冗余块在本次实验中不要求实现。
   
2. **循环不变式外提**
    要能够实现将与循环无关的表达式提取到循环的外面。不用考虑数组，与全局变量。举个例子：
    
    ```cpp
    while(i<10){
      while(j<10){
        a=i+1;
        j=j+1;
      }
      i=i+1;
    }
    ```
    那么表达式`a=i+1`与内部循环无关，可以提到j循环之外：
    ```cpp
    while(i<10){
      a=i+1;
      while(j<10){
        j=j+1;
      }
      i=i+1;
    }
    ```
    下面给出一些循环外提的tips：
    a. 思考如何判断语句与循环无关，且外提没有副作用
    b. 循环的条件块（就是在LoopSearch中找到的Base块）最多只有两个前驱，思考下，不变式应该外提到哪一个前驱。
    
3. **活跃变量分析**

   能够实现分析bb块的入口和出口的活跃变量，参考资料见附件(紫书9.2.4节)，在`ActiveVars.hpp`中定义了两个成员`live_in`, `live_out`，你需要将`pair<bb, IN[bb]>`插入`live_in`的map 结构中，将`pair<bb, OUT[bb]>`插入`live_out` 的map 结构中，并调用ActiveVars类中的print()方法输出bb活跃变量情况到json文件，助教会根据你输出的json文件进行批改。(为了保证输出变量名字的一致性，请不要对指令，bb等进行命名操作)
   
   **提示**：材料中没有phi节点的设计，因此数据流方程：$OUT[B] =\cup_{s是B的后继}IN[S]$ 的定义蕴含着S入口处活跃的变量在它所有前驱的出口处都是活跃的，由于`phi`指令的特殊性，例如`%0 = phi [%op1, %bb1], [%op2, %bb2]`如果使用如上数据流方程，则默认此`phi`指令同时产生了`op1`与`op2`的活跃性，而事实上只有控制流从`%bb1`传过来才有`%op1`的活跃性，从`%bb2`传过来才有`%op2`的活跃性。因此对此数据流方程需要做一些修改。
   
4. ...


#### Bonus：选做优化Pass
以下优化可以参考[毕业设计](https://github.com/alicemare/learn_compiler)，不做强制要求，可以自由发挥，但是需要在报告中说明达到的效果，并在线下验收时展示，选做Pass 在`2022/06/11`之前都可以联系助教验收，然后由助教统一安排线下验收。
1. 标记式死代码删除(支持过程间分析)
3. 函数内联
4. 块间公共子表达式删除
5. 尾递归消除
6. 多线程
7. 向量化
   - 参考 clang `clang -O3 -mllvm -riscv-v-vector-bits-min=256 -save-temps -Wno-unused-command-line-argument --target=riscv64-unknown-elf -march=rv32gcv0p10 -menable-experimental-extensions -mabi=lp32d` 的运行结果对 `vectorize.py` 和 `vectorize1.py` 进行优化。
   - 参考 [PPT](https://riscv.org/wp-content/uploads/2018/12/RISC-V-Vector-Performance-Analysis-Guy-Lemieux.pdf), [PPT](https://llvm.org/devmtg/2019-04/slides/TechTalk-Kruppe-Espasa-RISC-V_Vectors_and_LLVM.pdf) 和 [Blog](https://gms.tf/riscv-vector.html)
8. 访存优化
9.  ...

#### PAIII Bonus代码与实验报告提交
1. 基本优化Pass的代码都写在`src/optimization/`目录下面，头文件放入`include/optimization/`当中，最后只会在这两个目录下验收代码文件。
2. 对于选做的优化Pass，需要发邮件给助教，统一线下验收。
3. 需要在 `README.md` 撰写实验报告，且由队长说明成员贡献比率。其中，包括代码阅读部分的报告，解释你们的基本优化Pass的设计，遇到的困难和解决方案。


注意：组队实验意味着合作，但是小组间的交流是受限的，且严格**禁止**代码的共享。除此之外，如果小组和其它组进行了交流，必须在 `README.md`  中记录下来交流的小组和你们之间交流内容。

## 1. 实验框架

本次实验使用了由C++编写的 LightIR 来在IR层面完成优化化简，为了便于大家进行实验，助教对之前的`ChocoPy`增加了选项，用来选择是否开启某种优化；另外，若想要另外单独去进行某个优化Pass的调试，可以利用助教给出的PassManager来进行Pass的注册和运行。

在`include/ir-optimizer/chocopy_optimization.hpp`中，定义了一个用于管理Pass的类`PassManager`。它的作用是注册与运行Pass。它提供了以下接口：
```cpp
PassManager pm(module.get())
pm.add_Pass<Mem2Reg>(emit)  //注册Pass，emit为true时打印优化后的IR
pm.run()  //按照注册的顺序运行Pass的run()函数
```
基本Pass开发：

每一个Pass有一个cpp文件和对应的hpp文件，可以在hpp里定义辅助类或者成员变量使用，在cpp里的run()函数实现你的Pass。

## 2. 运行与调试

### 运行 chocopy
```sh
mkdir build && cd build
cmake ..
make ir-optimizer
```
编译后会产生 `ir-optimizer` 程序，它能将.py文件输出为LLVM IR，也可以利用clang将IR编译成二进制。程序逻辑写在`chocopy_lightir.cpp`中。

它通过`[-pass [pass_name]]`开关来控制优化Pass的使用，当需要对 `.py` 文件测试时，可以这样使用：
```bash
./ir-optimizer [-pass Mem2reg] [-pass ConstPropagation] [-pass ActiveVars] [-pass LoopInvariant] <input-file>
```
### 自动测试
助教贴心地为大家准备了自动测试脚本，它在 `tests/bonus.py`，使用方法如下：
* 有三个可用的选项：`--ConstPropagation`/`-C`，`--LoopInvHoist`/`-L`，`--ActiveVars/-A'`分别表示用来评测常量传播Pass以及循环不变式外提Pass，以及活跃变量分析Pass。

* 脚本中会使用`taskset`将程序与CPU核心进行绑定，以此来提高时间测试的稳定性；当然如果虚拟机中没有该命令则通过下面的命令来安装：
  ```bash
  sudo apt install schedtool
  ```
  
* 评测脚本会对样例进行编译和执行，然后对生成的可执行文件首先检查结果的正确性，每个样例的正确结果会放在.out文件中，结果正确的情况下才会去进一步评测运行时间。另外，在每类样例目录下中的`baseline`目录中还提供了相应testcase的`.ll`文件来作为baseline，基本Pass的优化效果得分也是要根据baseline的时间来进行计算。

* 如果显示执行时间的表格中出现了`None`则表示该样例有错误。

* 每个样例会运行三次取平均时间（时间单位是s）并且保留两位小数输出，当然每个样例的运行次数也可以自行更改脚本中`repeated_time`变量。

* 活跃变量Pass测试将与答案json文件脚本对比，得分计算规则见下面评分标准
```sh
# 在 tests/lab5 目录下运行：
./lab5_test.py -L
```
如果完全正确，它会输出：
```
========== LoopInvHoist ==========
Compiling  
100%|███████████████| 8/8 [00:00<00:00, 12.16it/s]
Evalution 
100%|███████████████| 8/8 [00:49<00:00,  6.14s/it]
Compiling  -loop-inv-hoist
100%|███████████████| 8/8 [00:00<00:00, 11.85it/s]
Evalution 
100%|███████████████| 8/8 [00:10<00:00,  1.25s/it]
Compiling baseline files
100%|███████████████| 8/8 [00:00<00:00, 13.63it/s]
Evalution 
100%|███████████████| 8/8 [00:07<00:00,  1.09it/s]
testcase         before optimization     after optimization      baseline
testcase-1              0.63                    0.36              0.36
testcase-2              0.46                    0.38              0.37
testcase-3              0.62                    0.36              0.36
testcase-4              0.40                    0.39              0.39
testcase-5              4.96                    0.38              0.38
testcase-6              1.03                    0.08              0.08
testcase-7              2.11                    0.24              0.24
testcase-8              1.98                    0.25              0.25
```
如果要增加样例，直接在样例目录中添加文件即可，命名参考目录下的其他文件。


### logging

[logging](../common/logging.md) 是帮助大家打印调试信息的工具，如有需求可以阅读文档后进行使用

### 建议

1. 比较你们编写的编译器产生的 IR 和 clang 产生的IR来找出可能的问题或发现新的思路
2. 使用 logging 工具来打印调试信息
2. 使用 GDB 等软件进行单步调试来检查错误的原因
3. 合理分工

## 3. 提交要求

### 目录结构


### 提交要求和评分标准

- 提交时间

  本次实验分阶段验收：

  **阶段一**：验收代码阅读报告及相关思考题 

  **阶段二**：验收lab5要求提交的代码

* 提交要求  
  本实验是组队实验，我们将收取**队长**实验仓库中的内容
  
  * 实验部分:
    * 需要填补 
    
      `./include/ir-optimizer/chocopy_optimization.hpp`，`./src/ir-optimizer/chocopy_optimization.cpp`

    * 需要在 `README.md` 目录下撰写实验报告，且由队长说明成员贡献比率
    
    * 本次实验需要更新
    
      `./include/ir-optimizer/chocopy_optimizer.hpp`

    * **选做Pass**在`2022/06/11`之前都可以联系助教验收，然后由助教统一安排线下验收，该部分由学生现场演示，**不需要**撰写实验报告。
* 评分标准: 最终评分按照组队规则，实验完成分组成如下：
  * 阶段一 代码阅读
    
    * README.md (5 分)
  * 阶段二 优化Pass开发
    * **基本Pass (55 分)**
      * README.md (10 分)
      
      * 常量传播 (15 分)
        ```
        对于每一个testcase: 
        (before_optimization-after_optimization)/(before_optimization-baseline) > 0.8 得满分
        (before_optimization-after_optimization)/(before_optimization-baseline) > 0.5 得85%分数
        (before_optimization-after_optimization)/(before_optimization-baseline) > 0.2 得60%分数
        ```
        **注**：`before_optimization`以助教

        若编译出错或者运行出错将不得分，此外评测时所用的`testcase`与发布的不完全一致，最终的评分会映射到15分的总分区间。
        
      * 循环不变式外提 (15 分)
        评分参考常量传播。
        
      * 活跃变量 (15 分)
      
        活跃变量的一个bb的入口处活跃变量或者出口处活跃变量的非空集合算一个分析结果，每个分析结果同分，对于每个分析结果`result`评分采取以下公式，（正确分析结果列表是`answer`）
        $$
        score = \frac{(answer\cap result).size()-(result-answer\cap result).size()}{answer.size()}
        $$
        例如：
      
        ```json
        "live_in":{
            //...
            "label8": ["%arg0","%arg1","%op5","%op6","%op8",]
          //...
        }
        // 算一个分析结果，如果正确答案是如下结果，
        "live_in":{
            //...
            "label8": ["%arg0","%arg1","%op5","%op6","%op7",]
          //...
        }
        // 则相比较于答案，缺少了op7，多分析了op8则此条分析结果得分为(4-1)/5=0.6
        ```
      
    * **选做Pass**
      助教会综合大家的实现完成度和难度给分（是队伍里面的每个人都能有哦）。
    
  * 禁止执行恶意代码，违者本次实验0分处理

* 关于抄袭和雷同
  经过助教和老师判定属于实验抄袭或雷同情况，所有参与方一律零分，不接受任何解释和反驳。如有任何问题，欢迎提issue进行批判指正。

