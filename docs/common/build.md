# How do I compile?

If there are any compilation problems that you can't solve, please solve them yourself. The environment is an introductory course for computer students, and really OH can provide answers.

## Compile LLVM with `--target=riscv64-unknown-linux-gnu` compiler
windows can refer to [Windows](https://www.emdalo.com/posts/risc-v-gnu-compiler-toolchain-howto-compile-on-windows/) , the rest of the OS can refer to [*nix](http://blog.kmckk.com/archives/5774305.html)

```bash
set -x -e

[ -z $TOP ] && export TOP=$PWD/riscv64

[ -z $PARA ] && export PARA=-j32

INSTALL64=$TOP/install64
BUILD64=$TOP/build64
export PATH=$INSTALL64/bin/:$PATH
export ARCH64="rv64gcv"
export CLANG_ARCH64="${ARCH64}0p10 -menable-experimental-extensions"
export ABI64=lp64d
export TARGET64=riscv64-unknown-elf
export CLANG_CFLAGS64="--target=${TARGET64} -march=${CLANG_ARCH64} -mabi=${ABI64}"

SRC=$PWD/src

mkdir -p $SRC
cd $SRC

git clone --depth 1 -b rvv-1.0.x-zfh https://github.com/riscv/riscv-binutils-gdb.git

wget ftp://sourceware.org/pub/newlib/newlib-4.1.0.tar.gz
tar xvf newlib-4.1.0.tar.gz

git clone --depth 1 https://github.com/llvm/llvm-project.git

mkdir -p $INSTALL64 $BUILD64

cd $BUILD64

mkdir binutils
cd binutils/
$SRC/riscv-binutils-gdb/configure --prefix=$INSTALL64 --target=$TARGET64 --enable-lto --disable-werror --disable-shared --disable-nls --with-sysroot=yes
make $PARA && make install
cd $BUILD64

mkdir clang
cd clang/
cmake -DLLVM_ENABLE_PROJECTS="clang;lld" \
      -G "Unix Makefiles" $SRC/llvm-project/llvm \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_INSTALL_PREFIX=$INSTALL64 \
      -DLLVM_TARGETS_TO_BUILD="RISCV" \
      -DLLVM_DEFAULT_TARGET_TRIPLE=$TARGET64 \
      -DLLVM_BUILD_EXAMPLES=OFF \
      -DLLVM_INCLUDE_EXAMPLES=OFF \
      -DBUILD_SHARED_LIBS=OFF \
      -DLLVM_OPTIMIZED_TABLEGEN=ON \
      -DLLVM_ENABLE_LIBXML2=OFF \
      -DCLANG_DEFAULT_RTLIB=compiler-rt \
      -DCLANG_DEFAULT_UNWINDLIB=libunwind \
      -DCLANG_DEFAULT_CXX_STDLIB=libc++

make $PARA && make install
cp bin/* $INSTALL64/bin
cd $BUILD64

ARGSTR='"$@"'
echo "${INSTALL64}/bin/clang ${CLANG_CFLAGS64} -Wno-unused-command-line-argument ${ARGSTR}" > $INSTALL64/bin/riscv64-unknown-elf-clang
echo "${INSTALL64}/bin/clang++ ${CLANG_CFLAGS64} -Wno-unused-command-line-argument ${ARGSTR}" > $INSTALL64/bin/riscv64-unknown-elf-clang++
chmod +x $INSTALL64/bin/$TARGET64-clang
chmod +x $INSTALL64/bin/$TARGET64-clang++

# stdlib.h is required to build compiler-rt, so we need to build newlib first.

mkdir newlib
cd newlib

export CFLAGS_FOR_TARGET=" -g -gdwarf-3 -gstrict-dwarf -O2 -ffunction-sections -fdata-sections "
export CC_FOR_TARGET=$TARGET64-clang
export AS_FOR_TARGET=$TARGET64-clang
export LD_FOR_TARGET=lld
export CXX_FOR_TARGET=$TARGET64-clang++
export AR_FOR_TARGET=llvm-ar
export NM_FOR_TARGET=llvm-nm
export RANLIB_FOR_TARGET=llvm-ranlib
export OBJCOPY_FOR_TARGET=llvm-objcopy
export OBJDUMP_FOR_TARGET=llvm-objdump
export READELF_FOR_TARGET=llvm-readelf
export STRIP_FOR_TARGET=llvm-strip
export LIPO_FOR_TARGET=llvm-lipo
export DLLTOOL_FOR_TARGET=llvm-dlltool

$SRC/newlib-4.1.0/configure --prefix=$INSTALL64 --target=$TARGET64 --disable-multilib --disable-nls
make $PARA all && make install
cd $BUILD64

mkdir libcompiler_rt
cd libcompiler_rt

export CC="${INSTALL64}/bin/${TARGET64}-clang"
export CXX="${INSTALL64}/bin/${TARGET64}-clang++"
export AR="${INSTALL64}/bin/llvm-ar"
export NM="${INSTALL64}/bin/llvm-nm"
export RANLIB="${INSTALL64}/bin/llvm-ranlib"
export OBJCOPY="${INSTALL64}/bin/llvm-objcopy"
export LLVM_CONFIG="${INSTALL64}/bin/llvm-config"

TARGET_CFLAGS=""
TARGET_CXXFLAGS="${TARGET_CFLAGS}"
TARGET_LDFLAGS=""

LLVM_VERSION=`$CC -dumpversion`
LLVM_RESOURCEDIR=/lib/clang/$LLVM_VERSION

cmake -G "Unix Makefiles" $SRC/llvm-project/compiler-rt \
    -DCMAKE_INSTALL_PREFIX=$INSTALL64/$LLVM_RESOURCEDIR/ \
    -DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY \
    -DCMAKE_CROSSCOMPILING=True \
    -DCMAKE_SYSTEM_NAME=Linux \
    -DCMAKE_BUILD_TYPE=Release \
    -DCOMPILER_RT_BUILD_BUILTINS=ON \
    -DCOMPILER_RT_BUILD_SANITIZERS=OFF \
    -DCOMPILER_RT_BUILD_XRAY=OFF \
    -DCOMPILER_RT_BUILD_LIBFUZZER=OFF \
    -DCOMPILER_RT_BUILD_PROFILE=OFF \
    -DCOMPILER_RT_BUILD_MEMPROF=OFF \
    -DCOMPILER_RT_BUILD_XRAY_NO_PREINIT=OFF \
    -DCOMPILER_RT_SANITIZERS_TO_BUILD=none \
    -DCMAKE_C_COMPILER=$CC \
    -DCMAKE_CXX_COMPILER=$CXX \
    -DCMAKE_AR=$AR \
    -DCMAKE_NM=$NM \
    -DCMAKE_RANLIB=$RANLIB \
    -DLLVM_CONFIG_PATH=$LLVM_CONFIG \
    -DCMAKE_C_COMPILER_TARGET=$TARGET64 \
    -DCOMPILER_RT_DEFAULT_TARGET_ONLY=ON \
    -DCMAKE_C_FLAGS="${TARGET_CFLAGS}" \
    -DCMAKE_CXX_FLAGS="${TARGET_CXXFLAGS}" \
    -DCMAKE_EXE_LINKER_FLAGS="${TARGET_LDFLAGS}" \
    -DCOMPILER_RT_BAREMETAL_BUILD=ON \
    -DCOMPILER_RT_OS_DIR=""

make $PARA && make install
cd $BUILD64
```

## Compile qemu with rvv

Works on Linux only.

```bash
git clone https://github.com/sifive/qemu -b v5.2.0-rvv-rvb-zfh
cd qemu
mkdir build
cd build
../configure --target-list=riscv64-softmmu,riscv32-softmmu,riscv64-linux-user,riscv32-linux-user --enable-kvm 
./qemu-riscv32 -cpu rv32,x-v=true,x-k=true,vlen=256,elen=64,vext_spec=v1.0 -g 1234 [file] <-在1234端口开启gdb-server
```

## Docker


This method is common to all platforms and already has all compiled binaries internally.

`docker pull yangyw12345/cs131-grader`

Can be developed in `VSCode`, need to install plugins in docker Install plugins `Docker`, `CMake`, `C/C++ Extension`, other configurations [reference](https://b23.tv/84juhvu).

![img.png](img.png)

## Windows

**Note: Lab3 Bonus is not suitable for Windows, you can compile riscv-gnu-toolchains by yourself if forced to do so**

First you need to install `choco`, and `everything` is recommended for file search.

By `choco install msys2 winflexbison cmake`.

Put `win_flex` and `pacman` in the windows environment variable

`pacman -Syuu mingw-w64-riscv64-unknown-elf-gcc`

Put the directory where `riscv64-unknown-elf-g++` is located into the windows environment variable

LLVM is compiled in the same way as above.

You can choose `CLion` or `VSCode` or `VS`. lab4 spike may require the use of wsl.

``bash
mkdir build
cd build
cmake ... -DLLVM_DIR=/path/to/llvm-config.cmake -DCMAKE_TOOLCHAIN_FILE=[vcpkg-root]/scripts/buildsystems/vcpkg.cmake -DBISON_EXCUTABLE=/path/ to/win_bison.exe -DFLEX_EXCUTABLE=/path/to/win_flex.exe
```

## MacOS

Suggested use ``CLion``

```bash
brew tap riscv/riscv
cd $(brew --prefix)/Library/Taps/riscv/homebrew-riscv
git remote-set-url origin https://github.com/victoryang00/homebrew-riscv.git 
git pull
brew install riscv-tools fmt cJSON
mkdir build
cd build
cmake ... -DLLVM_DIR=/path/to/llvm-config.cmake 
```

## Ubuntu or WSL

``CLion`` is recommended

```bash
apt install flex bison g++-riscv64-unknow-elf-gnu libfmt-dev libcjson-dev llvm-dev
mkdir build
cd build
cmake ... -DLLVM_DIR=/path/to/llvm-config.cmake 
```

## CLion's environment variable configuration
1. It is possible that {fmt} will appear as [segfault]() on a particular compiler, in which case we need to compile {fmt} ourselves, and in the CLion CMake interface, we need to configure
![img_2.png](img_2.png)
2. Lab3/4 involves the clang back-end compiler to verify your generated code, you need to put the generated clang in the environment variable, and you can add LOGV here.
![img_1.png](img_1.png)