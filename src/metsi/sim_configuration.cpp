#include <set>
#include <vector>
#include <algorithm>
#include "sim_configuration.hpp"


/**
 * Convert a YAML mapping for a single operation tag with parameters into a pair.
 * @param params YAML-CPP map iterator
 * @return pair of operation name and operation parameters
 */
std::pair<std::string, Parameters> parse_parameter_set(const YAML::detail::iterator_value& params) {
    auto operation_name = params.first.as<std::string>();
    auto operation_params = params.second.as<std::map<std::string, std::string>>();
    return {operation_name, operation_params};
}

/**
 * Parse the control YAML structure's given operation_params section into a STL map.
 * @param params a YAML node corresponding to the control.yaml operation_params structure
 * @return Operation tags mapped to a set of Parameters
 */
OperationsToParameters parse_default_parameters(const YAML::Node& params) {
    OperationsToParameters result;
    using namespace YAML;
    switch(params.Type()) {
        case NodeType::Map:
            for(auto operation_mapping : params) {
                if(operation_mapping.second.Type() == NodeType::Map) {
                    result.insert(parse_parameter_set(operation_mapping));
                }
            }
            break;
        default:
            break;
    }
    return result;
}

/**
 * arse the control YAML structure's given operation_aliases section into a STL map.
 * @param params a YAML node corresponding to the control.yaml operation_aliases structure
 * @return Operation tag aliases mapped to another operation tag, optionally mapped to Parameters
 */
OperationAliasMap parse_operation_aliases(const YAML::Node& params) {
    OperationAliasMap result;
    using namespace YAML;
    switch(params.Type()) {
        case NodeType::Map:
            for(auto alias_mapping : params) {
                auto alias_name = alias_mapping.first.as<std::string>();
                if(alias_mapping.second.Type() == NodeType::Map) {
                    auto aliased_node = *alias_mapping.second.begin();
                    if(aliased_node.second.Type() == NodeType::Map) {
                        result.insert({alias_name, parse_parameter_set(aliased_node)});
                    }
                    else {
                        result.insert({alias_name, {alias_mapping.second.as<std::string>(), {}}});
                    }
                }
                else if(alias_mapping.second.Type() == NodeType::Scalar) {
                    result.insert({alias_name, {alias_mapping.second.as<std::string>(), {}}});
                }
            }
            break;
        default:
            break;
    }
    return result;
}

/**
 * Walk through the simulation_events YAML structure to discover all unique time points for the simulation.
 * @param events_structure YAML Node of YAML::NodeType::Map containing the simulation events structure
 * @return std::set<int> of unique time points
 */
std::set<int> parse_time_points_from_events_structure(const YAML::Node& events_structure) {
    std::set<int> unique_time_points;
    for(auto& event_block : events_structure) {
        unique_time_points.merge(sequence_as_set<int>(event_block["time_points"]));
    }
    return unique_time_points;
}
