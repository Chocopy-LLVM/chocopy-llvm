# Programing Assignment III Bonus lab file

- [Programing Assignment III Bonus lab file](#programing-assignment-iii-bonus-lab-file)
  - [0. Preface](#0-preface)
    - [Main work](#main-work)
      - [Code and Material Readings](#code-and-material-readings)
      - [Develop basic optimization Passes](#develop-basic-optimization-passes)
      - [Bonus: Optional Optimization Pass](#bonus-optional-optimization-pass)
      - [PAIII Bonus Code and Lab Report Submission](#paiii-bonus-code-and-lab-report-submission)
  - [1. Experimental framework](#1-experimental-framework)
  - [2. Run and debug](#2-run-and-debug)
    - [Run chocopy](#run-chocopy)
    - [Automated testing](#automated-testing)
    - [logging](#logging)
    - [Suggestions](#suggestions)
  - [3. Submission requirements](#3-submission-requirements)
    - [Directory structure](#directory-structure)
    - [Submission requirements and scoring criteria](#submission-requirements-and-scoring-criteria)

## 0. Preface

This experiment is a team experiment, please read the team requirements carefully and divide the work reasonably.

**Pass concept**: accept a `module` as an argument, after PA III you can know that `module` is the uppermost structure of IR, and `Pass` traverses the structure within `module` and analyzes the information (e.g., analysis of active variables `Pass`), or does some transformations (e.g., constant propagation and loop-invariant outer lift `Pass` in this experiment).

After PA III, I believe you have mastered the structure of LightIR and have a better understanding of LLVM IR. In this experiment, we are going to implement three simple in-block optimization Pass and analysis Pass based on the understanding of SSA (static single assignment) format: constant propagation, cyclic invariant outpropagation, and active variable analysis.

It is worth mentioning the LightIR Core Class Introduction in [LightIR](../common/LightIR.md) in the User class for the `operands_` member which is the operand list, and the `use_list_` member of the Value class, these two chains describe the data dependencies between instructions, please take note of them.

### Main work

#### Code and Material Readings
For specific requirements and instructions refer to: [reading.md](./reading.md)

1. read the optimization section in `Compiler_USTC.pdf`, especially the static single assignment and active variables section.
2. read the code of `Mem2Reg` and `LoopSearch`, describe the basic flow of optimization, and answer the reflection questions.
3. Read the code and learn how to develop LightIR-based optimization Pass

#### Develop basic optimization Passes

1. **Constant Propagation**
   Be able to implement the direct replacement of variables that can calculate the result in the compilation and optimization phase with constants; add the following points to be noted.
    a. Only the propagation of constants within the process needs to be considered, and arrays can be disregarded. **Global variables only need to be considered for the propagation of constants within the block**, here is an example to illustrate the propagation of constants.
    ```cpp
    %a = 1 + 1.
    %b = %a + %c;
    ```
    So first we can collapse `%a = 1 + 1` into `%a = 2`, then we find that the instruction `%b = %a + %c` uses `%a`, then we can replace `%a` in `%b = %a + %c` directly with the constant 2, and the code translates to
    ```cpp
    %a = 2;
    %b = 2 + %c;
    ```
    Of course this experiment requires an extra step: the useless statement `%a = 2` is deleted because `%a` is a constant and has been propagated to where it is used, so this assignment statement can be deleted (since our IR is in SSA form, we don't have to worry about %a being reassigned): ``
    ```cpp
    %b=2+%c;
    ```
    b. Both integer floating-point types need to be considered.
    d. For the case of `a=1/0`, it can be disregarded, i.e., you can do the processing or not.
    c. Do remove the useless branch to change the conditional jump to mandatory jump, for example, the following statement, the else part can be removed
   
    ```cpp
    int a;
    a=1;
    if(a){...}
    else{...}
    ```
    Also for the case of branch nesting are able to delete the useless branches, after this step for the possible unreachable blocks are required to be deleted, and as for the redundant blocks containing only jump instructions are not required to be implemented in this experiment.
   
2. **Loop-invariant outer lift**
    To be able to implement the extraction of expressions that are not related to the loop to the outside of the loop. No need to consider arrays, with global variables. As an example.
    
    ```cpp
    while(i<10){
      while(j<10){
        a=i+1;
        j=j+1;
      }
      i=i+1;
    }
    ```
    Then the expression `a=i+1` is irrelevant to the inner loop and can be referred to outside the j-loop as follows.
    ```cpp
    while(i<10){
      a=i+1;
      while(j<10){
        j=j+1;
      }
      i=i+1;
    }
    ```
    The following are some tips for loop outer lifting.
    a. Think about how to determine the statement is not related to the loop and there are no side effects of the outer lift
    b. The conditional block of the loop (that is, the Base block found in LoopSearch) has at most two precursors, so think about which precursor the invariant should be referred to externally.
    
3. **active variable analysis**

   The active variables that enable the analysis of the entry and exit of bb blocks, see the annex for reference (section 9.2.4 of the Purple Book), are defined in `ActiveVars.hpp` with two members `live_in`, `live_out`, you need to insert `pair<bb, IN[bb]>` into the map structure of `live_in` and ` pair<bb, OUT[bb]>` into the map structure of `live_out`, and call the print() method in the ActiveVars class to output the bb active variable case to a json file, and the helper will approve the json file based on your output. (To ensure the consistency of output variable names, please do not perform naming operations on instructions, bb, etc.)
   
   **Hint**: There is no phi node design in the material, so the data flow equation: $OUT[B] = \cup_{s is a successor of B}IN[S]$ is defined to imply that a variable active at the entrance of S is active at the exit of all its predecessors, due to the special nature of the `phi` instruction, e.g. `%0 = phi [%op1, %bb1], [%op2, % bb2]` If you use the data flow equation as above, by default this `phi` instruction produces both `op1` and `op2` active, while in fact only the control flow coming from `%bb1` has `%op1` active and coming from `%bb2` has `%op2` active. So some modifications need to be made to this data flow equation.
   
4. ...


#### Bonus: Optional Optimization Pass
The following optimization can be referred to [graduation design](https://github.com/alicemare/learn_compiler), no mandatory requirements, free to play, but need to explain in the report to achieve the effect, and offline acceptance to show, optional to do Pass until `2022/06/11` can contact the teaching assistant to accept Then the teaching assistant will arrange offline acceptance.
1. tokenized dead code removal (inter-process analysis supported)
3. function inlining
4. inter-block public sub-expression removal
5. tail recursion elimination
6. multi-threading
7. vectorization
   - Reference clang `clang -O3 -mllvm -riscv-v-vector-bits-min=256 -save-temps -Wno-unused-command-line-argument --target=riscv64-unknown-elf -march= The results of running rv32gcv0p10 -menable-experimental-extensions -mabi=lp32d` are optimized for `vectorize.py` and `vectorize1.py`.
   - References [PPT](https://riscv.org/wp-content/uploads/2018/12/RISC-V-Vector-Performance-Analysis-Guy-Lemieux.pdf), [PPT](https://llvm.org/devmtg/2019-04/slides/TechTalk-Kruppe-Espasa-RISC-V_Vectors_and_LLVM.pdf) and [Blog](https://gms.tf/riscv-vector.html)
8. access optimization
9. ...

#### PAIII Bonus Code and Lab Report Submission
1. The code of the basic optimization pass is written in the `src/optimization/` directory, and the header files are placed in `include/optimization/`, and only the code files will be accepted in these two directories.
2. For the optional Optimization Pass, you need to send an email to the teaching assistant to unify the offline acceptance. 3.
3. you need to write a lab report in `README.md`, and the team leader should indicate the contribution ratio of the members. This includes a report on the code reading part, explaining the design of your basic optimization Pass, the difficulties encountered and the solutions.


Note: Team experiments imply collaboration, but communication between groups is limited and sharing of code is strictly **prohibited**. In addition, if the group communicates with other groups, you must document in `README.md` the group that communicated and what you communicated with each other.

## 1. Experimental framework

This experiment uses LightIR written in C++ to simplify the optimization at the IR level. In order to facilitate the experiment, the assistant professor has added an option to the previous `ChocoPy` to choose whether to enable a certain optimization; in addition, if you want to debug a certain optimized Pass separately, you can use the PassManager given by the assistant professor to Pass registration and operation.

In `include/ir-optimizer/chocopy_optimization.hpp`, a class `PassManager` is defined for managing Pass. Its role is to register and run Pass. it provides the following interfaces.
```cpp
PassManager pm(module.get())
pm.add_Pass<Mem2Reg>(exit) //register Pass, print optimized IR when exit is true
pm.run() //Run the run() function of Pass in the order of registration
```
Basic Pass development.

Each Pass has a cpp file and a corresponding hpp file. You can define auxiliary classes or member variables to use in hpp, and implement your Pass in the run() function in cpp.

## 2. Run and debug

### Run chocopy
```sh
mkdir build && cd build
cmake ...
make ir-optimizer
```
The compilation will produce the `ir-optimizer` program, which outputs the .py file as an LLVM IR, and can also compile the IR into binary using clang. The program logic is written in `chocopy_lightir.cpp`.

It controls the use of optimized Pass via the `[-pass [pass_name]]` switch, which can be used when a test of a `.py` file is needed, as follows
```bash
./ir-optimizer [-pass Mem2reg] [-pass ConstPropagation] [-pass ActiveVars] [-pass LoopInvariant] <input-file>
```
### Automated testing
The helper has kindly prepared an automated test script for you, which is in `tests/bonus.py` and is used as follows.
* There are three available options: `-ConstPropagation`/`-C`, `-LoopInvHoist`/`-L`, and `-ActiveVars/-A'` which indicate that they are used to evaluate constant propagation Pass as well as loop invariant outer lift Pass, and active variable analysis Pass, respectively.

* The script uses ``taskset`` to bind the program to the CPU core to improve the stability of the time test; of course, if the command is not available in the virtual machine, it is installed with the following command.
  ```bash
  sudo apt install schedtool
  ```
  
* The evaluation script will compile and execute the samples, and then check the correctness of the results in the resulting executable file first, and the correct results of each sample will be placed in the .out file. In addition, the `.ll` file of the corresponding testcase is provided as a baseline in the `baseline` directory under each category of samples, and the optimization score of basic Pass is also calculated based on the time of the baseline.

* If `None` appears in the table showing the execution time, it means that the sample has errors.

* Each sample is averaged three times (in s) and output with two decimal places, but the number of times each sample is run can be changed by changing the `repeated_time` variable in the script.

* The active variable Pass test will be compared with the answer json file script, and the rules for calculating the score are given in the following scoring criteria
```sh
# In the tests/pa3 directory run.
./bonus.py -L
```
If it is exactly right, it will output.
```
========== LoopInvHoist ==========
Compiling  
100%|███████████████| 8/8 [00:00<00:00, 12.16it/s]
Evalution 
100%|███████████████| 8/8 [00:49<00:00, 6.14s/it]
Compiling -loop-inv-hoist
100%|███████████████| 8/8 [00:00<00:00, 11.85it/s]
Evalution 
100%|███████████████| 8/8 [00:10<00:00, 1.25s/it]
Compiling baseline files
100%|███████████████| 8/8 [00:00<00:00, 13.63it/s]
Evalution 
100%|███████████████| 8/8 [00:07<00:00, 1.09it/s]
testcase before optimization after optimization baseline
testcase-1 0.63 0.36 0.36
testcase-2 0.46 0.38 0.37
test case-3 0.62 0.36 0.36
testcase-4 0.40 0.39 0.39
test case-5 4.96 0.38 0.38
test case-6 1.03 0.08 0.08
testcase-7 2.11 0.24 0.24
testcase-8 1.98 0.25 0.25
```
If you want to add samples, just add files directly to the samples directory, naming the other files in the reference directory.


### logging

[logging](../common/logging.md) is a tool to help you print debugging information, you can read the documentation and use it if you need to

### Suggestions

1. compare the IR generated by the compiler you wrote with the IR generated by clang to find possible problems or discover new ideas
2. use logging tools to print debugging information
2. use software such as GDB for single-step debugging to check the cause of errors
3. rational division of labor

## 3. Submission requirements

### Directory structure


### Submission requirements and scoring criteria

- Submission time

  This experiment is accepted in phases.

  **Phase I**: Acceptance of code reading report and related reflection questions 

  **Phase II**: Acceptance of the code required by lab5

* Submission requirements  
  This experiment is a group experiment, we will collect the content in the **Captain** experiment repository
  
  * Experiment section :
    * Need to fill 
    
      `./include/ir-optimizer/chocopy_optimization.hpp`, `./src/ir-optimizer/chocopy_optimization.cpp`

    * The experiment report should be written in the `README.md` directory, and the team leader should indicate the contribution rate of the members.
    
    * This experiment needs to be updated
    
      `./include/ir-optimizer/chocopy_optimizer.hpp`

    * **Optional Pass** can contact the teaching assistant for acceptance until `2022/06/11`, and then the teaching assistant will arrange offline acceptance, and the part will be demonstrated by the students on site, **no need to **write the experiment report.
* Scoring criteria: The final scoring is in accordance with the team rules, and the experiment completion score consists of the following.
  * Stage 1 Code Reading
    
    * README.md (5 points)
  * Phase II Optimization Pass Development
    * **Basic Pass (55 points)**
      * README.md (10 points)
      
      * Constant Propagation (15 points)
        ```
        For each testcase: 
        (before_optimization-after_optimization)/(before_optimization-baseline) > 0.8 scores full points
        (before_optimization-after_optimization)/(before_optimization-baseline) > 0.5 for a score of 85%
        (before_optimization-after_optimization)/(before_optimization-baseline) > 0.2 for a score of 60%
        ```
        **Note**: `before_optimization` to help teach

        No score will be given if there is a compile error or run error, in addition the `testcase` used in the evaluation is not exactly the same as the published one, the final score will be mapped to a total score range of 15 points.
        
      * Cyclic invariant outpropagation (15 points)
        Scoring reference constant propagation.
        
      * Active variable (15 points)
      
        The non-empty set of active variables at the entrance or active variables at the exit of a bb of active variables counts as one analysis result, each with the same score, and for each analysis result `result` scoring takes the following formula, (the correct list of analysis results is `answer`)
        $$
        score = \frac{(answer\cap result).size()-(result-answer\cap result).size()}{answer.size()}
        $$
        Example.
      
        ```json
        "live_in":{
            //...
            "label8": ["%arg0","%arg1","%op5","%op6","%op8",]
          //...
        }
        // Count the result of an analysis if the correct answer is the following
        "live_in":{
            //...
            "label8": ["%arg0","%arg1","%op5","%op6","%op7",]
          //...
        }
        // then compared to the answer, the lack of op7, more analysis of op8 then this analysis results in a score of (4-1)/5 = 0.6
        ```
      
    * **Optional Pass**
      The assistant teacher will integrate everyone's implementation completion and difficulty to give points (is everyone inside the team can have oh).
    
  * Prohibit the implementation of malicious code, violators of this experiment 0 points to deal with

* About plagiarism and similarity
  After the teaching assistant and teacher determine that the experiment is plagiarized or similar, all participants will receive zero points, and will not accept any explanation or rebuttal. If you have any questions, please feel free to issue a critique and correction.

