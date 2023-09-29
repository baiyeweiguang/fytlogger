# FYT日志系统 fyt_logger

copyright 邹承甫 2023

## 编译安装
```bash
mkdir build
cd build
cmake ..
make
sudo make install
```

## 使用方法
1. 引入头文件, cmake添加依赖
```cpp
#include "fytlogger/logger.hpp"
```
```cmake
find_package(fytlogger REQUIRED)
target_link_libraries(your_target fytlogger::fytlogger)
```

2. 注册Logger
```cpp
using namespace fyt::logger;
// 由于fytlogger使用<font></font>语法实现彩色日志，故文件名以md结尾
LoggerPool::registerLogger("somename", LogLevel::DEBUG, "/path/to/logfile.log.md");
```

3. 日志
```cpp
// 使用format格式
// {}对应的内容必须是fmt支持的类型
// 所有>=Logger日志级别的消息会被写入文件中，其余将在终端中打印
LoggerPool::getLogger("somename").log(LogLvel::INFO, "a = {}, b = {}, c = {}", 1, 2, 3);
LoggerPool::getLogger("somename").info("a = {}, b = {}, c = {}", 1, 2, 3); // 等同于上一行
```

4. 其他操作
```cpp
// 设置日志级别
LoggerPool::getLogger("somename").setLevel(LogLevel::INFO);
// 刷新输出流
LoggerPool::getLogger("somename").flush();
// 只打印
LoggerPool::getLogger("somename").print("a = {}, b = {}, c = {}", 1, 2, 3);
```