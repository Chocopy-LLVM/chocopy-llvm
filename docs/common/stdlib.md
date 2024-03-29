# ChocoPy stdlib

<!-- TOC -->

- [ChocoPy stdlib](#chocopy-stdlib)
  - [Assembler definition](#assembler-definition)
    - [.equiv](#equiv)
    - [.word](#word)
    - [.align](#align)
    - [.string](#string)
    - [.space](#space)
    - [Initialize global variable definitions](#initialize-global-variable-definitions)
  - [Memory management](#memory-management)
    - [init heap at program startup](#init-heap-at-program-startup)
    - [alloc/alloc2](#allocalloc2)
    - [IO](#io)
    - [input](#input)
    - [print](#print)
  - [int/bool](#intbool)
    - [makeint/bool](#makeintbool)
  - [list](#list)
    - [conslist](#conslist)
    - [len](#len)
  - [str](#str)
    - [makestr](#makestr)
    - [len](#len-1)
  - [class](#class)
    - [`object.__init__`](#object__init__)
  - [Error](#error)
  - [Bonus Part](#bonus-part)
    - [Multi-threaded part](#multi-threaded-part)
      - [find](#find)
      - [wrap](#wrap)
      - [accstart](#accstart)
      - [accend](#accend)


<!-- /TOC -->

Due to the need to design the intermediate layer Light IR, it cannot be defined in the ChocoPy source code, but is located in [stdlib](../../src/cgen/stdlib/). It needs to be abstracted to a function that can be called with a c calling convention pass-through. Since Berkeley's system abi on Venus is inconsistent, I designed an additional set of stdlib to run on qemu. at compile time, the riscv compiler will compile stdlib to the static link library first.

## Assembler definition
### .equiv 
Since only RV32IMAC and Venus use `.equiv` for variable substitution, cgen will only output `.equiv` if the environment variable `EMIT` is present. 
### .word
The linker resolves itself to the address of the word label, usually storing int literal and pointer.
### .align
We have added support for specifying byte alignment in data segments. The supported syntax is .align $langle n\rangle$, which inserts zero-valued bytes as padding so that the next available address is a multiple of $2^{n}$.
### .string 
is the equivalent of .ascii
### .space 
The ascii equivalent of '\n'.
### Initialize global variable definitions
- const_0 bool false
- const_1 bool true
- const_2 string "Invalid argument"
- const_3 string "True"
- const_4 string "False"
- const_5 string "Out of memory"
- const_6 string "Operation on None"
- const_7 string "Division by zero"
- const_8 string "Index out of bounds"

## Memory management
### init heap at program startup
Default startup allocates `8192 << 16` memory, call `heap.init` in stdlib, i.e. 32MB. venus implements `ecall` when `a0` is 9, linux implements `ecall` when `a0` is 9, put registers in ``
### alloc/alloc2
Request memory from the OS via a system call and return the requested memory.
### IO
### input
`myscanf` reads stdin and determines if the input is int/bool/str, automatically calls the corresponding function to initialize the variable, and finally returns the respective prototype.

### print

`myprintf` will output stdout, and determine the output int/bool/str type.

## int/bool
### makeint/bool
The function is defined as `%$int/bool$prototype_type* @makeint(i32)`, giving an i32 and returning a created int prototype. implementation requires alloc memory.
## list

### conslist
function defined as `%$.list$prototype_type* @conslist(i32, %$union.conslist, ...) `, `i32` is the number of elements, the second argument determines what type of list it is, and initializes the corresponding list. all lists are dynamically called conslist. For arrays without arguments or call intrinsic function, `conslist` is not needed to get prototype by default, thus improving performance.

### len
function is defined as `i32 @$len(%$union.len*)`, can accept list or str types, reads `__len__` attributes and returns i32.

## str

### makestr
Function defined as `%$str$prototype_type* @makestr(%i8*)` which takes i8* and puts it inside `$str$prototype_type`.
### len
Consistent with the list part.

## class

### `object.__init__`
is a placeholder.
## Error

Will output an error message and call abort to exit the program, equivalent to `exit(-1)`.
- Div except 0 error, output `const_7`
- None except 0 error, output `const_6`.
- OOB except 0 error, output `const_8`

## Bonus Part
### Multi-threaded part
Specific procedure: find + wrap

#### find

There are many conditions for find, in short, find the outermost loop of each innermost loop, then see if it can be `MultiThreading`ed

The conditions that this outer loop should satisfy are

* need a start-to-end incremental pattern
* The branch instruction should be of the form `Br Lt i32 Const <Label1> <Label2>`.
* There can be no other Phi instructions (fans, which limits to one loop variable)


In the matrix assignment example, the br is based on the relationship between the magnitude of i and n

Phi's four operands

1. i, incremental variables
2. BB1, the BB that increments i
3. constant, 100, the loop boundary
4. BB2, %entry


Accumulated values.

```c++
auto accuVal = dynamic_cast<ConstantInt *>(accu->getOperand(1));
```

#### wrap

Wrapping section: insert ACCstart before the conditional block of the loop, and insert ACCend before the update iteration variable

```c
while (i<n){
	A[i] = A[i]+B[i];
}
```

Before instrumentation

![](0.svg)

After instrumentation

```llvm
%37: thread id tid
%38: size of problem n
%40: _l = (n*tid)/4 
%43: _r = (n*(tid+1))/4
%44: _r + start		右边界
%48: (_l + step-1)/step * step + start iterable starting number
```

![](1.svg)
```llvm
define @main()
<label>entry:   level=0   preds=   succs=(%13)
    REG 4    [ 100 x i32]* %34  = Alloca 
    REG 1    [ 100 x i32]* %35  = Alloca 
    REG 0    i32 %2  = Call getarray [ 100 x i32]* %35 
    REG 0    i32 %4  = Call getarray [ 100 x i32]* %34 
    REG 10    i32 %37  = Call accstart 
 MT REG 2    i32 %39  = Mul i32 10 i32 %37 
 MT REG 0    i32 %41  = Add i32 %37 i32 1 
 MT REG 0    i32 %42  = Mul i32 10 i32 %41 
 MT REG 5    i32 %50  = AShr i32 %42 i32 2 
 MT REG 0    i32 %51  = AShr i32 %39 i32 2 
 MT REG 0    i32 %52  = Shl i32 %51 i32 0 
 MT          Br <label> %13 
<label>13:   level=1   preds=(%17 %entry)   succs=(%17 %MutithreadEnd0)
 MT REG 0    i32 %36  = PHI i32 %31 <label> %17 i32 %52 <label> %entry 
 MT          Br LT i32 %36 i32 %50 <label> %17 <label> %MutithreadEnd0 
<label>17:   level=1   preds=(%13)   succs=(%13)
 MT REG 3    i32 %24  = Load [ 100 x i32]* %35 i32 %36 i32 2 
 MT REG 2    i32 %28  = Load [ 100 x i32]* %34 i32 %36 i32 2 
 MT REG 2    i32 %29  = Add i32 %24 i32 %28 
 MT          Store i32 %29 [ 100 x i32]* %35 i32 %36 i32 2 
 MT REG 0    i32 %31  = Add i32 %36 i32 1 
 MT          Br <label> %13 
<label>MutithreadEnd0:   level=0   preds=(%13)   succs=
             Call accend i32 %37 
             ret i32 0 
```

First know the system call convention

```bash
> man syscall 2
The first table lists the instruction used to transition to kernel
mode, the register used to indicate the system call number, the 
register used to return the sys‐tem call result, and the register 
used to signal an error.

arch/ABI    instruction           syscall #  retval  error    Notes
────────────────────────────────────────────────────────────────────
arc         trap0                 r8         r0      -
arm/OABI    swi NR                -          a1      -        [2]
arm/EABI    swi 0x0               r7         r0      -
arm64       svc #0                x8         x0      -
riscv       ecall                 a7         a0      a1        -

Note:
[2] NR is the system call number.

The second table shows the registers used to pass the system call
arguments.

arch/ABI      arg1  arg2  arg3  arg4  arg5  arg6  arg7  Notes
──────────────────────────────────────────────────────────────
arc           r0    r1    r2    r3    r4    r5    -
arm/OABI      a1    a2    a3    a4    v1    v2    v3
arm/EABI      r0    r1    r2    r3    r4    r5    r6
arm64         x0    x1    x2    x3    x4    x5    -
riscv         a0    a1    a2    a3    a4    a5    -
```

Then `man clone 2`

```c
int clone(int (*fn)(void *), void *stack, int flags, void *arg, ...
          /* pid_t *parent_tid, void *tls, pid_t *child_tid */ );
```

Linux design for threads: threads are nothing more than processes that share virtual address space and file descriptor tables, and by definition share everything between threads except registers, stacks, and thread-local storage (TLS).

The OS and underlying hardware naturally ensure that threads do not share registers, TLS is not required, and this project makes it possible to share stack space between threads, so there is almost no switching overhead

clone and fork both create child processes (threads), the difference is that clone can customize the content shared by the child and parent processes in more detail: virtual memory address space, file descriptor table, the table of signal handlers

clone returns the tid of the created child process to the parent process, or -1 if it fails.

The length of `clone_flags` is 8 bytes, after subtracting one byte of the signal code sent to the parent process, there are 7 bytes left to pass process copy information, i.e. 7*8=56 flags can be set at the same time, i.e. 24 kinds of process information can be copied at the same time. To set a certain flag bit, you only need to set the corresponding value with `clone_flags` or.

Why is clone_flags 273?

The 32-bit binary of 273 is 000000000100010001, hexadecimal 0x0111

[linux 5.17 sched.h](https://elixir.bootlin.com/linux/v5.17/source/include/uapi/asm-generic/signal.h#L18)

```c
/*
 * cloning flags:
 */
#define CSIGNAL		0x000000ff	/* signal mask to be sent at exit */
#define CLONE_VM	0x00000100	/* set if VM shared between processes */
#define CLONE_FS	0x00000200	/* set if fs info shared between processes */
#define CLONE_FILES	0x00000400	/* set if open files shared between processes */
/*....*/
#define CLONE_NEWUSER		0x10000000	/* New user namespace */
#define CLONE_NEWPID		0x20000000	/* New pid namespace */
#define CLONE_NEWNET		0x40000000	/* New network namespace */
#define CLONE_IO		0x80000000	/* Clone io context */
```

在 [kernel/fork.c](https://elixir.bootlin.com/linux/v5.17/source/kernel/fork.c#L2502)

```c
/* ok, now we should be set up.. */
p->exit_signal = (clone_flags & CLONE_THREAD) ? -1 : (clone_flags & CSIGNAL);
p->pdeath_signal = 0;
p->exit_state = 0;
```

As for why the last two numbers are 17

Defined in [include/uapi/asm-generic/signal.h](https://elixir.bootlin.com/linux/v5.17/source/include/uapi/asm-generic/signal.h#L18) 32 semaphores

```c
#define SIGHUP		 1
#define SIGINT		 2
#define SIGQUIT		 3
#define SIGILL		 4
#define SIGTRAP		 5
#define SIGABRT		 6
#define SIGIOT		 6
#define SIGBUS		 7
#define SIGFPE		 8
#define SIGKILL		 9
#define SIGUSR1		10
#define SIGSEGV		11
#define SIGUSR2		12
#define SIGPIPE		13
#define SIGALRM		14
#define SIGTERM		15
#define SIGSTKFLT	16
#define SIGCHLD		17
//...
```

Project default off multi-threaded optimization Pass

Advantages compared to the classic framework OpenMP

* Saves stack overhead: no need to split out the parallelized region into functions
* Saving on switching contexts: no need to save contexts and maintain various information
* Only thread-unique registers need to be maintained
#### accstart

```llvm
# Runtime support function accstart
  mv fp, sp
  add t1, zero, zero
  lui t1, 1044480
  add sp, sp, t1
  add sp, sp, -12
  sw  a0, 0(sp)
  sw  a1, 4(sp)
  sw  a2, 8(sp)
  sw  a3, 12(sp)
  mv a3, a7
  addi a2, zero, 4
accstart_1:
  addi a2, a2, -1
  beqz a2, accstart_2
  addi a7, zero, 120
  addi a0, zero, 20
  mv a1, sp
  ecall
  bnez a0, accstart_1
accstart_2:
  mv tp, a2
  mv a7, a3
  lw  a0, 0(sp)
  lw  a1, 4(sp)
  lw  a2, 8(sp)
  lw  a3, 12(sp)
  add sp, sp, 12
  add s1, zero, zero
  lui s1, 4096
  add sp, sp, s1
  ret
```

#### accend

```llvm
# Runtime support function accend
  beqz s1, .accend_2
.accend_1:
  call exit
.accend_2:
  addi sp, sp, -16
  sw  a0, 0(sp)
  sw  a1, 4(sp)
  sw  a2, 8(sp)
  sw  a3, 12(sp)
  addi a1, zero, 4
.accend_3:
  addi a1, a1, -1
  beqz a1, .accend_4
  addi sp, sp, -8
  sw  a1, 0(sp)
  sw  ra, 4(sp)
  addi sp, sp, -4
  mv a0, sp
  jal wait
  addi sp, sp, 4
  lw  a1, 0(sp)
  lw  ra, 4(sp)
  addi sp, sp, 8
  j .accend_3
.accend_4:
  lw  a0, 0(sp)
  lw  a1, 4(sp)
  lw  a2, 8(sp)
  lw  a3, 12(sp)
  addi sp, sp, 16
  mv s1, zero
  ret
```