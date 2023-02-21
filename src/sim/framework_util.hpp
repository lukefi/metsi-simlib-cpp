#include <map>
#include <string>
#include <optional>
#include "event_graph.hpp"
#include "core_types.hpp"

/**
 * Prepare the given CU,map=>CU parametrizable function as a CU=>CU closure capturing the parameter map.
 *
 * @tparam CU a type representing the simulation state
 * @param op a CU,map=>CU function
 * @param params a map representing domain parameters for op
 * @return a CU=>CU closure capturing the map of domain parameters for op
 */
template<typename CU> SimOperation<CU> parameter_bound_operation(const ParameteredOperation<CU>& op, const Parameters& params) {
    return [op, params](std::shared_ptr<CU> payload) { return op(payload, params); };
}

std::pair<std::string, Parameters>
resolve_operation_parameters(const std::string &, OperationsToParameters, OperationAliasMap, Parameters);
