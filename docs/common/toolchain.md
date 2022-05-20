# 编译器工具链介绍

## GNU

一般编译器分为三个stage: cc1,as,ld.

- 预处理器：cc1
- 编译器：cc1 →输出汇编代码 (* .s)
- 汇编程序：as → 输出目标文件 (* .o)
- 链接器：ld ([collect2](https://gcc.gnu.org/onlinedocs/gccint/Collect2.html) )→输出执行对象文件（a.out等） 在gcc版本 2 之前，预处理器是一个名为 cpp
  的命令。cpp 是预生产没有输出文件 .i 棒的扩展名作为过程的结果文件。从 gcc版本 3 开始，编译器cc1也用作预处理器。

从 collect2 调用链接器 ld。当实际删除 /usr/bin/ld 并运行 collect2 时，会收到类似 ld not found 的错误。

gcc放了-v 选项，gcc就可以看到处理的过程了。

```bash
root@node2:~/Downloads# gcc ./a.c -v
Using built-in specs.
COLLECT_GCC=gcc
COLLECT_LTO_WRAPPER=/usr/lib/gcc/x86_64-linux-gnu/10/lto-wrapper
OFFLOAD_TARGET_NAMES=nvptx-none:amdgcn-amdhsa:hsa
OFFLOAD_TARGET_DEFAULT=1
Target: x86_64-linux-gnu
Configured with: ../src/configure -v --with-pkgversion='Ubuntu 10.3.0-1ubuntu1~20.04' --with-bugurl=file:///usr/share/doc/gcc-10/README.Bugs --enable-languages=c,ada,c++,go,brig,d,fortran,objc,obj-c++,m2 --prefix=/usr --with-gcc-major-version-only --program-suffix=-10 --program-prefix=x86_64-linux-gnu- --enable-shared --enable-linker-build-id --libexecdir=/usr/lib --without-included-gettext --enable-threads=posix --libdir=/usr/lib --enable-nls --enable-bootstrap --enable-clocale=gnu --enable-libstdcxx-debug --enable-libstdcxx-time=yes --with-default-libstdcxx-abi=new --enable-gnu-unique-object --disable-vtable-verify --enable-plugin --enable-default-pie --with-system-zlib --enable-libphobos-checking=release --with-target-system-zlib=auto --enable-objc-gc=auto --enable-multiarch --disable-werror --with-arch-32=i686 --with-abi=m64 --with-multilib-list=m32,m64,mx32 --enable-multilib --with-tune=generic --enable-offload-targets=nvptx-none=/build/gcc-10-S4I5Pr/gcc-10-10.3.0/debian/tmp-nvptx/usr,amdgcn-amdhsa=/build/gcc-10-S4I5Pr/gcc-10-10.3.0/debian/tmp-gcn/usr,hsa --without-cuda-driver --enable-checking=release --build=x86_64-linux-gnu --host=x86_64-linux-gnu --target=x86_64-linux-gnu --with-build-config=bootstrap-lto-lean --enable-link-mutex
Thread model: posix
Supported LTO compression algorithms: zlib zstd
gcc version 10.3.0 (Ubuntu 10.3.0-1ubuntu1~20.04)
COLLECT_GCC_OPTIONS='-v' '-mtune=generic' '-march=x86-64'
 /usr/lib/gcc/x86_64-linux-gnu/10/cc1 -quiet -v -imultiarch x86_64-linux-gnu ./a.c -quiet -dumpbase a.c -mtune=generic -march=x86-64 -auxbase a -version -fasynchronous-unwind-tables -fstack-protector-strong -Wformat -Wformat-security -fstack-clash-protection -fcf-protection -o /tmp/ccq3lrsa.s
GNU C17 (Ubuntu 10.3.0-1ubuntu1~20.04) version 10.3.0 (x86_64-linux-gnu)
	compiled by GNU C version 10.3.0, GMP version 6.2.0, MPFR version 4.0.2, MPC version 1.1.0, isl version isl-0.22.1-GMP

GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
ignoring nonexistent directory "/usr/local/include/x86_64-linux-gnu"
ignoring nonexistent directory "/usr/lib/gcc/x86_64-linux-gnu/10/include-fixed"
ignoring nonexistent directory "/usr/lib/gcc/x86_64-linux-gnu/10/../../../../x86_64-linux-gnu/include"
#include "..." search starts here:
#include <...> search starts here:
 /usr/lib/gcc/x86_64-linux-gnu/10/include
 /usr/local/include
 /usr/include/x86_64-linux-gnu
 /usr/include
End of search list.
GNU C17 (Ubuntu 10.3.0-1ubuntu1~20.04) version 10.3.0 (x86_64-linux-gnu)
	compiled by GNU C version 10.3.0, GMP version 6.2.0, MPFR version 4.0.2, MPC version 1.1.0, isl version isl-0.22.1-GMP

GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
Compiler executable checksum: f2dc2f7bd2244142e4cf7a4d43b693bf
COLLECT_GCC_OPTIONS='-v' '-mtune=generic' '-march=x86-64'
 as -v --64 -o /tmp/ccRKbQ88.o /tmp/ccq3lrsa.s
GNU assembler version 2.34 (x86_64-linux-gnu) using BFD version (GNU Binutils for Ubuntu) 2.34
COMPILER_PATH=/usr/lib/gcc/x86_64-linux-gnu/10/:/usr/lib/gcc/x86_64-linux-gnu/10/:/usr/lib/gcc/x86_64-linux-gnu/:/usr/lib/gcc/x86_64-linux-gnu/10/:/usr/lib/gcc/x86_64-linux-gnu/
LIBRARY_PATH=/usr/lib/gcc/x86_64-linux-gnu/10/:/usr/lib/gcc/x86_64-linux-gnu/10/../../../x86_64-linux-gnu/:/usr/lib/gcc/x86_64-linux-gnu/10/../../../../lib/:/lib/x86_64-linux-gnu/:/lib/../lib/:/usr/lib/x86_64-linux-gnu/:/usr/lib/../lib/:/usr/lib/gcc/x86_64-linux-gnu/10/../../../:/lib/:/usr/lib/
COLLECT_GCC_OPTIONS='-v' '-mtune=generic' '-march=x86-64'
 /usr/lib/gcc/x86_64-linux-gnu/10/collect2 -plugin /usr/lib/gcc/x86_64-linux-gnu/10/liblto_plugin.so -plugin-opt=/usr/lib/gcc/x86_64-linux-gnu/10/lto-wrapper -plugin-opt=-fresolution=/tmp/ccYzFpGa.res -plugin-opt=-pass-through=-lgcc -plugin-opt=-pass-through=-lgcc_s -plugin-opt=-pass-through=-lc -plugin-opt=-pass-through=-lgcc -plugin-opt=-pass-through=-lgcc_s --build-id --eh-frame-hdr -m elf_x86_64 --hash-style=gnu --as-needed -dynamic-linker /lib64/ld-linux-x86-64.so.2 -pie -z now -z relro /usr/lib/gcc/x86_64-linux-gnu/10/../../../x86_64-linux-gnu/Scrt1.o /usr/lib/gcc/x86_64-linux-gnu/10/../../../x86_64-linux-gnu/crti.o /usr/lib/gcc/x86_64-linux-gnu/10/crtbeginS.o -L/usr/lib/gcc/x86_64-linux-gnu/10 -L/usr/lib/gcc/x86_64-linux-gnu/10/../../../x86_64-linux-gnu -L/usr/lib/gcc/x86_64-linux-gnu/10/../../../../lib -L/lib/x86_64-linux-gnu -L/lib/../lib -L/usr/lib/x86_64-linux-gnu -L/usr/lib/../lib -L/usr/lib/gcc/x86_64-linux-gnu/10/../../.. /tmp/ccRKbQ88.o -lgcc --push-state --as-needed -lgcc_s --pop-state -lc -lgcc --push-state --as-needed -lgcc_s --pop-state /usr/lib/gcc/x86_64-linux-gnu/10/crtendS.o /usr/lib/gcc/x86_64-linux-gnu/10/../../../x86_64-linux-gnu/crtn.o
COLLECT_GCC_OPTIONS='-v' '-mtune=generic' '-march=x86-64'
```

### riscv-gnu-toolchain

LLVM的链接器只在linux上有完整实现，所以为了本编译器的跨平台测试，仍然使用LLVM编译`.ll`文件，gcc链接器后端。有关riscv链接器的dark
side，详见[MaskRay](https://zhuanlan.zhihu.com/p/357112171)

## LLVM

LLVM也有对应的一套工具链。对于 GCC，可以仅使用宿主 C++ 编译器（通常是 GCC）从 Binutils 和 GCC 源代码构建完整的 C/C++ 交叉编译器。Clang 依赖于 GCC，并且您无法仅使用 LLVM
项目的源代码构建完整的交叉编译器（因此您必须按原样使用 GCC 头文件和库）。这是因为CMake需要一个完整的C/C++编译器（可以生成a.out），除此之外，还需要在Clang本身上构建Clang的运行时库compiler-rt。如果使用
GCC 构建甚至一个库，该库将依赖于 GCC 头文件，因此所有其他库都必须使用相同的 GCC 构建。

```bash
Apple clang version 13.0.0 (clang-1300.0.29.30)
Target: x86_64-apple-darwin21.1.0
Thread model: posix
InstalledDir: /Library/Developer/CommandLineTools/usr/bin
"/Library/Developer/CommandLineTools/usr/bin/clang" -cc1 -triple x86_64-apple-macosx12.0.0 -Wundef-prefix=TARGET_OS_ -Wdeprecated-objc-isa-usage -Werror=deprecated-objc-isa-usage -Werror=implicit-function-declaration -emit-obj -mrelax-all --mrelax-relocations -disable-free -disable-llvm-verifier -discard-value-names -main-file-name a.c -mrelocation-model pic -pic-level 2 -mframe-pointer=all -fno-strict-return -fno-rounding-math -munwind-tables -target-sdk-version=12.1 -fvisibility-inlines-hidden-static-local-var -target-cpu penryn -tune-cpu generic -debugger-tuning=lldb -target-linker-version 711 -v -resource-dir /Library/Developer/CommandLineTools/usr/lib/clang/13.0.0 -isysroot /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk -I/usr/local/include -internal-isystem /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/local/include -internal-isystem /Library/Developer/CommandLineTools/usr/lib/clang/13.0.0/include -internal-externc-isystem /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include -internal-externc-isystem /Library/Developer/CommandLineTools/usr/include -Wno-reorder-init-list -Wno-implicit-int-float-conversion -Wno-c99-designator -Wno-final-dtor-non-final-class -Wno-extra-semi-stmt -Wno-misleading-indentation -Wno-quoted-include-in-framework-header -Wno-implicit-fallthrough -Wno-enum-enum-conversion -Wno-enum-float-conversion -Wno-elaborated-enum-base -fdebug-compilation-dir /Users/yiweiyang/project/hpl-ai -ferror-limit 19 -stack-protector 1 -fstack-check -mdarwin-stkchk-strong-link -fblocks -fencode-extended-block-signature -fregister-global-dtors-with-atexit -fgnuc-version=4.2.1 -fmax-type-align=16 -fcommon -fcolor-diagnostics -clang-vendor-feature=+nullptrToBoolConversion -clang-vendor-feature=+messageToSelfInClassMethodIdReturnType -clang-vendor-feature=+disableInferNewAvailabilityFromInit -clang-vendor-feature=+disableNeonImmediateRangeCheck -clang-vendor-feature=+disableNonDependentMemberExprInCurrentInstantiation -fno-odr-hash-protocols -clang-vendor-feature=+revert09abecef7bbf -mllvm -disable-aligned-alloc-awareness=1 -mllvm -enable-dse-memoryssa=0 -o /var/folders/1x/tx5s6n452zv5qkyx4n3xz58c0000gn/T/a-d764d3.o -x c a.c
clang -cc1 version 13.0.0 (clang-1300.0.29.30) default target x86_64-apple-darwin21.1.0
ignoring nonexistent directory "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/local/include"
ignoring nonexistent directory "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/Library/Frameworks"
#include "..." search starts here:
#include <...> search starts here:
/usr/local/include
/Library/Developer/CommandLineTools/usr/lib/clang/13.0.0/include
/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include
/Library/Developer/CommandLineTools/usr/include
/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/System/Library/Frameworks (framework directory)
End of search list.
"/Library/Developer/CommandLineTools/usr/bin/ld" -demangle -lto_library /Library/Developer/CommandLineTools/usr/lib/libLTO.dylib -no_deduplicate -dynamic -arch x86_64 -platform_version macos 12.0.0 12.1 -syslibroot /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk -o a.out -L/usr/local/lib /var/folders/1x/tx5s6n452zv5qkyx4n3xz58c0000gn/T/a-d764d3.o -lSystem /Library/Developer/CommandLineTools/usr/lib/clang/13.0.0/lib/darwin/libclang_rt.osx.a
```

在不同的编译器之间需要考虑ABI兼容的问题。 相比之下

```c++
$A$prototype:
.word	4                               # 0x4
.word	4                               # 0x4
.word	($A$dispatchTable)
.word	42                              # 0x2a
.size	$A$prototype, 16

	.type	$A$dispatchTable,@object        # @"$A$dispatchTable"
	.section	.sdata,"aw",@progbits
	.globl	$A$dispatchTable
	.p2align	3
```

## Venus

在体系结构课上使用的是Venus做模拟，使用的ecall遵循spim的规则，其call convention如下

| ID (`a0`) | Name            | Description                                                                                                                                                                                                                                                                                                                                                                                     |
|-----------|-----------------|-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| 1         | print_int       | prints integer in `a1`                                                                                                                                                                                                                                                                                                                                                                          |
| 4         | print_string    | prints the null-terminated string whose address is in `a1`                                                                                                                                                                                                                                                                                                                                      |
| 9         | sbrk            | allocates `a1` bytes on the heap, returns pointer to start in `a0`                                                                                                                                                                                                                                                                                                                              |
| 10        | exit            | ends the program                                                                                                                                                                                                                                                                                                                                                                                |
| 11        | print_character | prints ASCII character in `a1`                                                                                                                                                                                                                                                                                                                                                                  |
| 13        | openFile        | Opens the file in the VFS where a pointer to the path is in `a1` and the permission bits are in `a2`. Returns to a0 an integer representing the file descriptor.                                                                                                                                                                                                                                |
| 14        | readFile        | Takes in: `a1` = FileDescriptor, `a2` = Where to store the data (an array), `a3` = the amount you want to read from the file. Returns `a0` = Number of items which were read and put to the given array. If it is less than `a3` it is either an error or EOF. You will have to use another ecall to determine what was the cause.                                                              |
| 15        | writeFile       | Takes in: `a1` = FileDescriptor, `a2` = Buffer to read data from, `a3` = amount of the buffer you want to read, `a4` = Size of each item. Returns `a0` = Number of items written. If it is less than `a3` it is either an error or EOF. You will have to use another ecall to determine what was the cause. Also, you need to flush or close the file for the changes to be written to the VFS. |
| 16        | closeFile       | Takes in: `a1` = FileDescriptor. Returns 0 on success and EOF (-1) otherwise. Will flush the data as well.                                                                                                                                                                                                                                                                                      |
| 17        | exit2           | ends the program with return code in `a1`                                                                                                                                                                                                                                                                                                                                                       |
| 18        | fflush          | Takes in: `a1` = FileDescriptor. Will return 0 on success otherwise EOF on an error.                                                                                                                                                                                                                                                                                                            |
| 19        | feof            | Takes in: `a1` = FileDescriptor. Returns a nonzero value when the end of file is reached otherwise, it returns 0.                                                                                                                                                                                                                                                                               |
| 20        | ferror          | Takes in: `a1` = FileDescriptor. Returns Nnnzero value if the file stream has errors occurred, ​0​ otherwise.                                                                                                                                                                                                                                                                                   |
| 34        | printHex        | prints hex in `a1`                                                                                                                                                                                                                                                                                                                                                                              |
| 0x3CC     | vlib            | Please check out the [vlib page](Venus's-optimized-clib) to see what functions you can use!                                                                                                                                                                                                                                                                                                     |

以下是使用方法

```asm
li a0, 1    # syscall number for printing integer
li a1, 1024 # the integer we're printing
ecall       # issue system call
```

`a0`为syscall参数，决定是哪个系统调用，`a1`为系统调用的参数。由于只有几个系统调用，所以可以用`li`指令来构造参数，且只有一个寄存器传参。 linux下的系统调用很多，其call
convention如下，可以最多支持6个参数传递，再多就只能放栈上了。

```asm
      Arch/ABI    Instruction           System  Ret  Ret  Error    Notes
                                         call #  val  val2
       ───────────────────────────────────────────────────────────────────
       alpha       callsys               v0      v0   a4   a3       1, 6
       arc         trap0                 r8      r0   -    -
       arm/OABI    swi NR                -       r0   -    -        2
       arm/EABI    swi 0x0               r7      r0   r1   -
       arm64       svc #0                w8      x0   x1   -
       blackfin    excpt 0x0             P0      R0   -    -
       i386        int $0x80             eax     eax  edx  -
       ia64        break 0x100000        r15     r8   r9   r10      1, 6
       m68k        trap #0               d0      d0   -    -
       microblaze  brki r14,8            r12     r3   -    -
       mips        syscall               v0      v0   v1   a3       1, 6
       nios2       trap                  r2      r2   -    r7
       parisc      ble 0x100(%sr2, %r0)  r20     r28  -    -
       powerpc     sc                    r0      r3   -    r0       1
       powerpc64   sc                    r0      r3   -    cr0.SO   1
       riscv       ecall                 a7      a0   a1   -
       s390        svc 0                 r1      r2   r3   -        3
       s390x       svc 0                 r1      r2   r3   -        3
       superh      trapa #31             r3      r0   r1   -        4, 6
       sparc/32    t 0x10                g1      o0   o1   psr/csr  1, 6
       sparc/64    t 0x6d                g1      o0   o1   psr/csr  1, 6
       tile        swint1                R10     R00  -    R01      1
       x86-64      syscall               rax     rax  rdx  -        5
       x32         syscall               rax     rax  rdx  -        5
       xtensa      syscall               a2      a2   -    -
```

## spike

[spike](https://github.com/riscv-software-src/riscv-isa-sim) 是完整的用户态CPU模拟器，同时支持Machine Mode/Hypervisor Mode/User
Mode的模拟，在启动的时候会自己通过定义分配用户态栈空间。系统调用、指令crash都会被记录，并通过openocd模拟jtag方式告诉用户。proxy
kernel是一个最简单的软核心，自带bootloader，可以通过bootloader后启动一个最小linux跑任意riscv elf文件，但是支持的syscall有限。

调试和编译方法如下，详细介绍请见官网。

```bash
riscv64-unknown-elf-gcc -march=rv32imac -mabi=ilp32 -g -Og -T spike.lds -nostartfiles -o [file] ./[file].c
spike --rbb-port=9824 -m0x10000000:0x20000 [file]

#another shell for probing the output and open port for debugging
openocd -f spike.cfg

#another shell for gdb
riscv64-unknown-elf-gdb [file]
-> target remote :3333
```

## qemu

qemu是模拟届的永远的神，在qemu出来以前，我们需要通过真的在体制好的CPU及外设上进行调试操作系统，且需要实时查看硬件错误码，而qemu完整的模拟了硬件的状态，新硬件出来时，硬件厂商会给予第一步支持。

### qemu-user-static

用户态模拟qemu，内部自带一个linux handler。
