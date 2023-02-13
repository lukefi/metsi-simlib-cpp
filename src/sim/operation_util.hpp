#include <map>
#include "event_graph.hpp"

template<typename T> using ParameteredOperation = std::function<std::shared_ptr<T>(std::shared_ptr<T>, std::map<std::string, std::string>)>;

template<typename T> SimOperation<T> parameter_bound_operation(ParameteredOperation<T> op, std::map<std::string, std::string> params) {
    return [&op, &params](std::shared_ptr<T> payload) { return op(payload, params); };
}