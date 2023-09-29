#include <thread>
#include <vector>

#include "fytlogger/logger.hpp"

int main()
{
  // 使用方法
  // 1.使用LoggerPool::registerLogger注册一个logger
  // 2.使用LoggerPool::getLogger获取一个logger，然后使用log方法打印日志
  using namespace fyt::logger;
  LoggerPool::registerLogger("test1", "../log/e.log.md", LogLevel::DEBUG);
  LoggerPool::registerLogger("test2", "../log/e.log.md", LogLevel::DEBUG);
  // 多线程测试
  std::vector<std::thread> threads;
  for (unsigned j = 0; j < 5; j++) {
    threads.emplace_back(std::thread([j]() {
      for (unsigned i = 0; i < 10; i++) {
        LoggerPool::getLogger("test1").log(LogLevel::WARN, "Hello, world! {} {}", j, i);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        LoggerPool::getLogger("test2").info("你好，世界！ {} {}", j, i);
      }
    }));
  }
  for (auto & t : threads) {
    t.join();
  }
  return 0;
}