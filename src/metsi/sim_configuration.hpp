#ifndef SIM_CONFIGURATION_HPP
#define SIM_CONFIGURATION_HPP
#include <yaml-cpp/yaml.h>
#include <core_types.hpp>
#include <vector>

OperationsToParameters parse_default_parameters(const YAML::Node& params);
OperationAliasMap parse_operation_aliases(const YAML::Node& aliases);

#endif