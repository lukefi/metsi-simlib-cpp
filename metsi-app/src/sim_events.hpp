#ifndef SIM_EVENTS_HPP
#define SIM_EVENTS_HPP
#include <vector>
#include <string>
#include <metsi-simlib/core_types.hpp>
#include <metsi-simlib/framework_util.hpp>
#include "state_model.hpp"

StateReference<SimulationState> rest(StateReference<SimulationState>, EventParameters);
StateReference<SimulationState> grow_acta(StateReference<SimulationState>, EventParameters);
StateReference<SimulationState> grow_motti(StateReference<SimulationState>, EventParameters);

static std::map<std::string, ParameterizedEventFn<SimulationState>> builtin_events{
        {"grow", grow_acta},
        {"grow_acta", grow_acta},
        {"grow_motti", grow_motti},
        {"rest", rest},
        {"do_nothing", rest}
};

ParameterizedEventFn<SimulationState> base_event_resolver(EventLabelWithParameters);


#endif