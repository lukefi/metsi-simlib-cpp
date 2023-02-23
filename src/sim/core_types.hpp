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
template<typename T> class EventDAG;
template<typename T> using EventNode = std::shared_ptr<EventDAG<T>>;
template<typename T> using EventNodes = std::vector<EventNode<T>>;
template<typename T> using LeafNodes = std::set<EventNode<T>>;
template<typename T> using OperationResults = std::vector<std::shared_ptr<T>>;
template<typename T> using SimOperation = std::function<std::shared_ptr<T>(std::shared_ptr<T>)>;
template<typename T> using OperationChain = std::vector<SimOperation<T>>;

/// Generator types
template<typename T> using GeneratorFn = std::function<LeafNodes<T>(LeafNodes<T>, std::vector<SimOperation<T>>)>;


/// Utility types
using Parameters = std::map<std::string, std::string>;
using OperationWithParameters = std::pair<std::string, Parameters>;
using OperationsToParameters = std::map<std::string, Parameters>;
using OperationAliasMap = std::map<std::string, OperationWithParameters>;
template<typename T> using ParameteredOperation = std::function<std::shared_ptr<T>(std::shared_ptr<T>, Parameters)>;

#endif

