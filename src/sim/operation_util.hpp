#include <map>
#include <string>
#include <optional>
#include "event_graph.hpp"

using Parameters = std::map<std::string, std::string>;
using OperationsToParameters = std::map<std::string, Parameters>;
using OperationAliasMap = std::map<std::string, OperationsToParameters>;
template<typename T> using ParameteredOperation = std::function<std::shared_ptr<T>(std::shared_ptr<T>, Parameters)>;

template<typename T> SimOperation<T> parameter_bound_operation(ParameteredOperation<T> op, Parameters params) {
    return [&op, &params](std::shared_ptr<T> payload) { return op(payload, params); };
}

std::pair<std::string, Parameters>
resolve_operation_parameters(const std::string &, OperationsToParameters, OperationAliasMap, Parameters);
