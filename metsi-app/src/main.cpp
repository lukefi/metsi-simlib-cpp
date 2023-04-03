#include <iostream>

#include "app_io.hpp"
#include "file_io.hpp"
#include "sim_configuration.hpp"
#include "sim_events.hpp"
#include <metsi-simlib/core_types.hpp>

ResultStates<SimulationState> do_simulate(
        const std::map<int, EventNode<SimulationState>>& simulation,
        const ForestStand& stand) {
    ResultStates<SimulationState> stand_states{std::make_shared<SimulationState>(stand)};

    for(const auto& pair : simulation) {
        ResultStates<SimulationState> time_point_results;
        for(const auto& state : stand_states) {
            try {
                auto current = pair.second->evaluate_depth(state);
                time_point_results.insert(time_point_results.end(), current.begin(), current.end());
            }
            catch(BranchException& e) {}
        }
        stand_states = time_point_results;
    }
    return stand_states;
}

void do_export(const ResultStates<SimulationState>& schedules) {
    //TODO: export modules logic
}

void process_stands(const YAML::Node& control_data, ForestStandCSVReader source_data) {
    auto alias_resolver = alias_resolver_closure(control_data);
    auto event_resolver = parameterized_event_resolver_closure<SimulationState>(alias_resolver, base_event_resolver);
    auto simulation = prepare_simulation<SimulationState>(event_resolver, control_data["simulation_events"]);

    for(const auto& stand : source_data) {
        auto stand_id = stand.get<std::string>("identifier");
        auto stand_states = do_simulate(simulation, stand);
        std::cout << "Produced " << stand_states.size() << " schedules for stand " << stand_id << "\n";
        do_export(stand_states);
        std::cout << "Exported data for stand " << stand_id << " (not really; not implemented yet)" << "\n";
    }
}

int main(int argc, char** argv) {
    auto args = positional_arguments(argc, argv);

    std::cout << "Input data file: " << args["input-path"] << '\n';
    std::cout << "Output data directory: " << args["output-path"] << '\n';
    std::cout << "Control file: " << args["control-file"] << '\n';

    YAML::Node control_data;
    try {
        control_data = read_yaml(args["control-file"]);
    }
    catch(std::exception&) {
        std::cout << "Unable to read control file. Aborting.\n";
        return 1;
    }

    ForestStandCSVReader stands;
    try {
        stands = ForestStandCSVReader(args["input-path"]);
    }
    catch(std::exception&) {
        std::cout << "Unable to read input file. Aborting.\n";
        return 1;
    }

    try {
        process_stands(control_data, stands);
    }
    catch(std::exception& e) {
        std::cout << "General exception while running simulation:" << "\n";
        std::cout << e.what();
    }

    return 0;
}