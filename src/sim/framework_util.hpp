#ifndef FRAMEWORK_UTIL_HPP
#define FRAMEWORK_UTIL_HPP

#include <map>
#include <string>
#include <optional>
#include "event_graph.hpp"
#include "core_types.hpp"

/**
 * Prepare the given T,map=>T parameterizable event function as a T=>T closure capturing the parameter map.
 *
 * @tparam T a type representing the simulation state
 * @param event a T,map=>T function
 * @param params a map representing domain parameters for event
 * @return a T=>T closure capturing the map of domain parameters for event
 */
template<typename T> EventFn<T> parameterized_event_closure(const ParameterizedEventFn<T>& event, const EventParameters& params) {
    return [event, params](std::shared_ptr<T> sim_state) { return event(sim_state, params); };
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
    bool is_leaf() { return this->nested_generator_prototypes.empty(); };
    const std::vector<NestableGeneratorPrototype>& get_nested_generator_prototypes() const { return this->nested_generator_prototypes; };
    const std::vector<EventLabelWithParameters>& get_event_prototypes() const { return this->event_prototypes; };
    const std::string get_type() const { return this->generator_type; };
};

#endif