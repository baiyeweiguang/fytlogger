// Created by Chengfu Zou
// Copyright 2023 Chengfu Zou.

#ifndef FYT_LOGGER_TYPES_HPP_
#define FYT_LOGGER_TYPES_HPP_

// std
#include <algorithm>
#include <mutex>
#include <unordered_map>

// fmt
#include <fmt/color.h>
#include <fmt/core.h>

namespace fyt::logger
{

enum class LogLevel : std::uint8_t { DEBUG, INFO, WARN, ERROR, FATAL };

constexpr const char * LogNameTable[5] = {"DEBUG", "INFO", "WARN", "ERROR", "FATAL"};

// DEBUG = gray, INFO = white, WARN = yellow, ERROR = red, FATAL = blue
constexpr const char * LogColorTable[5] = {
  "<font color=\"#9B9B9B\">{}</font>", "<font color=\"#FFFFFF\">{}</font>",
  "<font color=\"#FFFF00\">{}</font>", "<font color=\"#FF0000\">{}</font>",
  "<font color=\"#0000FF\">{}</font>"};

constexpr fmt::color LogFmtColorTable[5] = {
  fmt::color::gray, fmt::color::white, fmt::color::yellow, fmt::color::red, fmt::color::blue};

class LoggerError : public std::runtime_error
{
  using std::runtime_error::runtime_error;
};

static std::mutex g_mutex_;
class GlobalMutex
{
public:
  inline static std::mutex & getConsoleMutex()
  {
    static std::mutex s_mutex;
    return s_mutex;
  }

  inline static std::mutex & getFileMutex(const std::string & filename)
  {
    static std::unordered_map<std::string, std::mutex> file_mutex_map;
    std::lock_guard<std::mutex> lock(g_mutex_);
    return file_mutex_map[filename];
  }

private:
  GlobalMutex() = default;
  ~GlobalMutex() = default;
};
}  // namespace fyt::logger
#endif  // FYT_LOGGER_TYPES_HPP_