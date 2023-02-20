#include <map>
#include <string>
#include <optional>
#include "event_graph.hpp"

using Parameters = std::map<std::string, std::string>;
using OperationsToParameters = std::map<std::string, Parameters>;
using OperationAliasMap = std::map<std::string, OperationsToParameters>;
template<typename T> using ParameteredOperation = std::function<std::shared_ptr<T>(std::shared_ptr<T>, Parameters)>;

/**
 * Prepare the given T,map=>T parametrizable function as a T=>T closure capturing the parameter map.
 *
 * @tparam T a type representing the simulation state
 * @param op a T,map=>T function
 * @param params a map representing domain parameters for op
 * @return a T=>T closure capturing the map of domain parameters for op
 */
template<typename T> SimOperation<T> parameter_bound_operation(ParameteredOperation<T> op, Parameters params) {
    return [&op, &params](std::shared_ptr<T> payload) { return op(payload, params); };
}

std::pair<std::string, Parameters>
resolve_operation_parameters(const std::string &, OperationsToParameters, OperationAliasMap, Parameters);
