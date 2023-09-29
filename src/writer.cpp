// Created by Chengfu Zou
// Copyright 2023 Chengfu Zou.

#include "fytlogger/writer.hpp"
#include "fytlogger/types.hpp"

#include <filesystem>
#include <fstream>

namespace fyt::logger
{

Writer::Writer(const std::string & filename) : r_mutex_(GlobalMutex::getFileMutex(filename))
{
  auto parent_path = std::filesystem::path(filename).parent_path();
  parent_path = parent_path.empty() ? "." : parent_path;
  if (!std::filesystem::exists(parent_path)) {
    std::filesystem::create_directories(parent_path);
  }
  file_.open(filename, std::ios::out | std::ios::app);
}

void Writer::write(const std::string & message)
{
  std::lock_guard<std::mutex> lock(r_mutex_);
  file_ << message << "\n\n";
  file_.flush();
}

void Writer::flush()
{
  std::lock_guard<std::mutex> lock(r_mutex_);
  file_.flush();
}

Writer::~Writer()
{
  std::lock_guard<std::mutex> lock(r_mutex_);
  file_.close();
}

}  // namespace fyt::logger