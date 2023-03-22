#ifndef CORE_TYPES_HPP
#define CORE_TYPES_HPP

#include <functional>
#include <map>
#include <memory>
#include <set>
#include <utility>
#include <vector>
/**
* Forward declarations of core types in the metsi sim library.
*/

/// Event graph types
template<typename T> class EventDAG;
template<typename T> using EventNode = std::shared_ptr<EventDAG<T>>;
template<typename T> using EventNodes = std::vector<EventNode<T>>;
template<typename T> using LeafNodes = std::set<EventNode<T>>;
template<typename T> using StateReference = std::shared_ptr<T>;
template<typename T> using ResultStates = std::vector<StateReference<T>>;
template<typename T> using EventFn = std::function<StateReference<T>(StateReference<T>)>;
template<typename T> using EventChain = std::vector<EventFn<T>>;

/// Generator types
template<typename T> using GeneratorFn = std::function<LeafNodes<T>(LeafNodes<T>, std::vector<EventFn<T>>)>;

/// Utility types
using EventParameters = std::map<std::string, std::string>;
using EventLabelWithParameters = std::pair<std::string, EventParameters>;
using EventLabelsWithParameters = std::map<std::string, EventParameters>;
using EventLabelAliases = std::map<std::string, EventLabelWithParameters>;
template<typename T> using ParameterizedEventFn = std::function<StateReference<T>(StateReference<T>, EventParameters)>;
template<typename T> using EventFnResolver = std::function<EventFn<T>(EventLabelWithParameters)>;

/// Exceptions
class BranchException : std::exception {
private:
    std::string message;
public:
    explicit BranchException(std::string msg) : message(std::move(msg)) {};
    [[nodiscard]] const char* what() const noexcept override {
        return message.c_str();
    }
};


#endif

