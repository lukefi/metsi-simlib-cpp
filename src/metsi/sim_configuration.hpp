#ifndef SIM_CONFIGURATION_HPP
#define SIM_CONFIGURATION_HPP
#include <yaml-cpp/yaml.h>
#include <core_types.hpp>
#include <framework_util.hpp>
#include <vector>
#include <set>

EventLabelsWithParameters parse_default_parameters(const YAML::Node&);
EventLabelAliases parse_event_aliases(const YAML::Node&);
std::set<int> parse_time_points_from_events_structure(const YAML::Node&);
std::vector<YAML::Node> find_generator_blocks_for_time(int, const YAML::Node&);
std::map<int, NestableGeneratorPrototype> parse_simulation_events(const YAML::Node&);

using AliasResolver = std::function<EventLabelWithParameters(EventLabelWithParameters)>;
template<typename T> using BaseEventResolver = std::function<ParameterizedEventFn<T>(EventLabelWithParameters)>;

template <typename T>
concept LessThanComparable = requires( T a, T b ) {
    { a < b } -> std::same_as<bool>;
};

template<LessThanComparable T> std::set<T> sequence_as_set(const YAML::Node& sequence) {
    auto vec = sequence.as<std::vector<T>>();
    return {vec.begin(), vec.end()};
};

AliasResolver alias_resolver_closure(const YAML::Node& control_yaml);


/**
 * Create a prepared event resolver with an alias resolver and a base event resolver as closure.
 *
 * @tparam T Simulation state type
 * @param alias_resolver a function for obtaining an unaliased EventLabelWithParameters i.e. an event prototype with base event name and unaliased parameters.
 * @param base_event_resolver a function for obtaining a base event function for given EventLabelWithParameters
 * @return a function for obtaining an EventFn<T> for simulation usage
 */
template<typename T> EventFnResolver<T> parameterized_event_resolver_closure(
        const AliasResolver& alias_resolver,
        const BaseEventResolver<T>& base_event_resolver) {
    return [alias_resolver, base_event_resolver](const EventLabelWithParameters& event_prototype) {
        EventLabelWithParameters unaliased_event_prototype = alias_resolver(event_prototype);
        ParameterizedEventFn<T> base_event = base_event_resolver(unaliased_event_prototype);
        EventFn<T> parameter_bound_event = parameterized_event_closure(base_event, unaliased_event_prototype.second);
        return parameter_bound_event;
    };
}


/**
 * With a given event resolver function and a control YAML source, prepare a runnable simulation for each time point,
 * and all simulation events.
 *
 * @tparam T Simulation state type
 * @param event_resolver a function for obtaining an EventFn<T> function for a given EventLabelWithParameters
 * @param control_yaml a YAML source with simulation_events, event_aliases and event_parameters blocks
 * @return a mapping of simulation time points to a root event graph node for that time point
 */
template<typename T> std::map<int, EventNode<T>> prepare_simulation(
        EventFnResolver<T> event_resolver,
        const YAML::Node& control_yaml
        ) {
    auto generator_prototypes = parse_simulation_events(control_yaml);
    std::map<int, EventNode<T>> retval;
    EventFn<T> no_op = [](StateReference<T> state) { return state; };
    for(auto pair : generator_prototypes) {
        auto rootnode = EventDAG<T>::new_node(no_op);
        LeafNodes<T> leafs = {rootnode};
        auto graph = construct_event_graph(pair.second, event_resolver, leafs);
        retval[pair.first] = rootnode;
    }
    return retval;
}

#endif