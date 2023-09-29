// Created by Chengfu Zou
// Copyright 2023 Chengfu Zou.

#ifndef FYT_WRITER_HPP_
#define FYT_WRITER_HPP_

// std
#include <fstream>
#include <mutex>

namespace fyt::logger
{

class Writer
{
public:
  explicit Writer(const std::string & filename);

  ~Writer();

  void write(const std::string & message);

  void flush();

private:
  std::ofstream file_;
  std::mutex & r_mutex_;
};
}  // namespace fyt::logger
#endif  // FYT_WRITER_HPP_
