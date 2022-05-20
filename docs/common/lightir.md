# Light IR

<!-- TOC -->

- [Light IR](#light-ir)
  - [LightIR](#lightir)
    - [IR Features](#ir-features)
    - [IR Format](#ir-format)
    - [Instruction](#instruction)
      - [Terminator Instructions](#terminator-instructions)
        - [Ret](#ret)
        - [Br](#br)
      - [Standard binary operators](#standard-binary-operators)
        - [Add](#add)
        - [Sub](#sub)
        - [Mul](#mul)
        - [Div](#div)
      - [Memory operators](#memory-operators)
        - [Alloca](#alloca)
        - [Load](#load)
        - [Store](#store)
      - [CastInst](#castinst)
        - [ZExt](#zext)
        - [Trunc](#trunc)
      - [Riscv VExt](#riscv-vext)
        - [Vstore](#vstore)
        - [Vload](#vload)
      - [Other operators](#other-operators)
        - [ICmp](#icmp)
        - [Call](#call)
        - [GetElementPtr](#getelementptr)
  - [C++ APIs](#c-apis)
    - [Core class concept map](#core-class-concept-map)
    - [BasicBlock](#basicblock)
    - [Constant](#constant)
    - [Function](#function)
    - [GlobalVariable](#globalvariable)
    - [IRBuilder](#irbuilder)
    - [Instruction](#instruction-1)
    - [Module](#module)
    - [Type](#type)
    - [User](#user)
    - [Value](#value)
    - [Class](#class)
    - [Summary](#summary)

<!-- /TOC -->

## LightIR

In the middle of lab3 we will use a compactly defined LLVM IR, whose output is compatible with the LLVM IR and can be compiled by clang into `.o` files and then linked with a linker to a program that can be run on the riscv machine. Validation of the `.ll` file can be done with the [Godbolt](https://godbolt.org/z/qcMhaYeca). The following is a simple definition, more detailed language semantic [official website](https://llvm.org/docs/LangRef.html) .

### IR Features

- Using the 3-address approach
    - Unlike the X86 assembly mode where target and source registers are shared: ADD EAX, EBX
    - %2 = add i32 %0, %1
- SSA form + infinite registers
    - Each variable is assigned only once
    - Easy to determine dependencies between operations for optimization analysis
- Strongly typed system
    - Each Value has its own type.
    - IR type system.
        - `i1`: 1-bit wide integer type
        - `i8`: 8-bit wide integer type, `i8*` is used to represent the `str` type
        - `i32`: 32-bit wide integer type
        - `pointer`: pointer type
            - Example: `i32*, [10 x i32*]`
        - `label` the identifier type of bb
        - `functiontype` function type, including function return value type and parameter type (not mentioned in the following documentation)

### IR Format

Take the following `easy.c` and `easy.ll` as an example for illustration.  
The command `clang -S -emit-llvm easy.c` gives you the corresponding `easy.ll` as follows (the helper adds extra comments). Comments in the `.ll` file start with `;`.

- `easy.c`:
  ``` c
  int main(){
    int a;
    int b;
    a = 1;
    b = 2;
    if(a < b)
      b = 3;
    return a + b;
  }
  ```
- `easy.ll`:
  ``` c
  ; Comments: Comments in .ll file start with ';'
  ModuleID = 'easy.c'                                
  source_filename = "easy.c"  
  ; Comment: Start of target
  target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
  target triple = "riscv32-unknown-linux-gnu"
  ; comment: end of target
  
  ; Comment: definition of global main function
  ; Function Attrs: noinline nounwind optnone uwtable
  define dso_local i32 @main() #0 {
  ; Comment: Start of the first basic block
    %1 = alloca i32, align 4
    %2 = alloca i32, align 4
    %3 = alloca i32, align 4
    store i32 0, i32* %1, align 4
    store i32 1, i32* %2, align 4
    store i32 2, i32* %3, align 4
    %4 = load i32, i32* %2, align 4
    %5 = load i32, i32* %3, align 4
    %6 = icmp slt i32 %4, %5
    br i1 %6, label %7, label %8
  ; Comment: End of the first basic block

  ; comment: start of the second basic block
  7: ; preds = %0
    store i32 3, i32* %3, align 4
    br label %8
  ; Comment: End of the second basic block
  
  ; Comment: Start of the third basic block
  8: ; preds = %7, %0
    %9 = load i32, i32* %2, align 4
    %10 = load i32, i32* %3, align 4
    %11 = add nsw i32 %9, %10
    ret i32 %11 ; Comment: Return statement
  ; Comment: End of the third basic block
  }
  
  attributes_ #0 = { noinline nounwind optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
  
  !llvm.module.flags = !{!0}
  !llvm.ident = !{!1}
  
  !0 = !{i32 1, !"wchar_size", i32 4}
  !1 = !{!"clang version 10.0.1 "}
  ```

Each program consists of 1 or more modules, and the modules are merged with each other by LLVM Linker.  
Each module is composed as follows.

- Target Information.
  ``` c
  target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
  target triple = "riscv32-unknown-linux-gnu"
  ```
- Global Symbols: definition of the main function
- Others:Trailing other information

The composition of each function is as follows.

- Header: function return value type, function name, function parameters
- One or more basic blocks.
    - Each basic block is in turn composed of Label and Instruction.
      ``` c
      8: ; preds = %7, %0
        %9 = load i32, i32* %2, align 4
        %10 = load i32, i32* %3, align 4
        %11 = add nsw i32 %9, %10
        ret i32 %11  
      ```
      In this example, `8` is Label.  
      In `%9 = load i32, i32* %2, align 4`, `%9` is the destination operand, `load` is the instruction helper, `i32` is the type of `int32`, `i32*` is the type of address pointing to `int32`, `%2`
      is the source operand, and `align 4` indicates alignment.

### Instruction

#### Terminator Instructions

**Note**: ret and br are both Terminator Instructions which means termination instructions. In the definition of llvm basic block, the basic block is single-in, single-out, so there can be only one termination instruction (ret or br). When a basic block has two terminator instructions, clang
will consider the first termination instruction to be the end of the block and will open a new anonymous block (and take the next number).

##### Ret

- Concept: The ` ret` instruction is used to return the control flow (and optionally the value) from the function to the caller. There are two forms of the `ret` instruction: one that returns the value and then terminates the function, and one that terminates the function only.
- The format
    - `ret <type> <value>`
    - `ret void`

- Example.
    - `ret i32 %0`
    - `ret void`

##### Br

- Concept: The `br` instruction is used to cause the control flow to be transferred to another basic block in the current function. There are two forms of this instruction, corresponding to conditional branches and unconditional branches.
- Format.
    - `br i1 <cond>, label <iftrue>, label <iffalse>`
    - `br label <dest>`
- Example.
    - `br i1 %cond label %truebb label %falsebb`
    - `br label %bb`

#### Standard binary operators

##### Add

- Concept: The `add` instruction returns the sum of its two operands of type `i32`, and the return value is of type `i32`. The `fadd` instruction returns the sum of its two operands of type `float`, and the return value is of type `float`.
- Format.
    - `<result> = add <type> <op1>, <op2>`
- Example.
    - `%2 = add i32 %1, %0`

##### Sub

- Concept: The `sub` instruction returns the difference between its two operands of type `i32`, and the return value is of type `i32`, and the `fsub` instruction returns the difference between its two operands of type `float`, and the return value is of type `float`
- The format and examples are similar to `add`

##### Mul

- Concept: The `mul` instruction returns the product of its two operands of type `i32`, the return value is of type `i32`, the `fmul` instruction returns the product of its two operands of type `float`, the return value is of type `float`
- The format and examples are similar to `add`

##### Div

- Concept: The `div` instruction returns the quotient of its two operands of type `i32`, the return value is of type `i32`, the `fdiv` instruction returns the quotient of its two operands of type `float`, the return value is of type `float`
- The format and examples are similar to `add`

#### Memory operators

##### Alloca

- Concept: The `alloca` instruction allocates memory on the stack frame of the currently executing function, which is automatically freed when the function returns to its caller. Always allocate objects in the address space for the allocated resources indicated in the data layout
- Format: `<result> = alloca <type>`
- Example.
    - `%ptr = alloca i32`
    - `%ptr = alloca [10 x i32]`

##### Load

- Concept: The `load` instruction is used to read from memory.
- Format: `<result> = load <type>, <type>* <pointer>`
- Example: `%val = load i32, i32* %ptr`

##### Store

- Concept: `store` instruction is used to write to memory
- Format: `store <type> <value>, <type>* <pointer>`
- Example: `store i32 3, i32* %ptr`

#### CastInst

##### ZExt

- Concept: The `zext` instruction extends its operands **zero** to the type `type2`.
- Format: `<result> = zext <type> <value> to <type2>`
- Example: `%1 = zext i1 %0 to i32`

##### Trunc

- Concept: The `Trunc` instruction compresses its operands **zero** to the type `type2`.
- Format: `<result> = trunc <type> <value> to <type2>`
- Example: `%1 = trunc i32 %0 to i1`

#### Riscv VExt

##### Vstore

- Concept: The `zext` instruction extends its operand **zero** to the type `type2`.
- Format: `<result> = zext <type> <value> to <type2>`
- Example: `%1 = zext i1 %0 to i32`

##### Vload

- Concept: The `Trunc` instruction compresses its operands **zero** to the type `type2`.
- Format: `<result> = trunc <type> <value> to <type2>`
- Example: `%1 = trunc i32 %0 to i1`

#### Other operators

##### ICmp

- Concept: The `icmp` instruction returns a boolean value based on the comparison of two integers.
- Format.
    - `<result> = icmp <cond> <type> <op1>, <op2>`
        - `<cond> = eq | ne | sgt | sge | slt | sle`
- Example: `i1 %2 = icmp sge i32 %0, %1`

##### Call
- Concept: The `call` instruction is used to cause the control flow to be transferred to the specified function with incoming arguments bound to the specified values. After the `ret` instruction is executed in the called function, the control flow will continue to execute the instruction after the function call and the return value of the function is bound to the `result` parameter.
- Format.
    - `<result> = call <return ty> <func name>(<function args>) `
- Example.
    - `%0 = call i32 @func( i32 %1, i32* %0)`
    - `call @func( i32 %arg)`

##### GetElementPtr

- Concept: The `getelementptr` instruction is used to get the addresses of the elements of an array structure.
  It only performs address calculation and does not access memory. This directive can be [referenced](https://blog.yossarian.net/2020/09/19/LLVMs-getelementptr-by-example) .
- Format: `<result> = getelementptr <type>, <type>* <ptrval> [, <type> <idx>]`
- Explanation of parameters: The first parameter is the calculation base type, the second and third parameters indicate the pointer type and pointer at the beginning of the index, and `[]` indicates the repeatable parameter, which indicates the offset type and offset value of the array index. (Question: Think about the pointer type `[10 x i32]` pointer and `i32`)
  pointer of type `[10 x i32]` and the `getelementptr` pointer, and give an explanation, and answer the difference between the two usage cases at the end of the experiment)
- Example.
    - `%2 = getelementptr [10 x i32], [10 x i32]* %1, i32 0, i32 %0`
    - `%2 = getelementptr i32, i32* %1 i32 %0`

## C++ APIs

### Core class concept map

! [c++](./c++.png)

### BasicBlock

- Meaning: Basic block, is a single-input single-output block, the class maintains a chain of instructions, the basic block itself belongs to Value, type \<label\>, will be called by branch instructions

- Members.

    - instr_list_: instruction chain table
    - pre_basic_blocks_: bb precursor set
    - succ_basic_blocks_: bb successor set

- API:

  ```c++
  static BasicBlock *create(Module *m, const std::string &name , Function *parent )
  // Create and return the BB block, the parameters are the Module to which the BB block belongs, name is its name default to null, and the Function to which the BB block belongs
  Function *get_parent();
  // return the function to which the BB block belongs
  Module *get_module();
  // return the Module to which the BB block belongs
  Instruction *get_terminator();
  // return the terminator instruction of the BB block (ret|br) if the last instruction of the BB block is not a terminator instruction return null
  void add_instruction(Instruction *instr);
  // add instruction *instr to the end of the instruction chain of this BB block, call IRBuilder to create the function will automatically call this method
  void add_instr_begin(Instruction *instr);
  // Add instruction *instr to the beginning of this BB block instruction chain
  void delete_instr(Instruction *instr);
  // Remove instruction str from the BB block instruction chain, and call api to maintain the use chain of str's operands.
  bool empty();
  // return true if the BB block is empty
  int get_num_of_instr();
  // return the number of instructions in the BB block
  std::list<Instruction *> &get_instructions();
  // return the chain of instructions in the BB block
  void erase_from_parent();
  // remove this BB block from the bb chain table of the function it belongs to
      
  /****************api about cfg****************/
  std::list<BasicBlock *> &get_pre_basic_blocks() // return the predecessor fast set
  std::list<BasicBlock *> &get_succ_basic_blocks() // return the set of successor blocks
  void add_pre_basic_block(BasicBlock *bb) // add precursor blocks
  void add_succ_basic_block(BasicBlock *bb) // add successor blocks
  void remove_pre_basic_block(BasicBlock *bb) // Remove the predecessor block
  void remove_succ_basic_block(BasicBlock *bb) // remove the successor block
  /****************api about cfg****************/
  
  ```

### Constant

- Inheritance: User
- Meaning: constants, base class for various types of constants
- Subclasses.
    - ConstantInt.
        - Meaning: constants of type int

        - Member

            - val_: constant value

        - API.

          ```cpp
          int get_value() // return the constant value stored in this constant type
          static int get_value(ConstantInt *const_val) // return the constant value stored in the constant type const_val
          static ConstantInt *get(int val, Module *m) // Create a constant class with the value of val
          static ConstantInt *get(bool val, Module *m) // Create a bool constant class with the value of val
          ```

    - ConstantStr:
        - Meaning: constant of type i8*

        - Members.

            - val_: constant value

        - API.

          ```cpp
          static ConstantStr *get(string val, Module *m) // Create and return i8* number constant class with val value
          float get_value() // return the constant value stored in this constant type
          ```

    - ConstantZero.

        - Meaning: Constant zero value used for initialization of global variables.

        - API.

          ```cpp
          static ConstantZero *get(Type *ty, Module *m);// Create and return ConstantZero constant class
          ```

    - ConstantArray

        - Meaning: array type constants
        - Members.
            - const_array_: array constant value

        - API: cminus syntax does not need the support of array constants (this experiment does not need to use), so we do not explain it here. If you are interested, you can check the source code yourself.

### Function

- Meaning: function, this class describes a simple procedure of LLVM, maintaining the basic block table, formatting the parameter table

- Members.
    - basic_blocks_: list of basic blocks
    - arguments_: list of formal parameters
    - parent_: the module to which the function belongs

- API.

  ```cpp
  static Function *create(FunctionType *ty, const std::string &name, Module *parent);
  // Create and return Function, the parameters are, in order, the function type ty to be created, the function name name (not null), the Module to which the function belongs
  FunctionType *get_function_type() const;
  // Returns the function type of this function class
  Type *get_return_type() const;
  // Returns the return value type of this function type
  void add_basic_block(BasicBlock *bb);
  // add bb to Function's bb chain (call to create function in bb will automatically call this function to hang on function's bb chain)
  unsigned get_num_of_args() const;
  // get the number of function arguments
  unsigned get_num_basic_blocks() const;
  // Get the number of basic blocks of the function
  Module *get_parent() const;
  // Get the Module to which the function belongs
  std::list<Argument *>::iterator arg_begin() 
  // Get the starting iterator for the list of the function's form parameter
  std::list<Argument *>::iterator arg_end() 
  // get the end iterator of the list of function formal parameters
  void remove(BasicBlock* bb) 
  // remove a bb from the function's bb chain
  std::list<BasicBlock *> &get_basic_blocks() 
  // return the function's bb chain table
  std::list<Argument *> &get_args() 
  // return the function's form reference chain
  void set_instr_name();
  // name the unnamed basic blocks and instructions in the function
  ```


- Related classes.
    - Argument.
        - Meaning: Argument

        - Member.
            - arg_no_: argument serial number
            - parent_: the function to which the argument belongs

        - API

          ```cpp
          Function *get_parent() // Returns the function to which the argument belongs
          unsigned get_arg_no() const // return the first argument of the function where the argument belongs to
          ```

### GlobalVariable

- Meaning: global variable, this class is used to represent global variables, it is a subclass of GlobalValue, it is accessed by address
- Members.
    - is_const: whether it is a constant or not
    - init_val_: initial value
- API: Since chocopy semantics requires all global variables to be initialized to the required variables by default, it is possible to call its initialization API, notably the use of zerointializer.

### IRBuilder

- Meaning: Helper class for generating IR. This class provides an independent interface to create various IR instructions and insert them into basic blocks, this helper class does not do any type checking.

- API.


  ```cpp
  BasicBlock *get_insert_block() // returns the BB into which the instruction is being inserted
  void set_insert_point(BasicBlock *bb)// set the bb of the current instruction to be inserted
  Instruction *create_[instr_type]() // create instr_type (specific name reference IRBuilder.h code) instructions and corresponding inserted into the BB block being inserted, this type of instruction to see the function name and parameter names and IR document is a one-to-one correspondence.
  ```

### Instruction

- Inheritance: inherited from User
- Meaning: Instruction, this class is the base class for all LLVM instructions, mainly maintains the opcode of the instruction (instruction class), the basic block to which the instruction belongs, and the operand number information of the instruction
- Members
    - parent_: the BasicBlock to which the instruction belongs
    - op_id_: the type id of the instruction
    - num_ops_: the number of operands of the instruction
- Subclasses.
    - BinaryInst: binary instructions including add, sub, mul, div
    - Other subclasses and the instructions mentioned in the previous document correspond one by one, not here to repeat.
- API: all instructions are created through the IRBuilder, do not need to pay attention to the implementation details of the Instruction class, (**Note**: not through the IRBuilder to create instructions, but directly call the instruction subclasses to create the method without teaching assistant perfect test)

### Module

- Meaning: a compilation unit, in the sense of this source language is a file

- Members.
    - function_list_: function chain table, which records all functions of this compilation unit
    - global_list_: chain of global variables
    - instr_id2string_: the string to be printed by the instruction type id
    - module_name_, source_file_name: not used
    - The basic types that can be fetched from the module

- API.

  ```cpp
  Type *get_void_type(); 
  // Get the void type in IR. Other types can be obtained with similar APIs (the recommended method to get the type is Type::get() provided by the lab3 tutor)
  void add_function(Function *f);
  // hook f to the module's function chain, this method will be called automatically when a function is created to add a function
  void add_global_variable(GlobalVariable* g);
  // hook g to the module's GlobalVariable chain and call this method automatically to add the GlobalVariable when it is created
  std::list<GlobalVariable *> get_global_variable();
  // Get a list of global variables
  std::string get_instr_op_name( Instruction::OpID instr ).
  // Get the instruction name corresponding to instr (called when printing ir)
  void set_print_name();
  // set the instruction to print ir with the bb name.
  ```

### Type

- Meaning: the type of IR, the class is a superclass of all types

- Members.

    - tid_: enumeration type, indicates the type of type (contains VoidType, LabelType, FloatType, Int1, Int32, ArrayType, PointerType)

- Subclasses.
    - IntegerType
        - Meaning: int type

        - Member

            - num_bits: length (i1 or i32)

        - API.

          ```cpp
          unsigned get_num_bits();// return the number of bits of int
          ```
    - FloatType

        - Meaning: float type
    - FunctionType
        - Meaning: function type

        - Member
            - result_: return value type
            - args_: list of argument types

        - API.

          ```cpp
          static FunctionType *get(Type *result, std::vector<Type*> params);
          // return the function type, the parameters are in turn the return value type result, the list of formal parameter types params
          unsigned get_num_of_args() const;
          // return the number of formal parameters
          Type *get_param_type(unsigned i) const;
          // return the type of the i-th formal parameter
          std::vector<Type *>::iterator param_begin() 
          // Returns the starting iterator for the list of formal parameter types
          std::vector<Type *>::iterator param_end() 
          // Returns the end iterator of the list of formal parameter types    
          Type *get_return_type() const;
          // return the return value type in the function type
          ```
    - ArrayType
        - Meaning: array type

        - Members.
            - contained_: type of the array member
            - num_elements_: the number of dimensions of the array

        - API.

          ```cpp
          static ArrayType *get(Type *contained, unsigned num_elements)
          // return the type of the array, the parameters are, in order, the type of the array element contained, the number of array elements num_elements
          Type *get_element_type() const 
          // Returns the type of the array elements
          unsigned get_num_of_elements() const
          // Returns the number of elements in the array
          ```
    - PointerType
        - Meaning: pointer type

        - Member

            - contained_: type of pointer pointers

        - API.

          ```cpp
          Type *get_element_type() const { return contained_; }
          // Return the type pointed by the pointer
          static PointerType *get(Type *contained);
          // Create a pointer type to a contained type
          Type *get_pointer_element_type()
          // returns the type pointed to by the pointer for pointertype, nullptr for the rest
          ```

- API:

  ```cpp
  bool is_void_type() // determine if it is a void type other types have similar API please see Type.h
  static Type *get_void_type(Module *m);// get the void type
  Type *get_pointer_element_type();// return the type pointed to if it is a PointerType, return nullptr if it is not.
  Type *get_array_element_type();// Returns the type pointed to if it is ArrayType, or nullptr if it is not.
  ```

### User

- Inheritance: inherit from value

- Meaning: user, provides a table of operands, each operand in the table points directly to a Value, provides use-def information, itself is a subclass of Value, Value
  class maintains a list of users of this data, providing use-def information. Simply put, the operand table indicates who I use, and the list of data users indicates who uses me. These two tables will be more important in subsequent **optimization experiments**, so be sure to understand them.

- Members.
    - operands_: a list of parameters, indicating the parameters used by this user
    - num_ops_: the number of arguments used by this user

- API.

  ```cpp
  Value *get_operand(unsigned i) const;
  // Retrieve the i-th operand from the user's operand chain
  void set_operand(unsigned i, Value *v);
  // set the i-th operand of user to v
  void add_operand(Value *v);
  // hook v to User's operand chain
  unsigned get_num_operand() const;
  // Get the size of the operand chain table
  void remove_use_of_ops();
  // Remove the User from the use_list_ of all operands in the User's operand chain;
  void remove_operands(int index1,int index2);
  // Remove operands with index index1-index2 from the operand chain, e.g. if you want to remove the 0th operand: remove_operands(0,0)
  ```

### Value

- Meaning: the most basic class, representing an operand, representing a data with type that may be used to command an operand

- Members.
    - use_list_: a list of all instructions that use this operand
    - class_name_: name
    - type_: type, a type class, representing the type of the operand

- API.

  ```cpp
  Type *get_type() const // return the type of this operand
  std::list<Use> &get_use_list() // returns a chain of users of value
  void add_use(Value *val, unsigned arg_no = 0);
  // add val to this's user list
  void replace_all_use_with(Value *new_val);
  // replace this with new_val in all places and maintain the use_def and def_use chains
  void remove_use(Value *val);
  // remove val from this's use_list_
  ```

### Class

-
Meaning: class used to characterize Class, inheritance semantics refer to [LLVM-C++ implementation](https://mapping-high-level-constructs-to-llvm-ir.readthedocs.io/en/latest/object-oriented- constructs/multiple-inheritance.html)
.

- -
Meaning: Classes used to characterize Class, inheritance semantics reference [LLVM-C++ implementation](https://mapping-high-level-constructs-to-llvm-ir.readthedocs.io/en/latest/object-oriented- constructs/multiple-inheritance.html)
.

- Members.
    - type_tag_: records the list of all instructions that use this operand
    - prototype_label_: records the address where this class prototype is stored
    - attributes_: ProtoType Table, used to record all member variables of this class, and also store the variables of the inherited class.
    - dispatch_table_label_: the address where the class attribute is stored
    - methods_: Dispatch Table table, a type class, used to store the existing implementation of this class and override the virtual function pointers of the inherited class.

- API.

  ```cpp
  void add_attribute(AttrInfo *attrInfo) const; //add an attribute
  void add_method(Func *method_) const; //add an attribute
  int get_size(); // return the size of attrubutes, but not the second argument of prototype, need get_size()/4+3.
  string get_string(); // Return the name of the class.
  vector<Function *> *get_method() const;
  vector<AttrInfo *> *get_attribute() const;
  int get_attr_offset(string_view attribute) const;
  // Return the relative position of attribute, equivalent to distance in stdlib.
  ```

### Summary

If you have any questions about the API, please move to the issue for discussion. The `lightir` interface is designed and implemented by the assistant, and a lot of testing has been done.

**Please modify the code directly when you encounter problems, especially the GEP part, and if you think other students need your code, please submit a PR**.

**Note**: Since `PA3 bonus` needs to modify the `ir` structure directly, in order to facilitate the students' implementation, the teaching assistant will return most of the private variables (such as instruction chains, etc.) directly to the reference, so please pay attention to your own modification of the private variables.