// Created by Chengfu Zou
// Copyright 2023 Chengfu Zou.

#ifndef FYT_LOGGER_HPP_
#define FYT_LOGGER_HPP_

// std
#include <array>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>

// fmt
#include <fmt/color.h>
#include <fmt/core.h>

// project
#include "fytlogger/types.hpp"
#include "fytlogger/writer.hpp"

namespace fyt::logger
{

namespace internal
{
class Logger
{
public:
  Logger(std::string name, const std::string & filename, LogLevel level);

  template <typename... Args>
  void log(LogLevel level, const std::string & format, Args... args)
  {
    std::string log_info = fmt::format(format, args...);
    std::string level_prefix = fmt::format("[{}] ", LogNameTable[static_cast<std::uint8_t>(level)]);
    std::string name_prefix = fmt::format("[{}] ", name_);
    std::string log_time = fmt::format("[{}] ", getLocalTime());
    std::string message =
      fmt::format("{} {} {}: {}", level_prefix, name_prefix, log_time, log_info);

    if (level >= this->level_) {
      std::string colored_message =
        fmt::format(LogColorTable[static_cast<std::uint8_t>(level)], message);
      writer_->write(colored_message);
    }
    std::lock_guard<std::mutex> lock(consle_mutex_);
    fmt::print(fg(LogFmtColorTable[static_cast<std::uint8_t>(level)]), "{}\n", message);
  }

  template <typename... Args>
  void debug(const std::string & format, Args... args)
  {
    log(LogLevel::DEBUG, format, args...);
  }

  template <typename... Args>
  void info(const std::string & format, Args... args)
  {
    log(LogLevel::INFO, format, args...);
  }

  template <typename... Args>
  void warn(const std::string & format, Args... args)
  {
    log(LogLevel::WARN, format, args...);
  }

  template <typename... Args>
  void error(const std::string & format, Args... args)
  {
    log(LogLevel::ERROR, format, args...);
  }

  template <typename... Args>
  void fatal(const std::string & format, Args... args)
  {
    log(LogLevel::FATAL, format, args...);
  }

  template<typename... Args>
  void print(const std::string & format, Args... args)
  {
    std::lock_guard<std::mutex> lock(consle_mutex_);
    fmt::print(format, args...);
  }

  void setLevel(LogLevel level);

  void flush();

private:
  std::string getLocalTime();

  std::mutex & consle_mutex_;
  std::string name_;
  LogLevel level_;
  std::unique_ptr<Writer> writer_;
};
}  // namespace internal

class LoggerPool
{
public:
  static internal::Logger & getLogger(const std::string & name);

  static void registerLogger(
    const std::string & name, const std::string & filename, LogLevel level);

private:
  LoggerPool() = default;
  ~LoggerPool() = default;
  LoggerPool(const LoggerPool &) = delete;
  LoggerPool & operator=(const LoggerPool &) = delete;
  LoggerPool(LoggerPool &&) = delete;

  static std::mutex l_mutex_;
  static std::unordered_map<std::string, std::shared_ptr<internal::Logger>> loggers_;
};

}  // namespace fyt::logger
#endif  // FYT_LOGGER_HPP_