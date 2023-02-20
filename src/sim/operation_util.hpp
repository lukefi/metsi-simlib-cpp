#include <map>
#include <string>
#include <optional>
#include "event_graph.hpp"

using Parameters = std::map<std::string, std::string>;
using OperationsToParameters = std::map<std::string, Parameters>;
using OperationAliasMap = std::map<std::string, OperationsToParameters>;
template<typename CU> using ParameteredOperation = std::function<std::shared_ptr<CU>(std::shared_ptr<CU>, Parameters)>;

/**
 * Prepare the given CU,map=>CU parametrizable function as a CU=>CU closure capturing the parameter map.
 *
 * @tparam CU a type representing the simulation state
 * @param op a CU,map=>CU function
 * @param params a map representing domain parameters for op
 * @return a CU=>CU closure capturing the map of domain parameters for op
 */
template<typename CU> SimOperation<CU> parameter_bound_operation(ParameteredOperation<CU> op, Parameters params) {
    return [&op, &params](std::shared_ptr<CU> payload) { return op(payload, params); };
}

std::pair<std::string, Parameters>
resolve_operation_parameters(const std::string &, OperationsToParameters, OperationAliasMap, Parameters);
