#ifndef FRAMEWORK_UTIL_HPP
#define FRAMEWORK_UTIL_HPP

#include <map>
#include <string>
#include <optional>
#include "event_graph.hpp"
#include "core_types.hpp"
#include "branching_generators.hpp"

/**
 * Prepare the given T,map=>T parameterizable event function as a T=>T closure capturing the parameter map.
 *
 * @tparam T a type representing the simulation state
 * @param event a T,map=>T function
 * @param params a map representing domain parameters for event
 * @return a T=>T closure capturing the map of domain parameters for event
 */
template<typename T> EventFn<T> parameterized_event_closure(const ParameterizedEventFn<T>& event, const EventParameters& params) {
    return [event, params](StateReference<T> sim_state) { return event(sim_state, params); };
}

EventLabelWithParameters resolve_event_aliasing(const std::string &event_label, EventLabelsWithParameters default_parameters, EventLabelAliases aliases,
                                                EventParameters override);

/**
 * A nested generator prototype class to portray event labels with parameters for generators, structured as
 * nested constructs for simulation events. Contains the bare essentials for generating an EventDAG
 * graph using a source for generator functions and event functions. An instance of this class is either
 *
 * 1) a leaf node with event labels and no nested generators, serving as instructions for running a generator function.
 * 2) an intermediate node with nested generators and no event labels, serving as a wrapper for eventual leaf nodes.
 *
 * Usage of the functions add_event_prototypes and add_nested_generators ensure the internal division between those two cases.
 */
class NestableGeneratorPrototype {
    mutable std::vector<EventLabelWithParameters> event_prototypes;
    std::string generator_type;
    mutable std::vector<NestableGeneratorPrototype> nested_generator_prototypes;

public:
    explicit NestableGeneratorPrototype(std::string);
    void add_event_prototypes(const std::vector<EventLabelWithParameters> &event_prototypes);
    void add_nested_generator(const NestableGeneratorPrototype&);
    bool is_leaf() const { return this->nested_generator_prototypes.empty(); };
    const std::vector<NestableGeneratorPrototype>& get_nested_generator_prototypes() const { return this->nested_generator_prototypes; };
    const std::vector<EventLabelWithParameters>& get_event_prototypes() const { return this->event_prototypes; };
    std::string get_type() const { return this->generator_type; };
};


/**
 * Recursively extend a simulation EventDAG<T> graph. Based on a given NestableGeneratorPrototype, an EventFn<T>
 * resolver function and the leaf nodes of an existing graph. For a new graph for starting a simulation event graph, the
 * external call to this function should give a LeafNodes<T> argument holding a single EventNode<T>.
 *
 * @tparam T simulation state type
 * @param generator_prototype a nestable generator prototype describing a simulation events tree
 * @param event_resolver an EventLabelWithParameters=>EventFn<T> function for finding a T=>T function for simulation
 * @param previous_nodes previous event nodes where the nodes from this prototype's generator are added
 * @return event nodes as extended by using the given generator_prototype
 */
template<typename T> LeafNodes<T> construct_event_graph(
        const NestableGeneratorPrototype& generator_prototype,
        EventFnResolver<T> event_resolver,
        LeafNodes<T> previous_nodes) {
    auto gen_type = generator_prototype.get_type();
    if(generator_prototype.is_leaf()) {
        GeneratorFn<T> generator_fn = generator_by_name<T>(gen_type).value();
        EventChain<T> events;
        for(auto event : generator_prototype.get_event_prototypes()) {
            events.emplace_back(event_resolver(event));
        }
        return generator_fn(previous_nodes, events);
    }
    else {
        if(gen_type == "sequence") {
            LeafNodes<T> current = previous_nodes;
            for(auto nested : generator_prototype.get_nested_generator_prototypes()) {
                current = construct_event_graph(nested, event_resolver, current);
            }
            return current;
        }
        else if(gen_type == "alternatives") {
            LeafNodes<T> current;
            for(auto nested : generator_prototype.get_nested_generator_prototypes()) {
                LeafNodes<T> next = construct_event_graph(nested, event_resolver, previous_nodes);
                current.insert(next.begin(), next.end());
            }
            return current;
        }
    }
    return previous_nodes;
}

#endif