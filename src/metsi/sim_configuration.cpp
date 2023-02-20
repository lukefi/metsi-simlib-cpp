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
