#define BOOST_TEST_MODULE simulation_run
#include <boost/test/unit_test.hpp>
#include <file_io.hpp>
#include <sim_configuration.hpp>
#include <sim_events.hpp>

BOOST_AUTO_TEST_CASE(simple_sequence_run) {
    // this test shows that a full simulation run is preparable and runnable from source files
    auto control_data = read_yaml("test_resources/mock_run_1.yaml");
    auto data = read_forest_csv("test_resources/forest_data.csv");
    auto alias_resolver = alias_resolver_closure(control_data);
    auto event_resolver = parameterized_event_resolver_closure<SimulationState>(alias_resolver, base_event_resolver);
    auto simulation = prepare_simulation<SimulationState>(event_resolver, control_data["simulation_events"]);

    ResultStates<SimulationState> states{std::make_shared<SimulationState>(data[0])};

    for(auto pair : simulation) {
        ResultStates<SimulationState> time_point_results;
        for(auto state : states) {
            auto current = pair.second->evaluate_depth(state);
            time_point_results.insert(time_point_results.end(), current.begin(), current.end());
        }
        states = time_point_results;
    }

    BOOST_CHECK(simulation.size() == 4);
    BOOST_CHECK(states.size() == 1);
}
