#include <metsi-simlib/framework_util.hpp>

/***
 * Resolve the base event label and parameters from an environment of default event parameters, event aliases
 * and a parameter override.
 *
 * Merge override, aliased and default parameter maps such that the default parameters are overridden and possibly
 * expanded by a chain of aliases and a final override. The environment may be empty, resulting in an empty map.
 *
 * @return pair representing a base event label and its parameter map in the given context
 */
EventLabelWithParameters resolve_event_aliasing(const std::string &event_label,
        EventLabelsWithParameters default_parameters, EventLabelAliases aliases, EventParameters override) {
    if(!aliases.contains(event_label) && !default_parameters.contains(event_label)) {
        return {std::make_pair(event_label, override)};
    }
    else if(default_parameters.contains(event_label)) {
        EventParameters merged;
        merged.merge(override);
        merged.merge(default_parameters[event_label]);
        return {std::make_pair(event_label, merged)};
    }
    EventLabelWithParameters target = aliases[event_label];
    EventParameters merged;
    merged.merge(override);
    merged.merge(target.second);
    return resolve_event_aliasing(target.first, default_parameters, aliases, merged);
}

NestableGeneratorPrototype::NestableGeneratorPrototype(std::string t): generator_type(std::move(t)) {}

/**
 * Adds event prototypes. If this generator prototype already contains nested generator prototypes,
 * wraps the event prototypes into a new nestable generator prototype of current instance's generator type and nests it.
 * Otherwise adds the event prototypes into current instance's event prototype vector.
 *
 * @param event_prototype an event prototype with its parameters
 */
void NestableGeneratorPrototype::add_event_prototypes(const std::vector<EventLabelWithParameters>& event_prototypes) {
    if(!this->nested_generator_prototypes.empty()) {
        NestableGeneratorPrototype wrapper{this->generator_type};
        wrapper.add_event_prototypes(event_prototypes);
        this->nested_generator_prototypes.emplace_back(wrapper);
    }
    else {
        this->event_prototypes.insert(
                this->event_prototypes.end(),
                event_prototypes.begin(),
                event_prototypes.end());
    }
}

/**
 * Add a nested generator prototype. If this generator prototype contains free event prototypes, wrap them into a
 * new nested generator prototype of current instance's generator type and nests it first.
 *
 * @param gen a nestable generator prototype
 */
void NestableGeneratorPrototype::add_nested_generator(const NestableGeneratorPrototype& gen) {
    if(!this->event_prototypes.empty()) {
        NestableGeneratorPrototype wrapper{this->generator_type};
        wrapper.add_event_prototypes(this->event_prototypes);
        this->nested_generator_prototypes.emplace_back(wrapper);
        this->event_prototypes.clear();
    }
    this->nested_generator_prototypes.emplace_back(gen);
}
