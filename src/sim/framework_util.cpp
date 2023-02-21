#include "framework_util.hpp"

/***
 * Resolve the base operation name and parameters from an environment of default operation parameters, operation aliases
 * and a parameter override.
 *
 * Merge override, aliased and default parameter maps such that the default parameters are overridden and possibly
 * expanded by a chain of aliases and a final override. The environment may be empty, resulting in an empty map.
 *
 * @return pair representing a base operation name and its parameter map in the given context
 */
std::pair<std::string, Parameters>
resolve_operation_parameters(const std::string &operation, OperationsToParameters default_parameters = {},
                             OperationAliasMap aliases = {}, Parameters override = {}) {
    if(!aliases.contains(operation) && !default_parameters.contains(operation)) {
        return {std::make_pair(operation, override)};
    }
    else if(default_parameters.contains(operation)) {
        Parameters merged;
        merged.merge(override);
        merged.merge(default_parameters[operation]);
        return {std::make_pair(operation, merged)};
    }
    OperationWithParameters target = aliases[operation];
    Parameters merged;
    merged.merge(override);
    merged.merge(target.second);
    return resolve_operation_parameters(target.first, default_parameters, aliases, merged);
}

NestableGeneratorPrototype::NestableGeneratorPrototype(std::string t): generator_type(std::move(t)) {}

/**
 * Add an operation prototype with parameters. If this generator prototype already contains nested generators, wraps
 * the operation into a new nested generator of current instance's generator type and nests it. Otherwise adds the
 * operation into current instance's operation prototype list.
 *
 * @param op an operation candidate with its parameters
 */
void NestableGeneratorPrototype::add_operations(const std::vector<OperationWithParameters>& ops) {
    if(!this->nested.empty()) {
        NestableGeneratorPrototype wrapper{this->generator_type};
        wrapper.add_operations(ops);
        this->nested.emplace_back(wrapper);
    }
    else {
        this->operation_candidates.insert(this->operation_candidates.end(), ops.begin(), ops.end());
    }
}

/**
 * Add a nested generator prototype. If this generator prototype contains free operation prototypes, wrap them into a
 * new nested generator of current instance's generator type and nests it first.
 *
 * @param op an operation candidate with its parameters
 */
void NestableGeneratorPrototype::add_nested_generator(const NestableGeneratorPrototype& gen) {
    if(!this->operation_candidates.empty()) {
        NestableGeneratorPrototype wrapper{this->generator_type};
        wrapper.add_operations(this->operation_candidates);
        this->nested.emplace_back(wrapper);
        this->operation_candidates.clear();
    }
    this->nested.emplace_back(gen);
}
