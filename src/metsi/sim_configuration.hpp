#ifndef SIM_CONFIGURATION_HPP
#define SIM_CONFIGURATION_HPP
#include <yaml-cpp/yaml.h>
#include <core_types.hpp>
#include <framework_util.hpp>
#include <vector>
#include <set>

EventLabelsWithParameters parse_default_parameters(const YAML::Node& params);
EventLabelAliases parse_event_aliases(const YAML::Node& params);
std::set<int> parse_time_points_from_events_structure(const YAML::Node& events_structure);
std::vector<YAML::Node> find_generator_blocks_for_time(int time_point, const YAML::Node& events_structure);
std::map<int, NestableGeneratorPrototype> parse_simulation_events(const YAML::Node& events_structure);

template <typename T>
concept LessThanComparable = requires( T a, T b ) {
    { a < b } -> std::same_as<bool>;
};

template<LessThanComparable T> std::set<T> sequence_as_set(const YAML::Node& sequence) {
    auto vec = sequence.as<std::vector<T>>();
    return {vec.begin(), vec.end()};
}

#endif