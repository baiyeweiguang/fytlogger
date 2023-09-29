// Created by Chengfu Zou
// Copyright 2023 Chengfu Zou.

#include "fytlogger/logger.hpp"

#include <chrono>
#include <iomanip>
#include <utility>

namespace fyt::logger
{

namespace internal
{
Logger::Logger(std::string name, const std::string & filename, LogLevel level)
: consle_mutex_(GlobalMutex::getConsoleMutex()), name_(std::move(name)), level_(level)
{
  writer_ = std::make_unique<Writer>(filename);
}

void Logger::setLevel(LogLevel level) { level_ = level; }

void Logger::flush() { writer_->flush(); }

std::string Logger::getLocalTime()
{
  std::time_t tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  // std::tm tm = *std::gmtime(&tt);  //GMT (UTC)
  std::tm tm = *std::localtime(&tt); //Locale time-zone, usually UTC by default.
  std::stringstream ss;
  ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
  return ss.str();
}
}  // namespace internal

internal::Logger & LoggerPool::getLogger(const std::string & name)
{
  if (auto iter = loggers_.find(name); iter != loggers_.end()) {
    return *iter->second;
  } else {
    throw LoggerError("Logger not found");
  }
}

void LoggerPool::registerLogger(
  const std::string & name, const std::string & filename, LogLevel level)
{
  std::lock_guard<std::mutex> lock(l_mutex_);
  if (auto iter = loggers_.find(name); iter == loggers_.end()) {
    loggers_.insert({name, std::make_shared<internal::Logger>(name, filename, level)});
  }
}

std::mutex LoggerPool::l_mutex_;
std::unordered_map<std::string, std::shared_ptr<internal::Logger>> LoggerPool::loggers_;

}  // namespace fyt::logger