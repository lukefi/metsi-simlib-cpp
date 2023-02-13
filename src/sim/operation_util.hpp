#include <map>
#include "event_graph.hpp"

template<typename T> using ParameteredOperation = std::function<T(T, std::map<std::string, std::string>)>;

template<typename T> SimOperation<T> parameter_bound_operation(ParameteredOperation<T> op, std::map<std::string, std::string> params) {
    return [&op, &params](T payload) { return op(payload, params); };
}