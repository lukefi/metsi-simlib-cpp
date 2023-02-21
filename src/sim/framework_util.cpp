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