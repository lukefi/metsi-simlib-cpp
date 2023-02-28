#ifndef FILE_IO_HPP
#define FILE_IO_HPP
#include <yaml-cpp/yaml.h>

YAML::Node read_yaml(const std::string& file_path);

#endif