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
template<typename T> using ResultStates = std::vector<std::shared_ptr<T>>;
template<typename T> using EventFn = std::function<std::shared_ptr<T>(std::shared_ptr<T>)>;
template<typename T> using EventChain = std::vector<EventFn<T>>;

/// Generator types
template<typename T> using GeneratorFn = std::function<LeafNodes<T>(LeafNodes<T>, std::vector<EventFn<T>>)>;

/// Utility types
using EventParameters = std::map<std::string, std::string>;
using EventLabelWithParameters = std::pair<std::string, EventParameters>;
using EventLabelsWithParameters = std::map<std::string, EventParameters>;
using EventLabelAliases = std::map<std::string, EventLabelWithParameters>;
template<typename T> using ParameterizedEventFn = std::function<std::shared_ptr<T>(std::shared_ptr<T>, EventParameters)>;
template<typename T> using EventFnResolver = std::function<EventFn<T>(EventLabelWithParameters)>;

#endif

