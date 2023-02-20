#ifndef CORE_TYPES_HPP
#define CORE_TYPES_HPP

#include <functional>
#include <map>
#include <memory>
#include <set>
#include <vector>
/**
* Forward declarations of core types in the metsi sim library.
*/

/// Event graph types
template<typename OP> class EventDAG;
template<typename OP> using EventNode = std::shared_ptr<EventDAG<OP>>;
template<typename OP> using EventNodes = std::vector<EventNode<OP>>;
template<typename OP> using LeafNodes = std::set<EventNode<OP>>;
template<typename OP> using OperationResults = std::vector<std::shared_ptr<OP>>;
template<typename OP> using SimOperation = std::function<std::shared_ptr<OP>(std::shared_ptr<OP>)>;
template<typename OP> using OperationChain = std::vector<SimOperation<OP>>;

/// Generator types
template<typename OP> using GeneratorFn = std::function<LeafNodes<OP>(LeafNodes<OP>, std::vector<SimOperation<OP>>)>;


/// Utility types
using Parameters = std::map<std::string, std::string>;
using OperationWithParameters = std::pair<std::string, Parameters>;
using OperationsToParameters = std::map<std::string, Parameters>;
using OperationAliasMap = std::map<std::string, OperationWithParameters>;
template<typename CU> using ParameteredOperation = std::function<std::shared_ptr<CU>(std::shared_ptr<CU>, Parameters)>;

#endif

