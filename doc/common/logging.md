# logging 工具使用

## 介绍

为了方便同学们在之后的实验中 debug，为大家从GLOG那抄写了一个C++简单实用的分级日志工具，不过由于编译器限制暂时还没有`std::source_location`。该工具将日志输出信息从低到高分成四种等级：`DEBUG`，`INFO`，`WARNING`，`ERROR`。通过设定环境变量`LOGV`的值，来选择输出哪些等级的日志。`LOGV`的取值是**0～3**,分别对应到上述的4种级别(`0:DEBUG`,`1:INFO`,`2:WARNING`,`3:ERROR`)。此外输出中还会包含打印该日志的代码所在位置。

## 使用

项目编译好之后，可以在`build`目录下运行`test_logging`，该文件的源代码在`tests/test_logging.cpp`。用法如下：

```cpp
#include <chocopy_logging.hpp>
// 引入头文件
int main(){
    LOG(DEBUG) << "This is DEBUG log item.";
    // 使用关键字LOG，括号中填入要输出的日志等级
    // 紧接着就是<<以及日志的具体信息，就跟使用std::cout一样
    LOG(INFO) << "This is INFO log item";
    LOG(WARNING) << "This is WARNING log item";
    LOG(ERROR) << "This is ERROR log item";
    return 0;
}
```

接着在运行该程序的时候，设定环境变量`LOGV=0`，那么程序就会输出级别**大于等于0**日志信息：

```bash
user@user:${ProjectDir}/build$ LOGV=0 ./test_logging
[DEBUG] (test_logging.cpp:5L  main)This is DEBUG log item.
[INFO] (test_logging.cpp:6L  main)This is INFO log item
[WARNING] (test_logging.cpp:7L  main)This is WARNING log item
[ERROR] (test_logging.cpp:8L  main)This is ERROR log item
```

输出中除了包含日志级别和用户想打印的信息，在圆括号中还包含了打印该信息代码的具体位置（包括文件名称、所在行、所在函数名称），可以很方便地定位到出问题的地方。

假如我们觉得程序已经没有问题了，不想看那么多的DEBUG信息，那么我们就可以设定环境变量`LOGV=1`，选择只看**级别大于等于1**的日志信息：

```bash
user@user:${ProjectDir}/build$ LOGV=0 ./test_logging
[INFO] (test_logging.cpp:6L  main)This is INFO log item
[WARNING] (test_logging.cpp:7L  main)This is WARNING log item
[ERROR] (test_logging.cpp:8L  main)This is ERROR log item
```

当然`LOGV`值越大，日志的信息将更加简略。如果没有设定`LOGV`的环境变量，将默认不输出任何信息。

这里再附带一个小技巧，如果日志内容多，在终端观看体验较差，可以输入以下命令将日志输出到文件中：

```
user@user:${ProjectDir}/build$ LOGV=0 ./test_logging > log
```

然后就可以输出到文件名为log的文件中啦～

## 已知Bug

1. 在 M1 上用 gcc-11 编译的 fmt::format 库中使用双重指针会 Seg Fault。
2. 在 Windows Powershell 中可能无法现实颜色，可以去除颜色标识或尝试用 `$env:TERM=xterm`。