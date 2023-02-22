#ifndef FRAMEWORK_UTIL_HPP
#define FRAMEWORK_UTIL_HPP

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

/**
 * A nested class to portray operations with parameters for generators, structured as nested constructs for simulation
 * events. Contains the bare essentials for generating an EventDAG graph using a source for generator functions and
 * operation functions. An instance of this class is either
 *
 * 1) a leaf node with operations and no nested nodes, serving as instructions for running a generator function.
 * 2) an intermediate node with nested generators and no operations, serving as a wrapper for eventual leaf nodes.
 *
 * Usage of the functions add_operations and add_nested_generators ensure the internal division between those two cases.
 */
class NestableGeneratorPrototype {
    mutable std::vector<OperationWithParameters> operation_candidates;
    std::string generator_type;
    mutable std::vector<NestableGeneratorPrototype> nested;

public:
    explicit NestableGeneratorPrototype(std::string);
    void add_operations(const std::vector<OperationWithParameters>&);
    void add_nested_generator(const NestableGeneratorPrototype&);
    bool is_leaf() { return this->nested.empty(); };
    const std::vector<NestableGeneratorPrototype>& get_nested() const { return this->nested; };
    const std::vector<OperationWithParameters>& get_operations() const { return this->operation_candidates; };
    const std::string get_type() const { return this->generator_type; };
};

#endif