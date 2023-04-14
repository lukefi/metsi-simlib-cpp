#include "sim_events.hpp"
#include "motti_wrapper.hpp"

#include <utility>



StateReference<SimulationState> rest(StateReference<SimulationState> state, EventParameters params) {
    return state;
}

/**
 * Stub implementation for the grow_acta growth event. Should implement the growth models from Nyyssönen & Mielikäinen,
 * Acta Forestalia Fennica 178, 1978. Suggested to be refactored to a separate native C++ events package later. May
 * modify simulation state as enabled by SimulationState class interface.
 *
 * EventParameters are:
 *     step - a number of years of growth that this event invocation will apply. Optional, default 5.
 *
 * @param state current simulation state reference
 * @param params a map of generic event parameters
 * @return current simulation state reference
 */
StateReference<SimulationState> grow_acta(StateReference<SimulationState> state, EventParameters params) {
    int step = 5;
    if(params.contains("step")) {
        step = boost::lexical_cast<int>(params["step"]);
    }

    auto& ds = state->get_vars("tree#dbh");
    auto& hs = state->get_vars("tree#height");
    auto& ages = state->get_vars("tree#age");
    auto& spes = state->get_vars("tree#species");

    for(int i=0;i<state->get_stand().trees.size();i++) {
        ds[i] += 0.1f;
        hs[i] += 1.0f;
        ages[i] += (float)step;
    }

    return state;
}

StateReference<SimulationState> grow_motti(StateReference<SimulationState> state, EventParameters params) {
    int step = 5;
    if(params.contains("step")) {
        step = boost::lexical_cast<int>(params["step"]);
    }
    auto& motti = MottiWrapper::get_instance();

    //TODO: state to Motti4Site, Motti4Tree etc here
    //...

    //TODO: call Motti 4 functions
    //...

    //TODO: results back to state
    //...

    return state;
}


/**
 * Find a ParameterizedEventFn based on an EventLabelWithParameters.
 *
 * EventLabelWithParameters -> EventParameters are:
 *     implementation - a known implementation for this event. Optional, default 'native'.
 *         'native' denotes C++ compiled event functions
 *
 * @param prototype
 * @return a ParameterizedEventFn matchin the event label
 * @throws domain_error upon unknown implementation type or function not found
 */
ParameterizedEventFn<SimulationState> base_event_resolver(EventLabelWithParameters prototype) {
    auto event_name = prototype.first;
    auto params = prototype.second;
    std::string impl_type("native");
    if(params.contains("implementation")) {
        impl_type = params["implementation"];
    }
    if(impl_type == "native") {
        if(!builtin_events.contains(event_name)) {
            throw std::domain_error("Event " + event_name + " not available as native implementation.");
        }
        auto parameterized_event_fn = builtin_events.at(event_name);
        return parameterized_event_fn;
    }
    else {
        throw std::domain_error("Unknown implementation type " + impl_type);
    }
}