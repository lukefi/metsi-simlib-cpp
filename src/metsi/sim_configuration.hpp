#ifndef SIM_CONFIGURATION_HPP
#define SIM_CONFIGURATION_HPP
#include <yaml-cpp/yaml.h>
#include <core_types.hpp>
#include <vector>
#include <set>

OperationsToParameters parse_default_parameters(const YAML::Node& params);
OperationAliasMap parse_operation_aliases(const YAML::Node& aliases);

template <typename T>
concept LessThanComparable = requires( T a, T b ) {
    { a < b } -> std::same_as<bool>;
};

template<LessThanComparable T> std::set<T> sequence_as_set(const YAML::Node& sequence) {
    auto vec = sequence.as<std::vector<T>>();
    return {vec.begin(), vec.end()};
}

#endif