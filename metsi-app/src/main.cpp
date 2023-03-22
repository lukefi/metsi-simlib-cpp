#include <iostream>

#include "app_io.hpp"
#include "file_io.hpp"
#include "sim_configuration.hpp"
#include "sim_events.hpp"
#include <metsi-simlib/core_types.hpp>

std::map<std::string, ResultStates<SimulationState>> do_simulate(const YAML::Node& control_data, const std::vector<ForestStand>& source_data) {
    auto alias_resolver = alias_resolver_closure(control_data);
    auto event_resolver = parameterized_event_resolver_closure<SimulationState>(alias_resolver, base_event_resolver);
    auto simulation = prepare_simulation<SimulationState>(event_resolver, control_data["simulation_events"]);

    std::map<std::string, ResultStates<SimulationState>> results;
    for(const auto& stand : source_data) {
        ResultStates<SimulationState> stand_states{std::make_shared<SimulationState>(stand)};
        auto stand_id = stand.get<std::string>("identifier");

        for(auto pair : simulation) {
            ResultStates<SimulationState> time_point_results;
            for(auto state : stand_states) {
                try {
                    auto current = pair.second->evaluate_depth(state);
                    time_point_results.insert(time_point_results.end(), current.begin(), current.end());
                }
                catch(BranchException& e) {}
            }
            stand_states = time_point_results;
        }
        results[stand_id] = stand_states;
    }
    return results;
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

    std::vector<ForestStand> stands;
    try {
        stands = read_forest_csv(args["input-path"]);
    }
    catch(std::exception&) {
        std::cout << "Unable to read input file. Aborting.\n";
        return 1;
    }

    try {
        auto results = do_simulate(control_data, stands);

        for (auto pair: results) {
            std::cout << "Produced " << pair.second.size() << " schedules for stand " << pair.first << "\n";
        }
    }
    catch(std::exception& e) {
        std::cout << "General exception while running simulation:" << "\n";
        std::cout << e.what();
    }

    return 0;
}