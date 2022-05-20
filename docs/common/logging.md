# logging tool usage

## Introduction

In order to facilitate students to debug in later experiments, we have copied a simple and practical C++ hierarchical logging tool from GLOG, but due to compiler limitations there is no `std::source_location` for now. The tool divides the log output information into four levels from low to high: `DEBUG`, `INFO`, `WARNING`, and `ERROR`. You can choose which level of log output by setting the value of the environment variable `LOGV`. The values of `LOGV` are **0 to 3**, corresponding to the four levels mentioned above (`0:DEBUG`, `1:INFO`, `2:WARNING`, `3:ERROR`). In addition the output will contain the location of the code to print this log.

## Usage

After the project is compiled, you can run `test_logging` in the `build` directory, the source code of the file is in `tests/test_logging.cpp`. Use it as follows.

```cpp
#include <chocopy_logging.hpp>
// Introduce the header file
int main(){
    LOG(DEBUG) << "This is DEBUG log item;
    // Use the keyword LOG and fill in the brackets with the log level to be output
    // followed by << and the specific information about the log, just like with std::cout
    LOG(INFO) << "This is INFO log item";
    LOG(WARNING) << "This is WARNING log item";
    LOG(ERROR) << "This is ERROR log item";
    return 0;
}
```

Next, when you run the program, set the environment variable `LOGV=0`, then the program will output the level **greater than or equal to 0** log message.

```bash
user@user:${ProjectDir}/build$ LOGV=0 ./test_logging
[DEBUG] (test_logging.cpp:5L main)This is DEBUG log item.
[INFO] (test_logging.cpp:6L main)This is INFO log item
[WARNING] (test_logging.cpp:7L main)This is WARNING log item
[ERROR] (test_logging.cpp:8L main)This is ERROR log item
```

In addition to the log level and the information the user wants to print, the output also contains in parentheses the exact location of the code that printed the information (including the name of the file, the line, and the name of the function), making it easy to locate the problem.

If we feel that the program has no more problems and we don't want to see so many DEBUG messages, then we can set the environment variable `LOGV=1` and choose to see only log messages with **level greater than or equal to 1**: ```bash

```bash
user@user:${ProjectDir}/build$ LOGV=0 ./test_logging
[INFO] (test_logging.cpp:6L main)This is INFO log item
[WARNING] (test_logging.cpp:7L main)This is WARNING log item
[ERROR] (test_logging.cpp:8L main)This is ERROR log item
```

Of course the larger the `LOGV` value, the more abbreviated the log information will be. If the `LOGV` environment variable is not set, no information will be output by default.

Here's another tip, if the logs are very voluminous and the viewing experience in the terminal is poor, you can output the logs to a file by entering the following command.

```
user@user:${ProjectDir}/build$ LOGV=0 ./test_logging > log
```

Then you can output to a file with the file name log ~

## Known bugs

1. Using double pointers in the fmt::format library compiled with gcc-11 on M1 will Seg Fault.[solved by `-D_GLIBCXX_USE_CXX11_ABI=0`]
2. You may not get realistic colors in Windows Powershell, you can remove the color flag or try `$env:TERM=xterm`.