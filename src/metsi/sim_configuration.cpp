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

/**
 * Walk through the simulation_events YAML structure to discover all individual generator declaration blocks for the
 * simulation appearing in the given time point.
 * @param time_point a simulation time point
 * @param events_structure YAML Node of YAML::NodeType::Map containing the simulation events structure
 * @return std::vector<YAML::Node> of generator blocks
 */
std::vector<YAML::Node> find_generator_blocks_for_time(int time_point, const YAML::Node& events_structure) {
    std::vector<YAML::Node> generators;
    for(auto event_block : events_structure) {
        auto time_points = event_block["time_points"].as<std::vector<int>>();

        if(sequence_as_set<int>(event_block["time_points"]).contains(time_point)) {
            auto generator_block = event_block["generators"];
            for(auto generator : generator_block) {
                generators.emplace_back(generator);
            }
        }
    }
    return generators;
}

/**
 * Recursively walk through a nested simulation event declaration node, producing a root generator node for it.
 * @param generator_block a single generator declaration
 * @return NestableGeneratorPrototype representation of the generator declaration
 */
NestableGeneratorPrototype build_generator(const YAML::Node& generator_block) {
    auto current = *generator_block.begin();
    auto current_key  = current.first.as<std::string>();
    NestableGeneratorPrototype target(current_key);
    for(auto entry : current.second) {
        if(entry.Type() == YAML::NodeType::Scalar) {
            // bare operation name without inline parameters
            auto tag = entry.as<std::string>();
            OperationWithParameters prepared_operation = std::make_pair(tag, Parameters{});
            target.add_operations(std::vector<OperationWithParameters>{prepared_operation});
        }
        else if(entry.Type() == YAML::NodeType::Map) {
            auto potentially_nested = *entry.begin();
            auto keyname = potentially_nested.first.as<std::string>();
            std::set<std::string> generator_types{"sequence", "alternatives"};
            if(generator_types.contains(keyname)) {
                // nested generator
                auto nested = build_generator(entry);
                target.add_nested_generator(nested);
            }
            else {
                // in-line parametrized operation
                auto tag = entry.as<std::string>();
                target.add_operations(std::vector<OperationWithParameters>{parse_parameter_set(potentially_nested)});
            }
        }
    }
    return target;
}

/**
 * Transform the simulation_events structure into a map of NestableGeneratorPrototype for each individual time point in
 * the simulation.
 * @param events_structure YAML node describing the simulation_events structure for the simulation
 * @return a map of unique time points paired with a NestableGeneratorPrototype
 */
std::map<int, NestableGeneratorPrototype> parse_simulation_events(const YAML::Node& events_structure) {
    using namespace YAML;
    std::set<int> unique_time_points = parse_time_points_from_events_structure(events_structure);
    std::map<int, NestableGeneratorPrototype> generators_by_time_point;
    for(auto time_point : unique_time_points) {
        auto generator_blocks = find_generator_blocks_for_time(time_point, events_structure);
        NestableGeneratorPrototype root_generator("sequence");
        for(const auto& generator_block : generator_blocks) {
            root_generator.add_nested_generator(build_generator(generator_block));
        }
        generators_by_time_point.insert({time_point, root_generator});
    }
    return generators_by_time_point;
}
