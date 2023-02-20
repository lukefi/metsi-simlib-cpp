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
                    OperationsToParameters target;
                    if(aliased_node.second.Type() == NodeType::Map) {
                        target.insert(parse_parameter_set(aliased_node));
                        result.insert({alias_name, target});
                    }
                }
                else if(alias_mapping.second.Type() == NodeType::Scalar) {
                    OperationsToParameters target{{alias_mapping.second.as<std::string>(), {}}};
                    result.insert({alias_name, target});
                }
            }
            break;
        default:
            break;
    }
    return result;
}
