#ifndef FILE_IO_HPP
#define FILE_IO_HPP
#include <yaml-cpp/yaml.h>
#include <csv.hpp>
#include "state_model.hpp"

YAML::Node read_yaml(const std::string&);
std::vector<ForestStand> read_forest_csv(const std::string&);

#endif