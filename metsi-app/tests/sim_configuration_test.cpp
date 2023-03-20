#define BOOST_TEST_MODULE sim_configuration
#include <boost/test/unit_test.hpp>
#include <sim_configuration.hpp>
#include <file_io.hpp>

BOOST_AUTO_TEST_CASE(default_parameters_parsing) {
    YAML::Node content = read_yaml("test_resources/event_aliasing.yaml");
    YAML::Node params_node = content["event_parameters"];
    EventLabelsWithParameters default_params = parse_default_parameters(params_node);
    BOOST_CHECK(default_params.contains("base_event") == true);
    BOOST_CHECK(default_params.contains("bad_format") == false);

    EventParameters params = default_params["base_event"];
    EventParameters expectation = {
            {"param1", "1"},
            {"param2", "1"},
            {"param3", "1"},
            {"param4", "1"}
    };
    BOOST_CHECK(expectation == params);
}

BOOST_AUTO_TEST_CASE(aliased_events_parsing) {
    YAML::Node content = read_yaml("test_resources/event_aliasing.yaml");
    YAML::Node aliases_node = content["event_aliases"];
    EventLabelAliases event_aliases = parse_event_aliases(aliases_node);
    BOOST_CHECK(event_aliases.size() == 4);

    auto alias1 = event_aliases["aliased_event_1"];
    auto alias2 = event_aliases["aliased_event_2"];
    auto alias3 = event_aliases["aliased_event_3"];
    auto alias4 = event_aliases["aliased_event_4"];
    EventParameters aliased_param1{{"param1", "2"}};
    EventParameters aliased_param2{{"param2", "2"}, {"param5", "2"}};
    EventParameters aliased_param3{{"param2", "2"}, {"param3", "2"}};
    EventParameters aliased_param4{};
    EventLabelWithParameters expected1{"base_event", aliased_param1};
    EventLabelWithParameters expected2{"base_event", aliased_param2};
    EventLabelWithParameters expected3{"aliased_event_2", aliased_param3};
    EventLabelWithParameters expected4{"some_other_event", {}};
    BOOST_CHECK(alias1 == expected1);
    BOOST_CHECK(alias2 == expected2);
    BOOST_CHECK(alias3 == expected3);
    BOOST_CHECK(alias4 == expected4);
}

BOOST_AUTO_TEST_CASE(unique_set_parsing) {
    YAML::Node seq = YAML::Load("[3, 2, 2, 1]");
    auto result = sequence_as_set<int>(seq);
    std::set<int> expected{1,2,3};
    BOOST_CHECK(result == expected);
}

BOOST_AUTO_TEST_CASE(time_points_parsing) {
    YAML::Node content = read_yaml("test_resources/simulation_events_non_nested.yaml");
    auto events = content["simulation_events"];
    auto results = parse_time_points_from_events_structure(events);
    std::set<int> expected{0,5,10};
    BOOST_CHECK(results == expected);
}

BOOST_AUTO_TEST_CASE(generators_parsing_by_time) {
    YAML::Node content = read_yaml("test_resources/simulation_events_non_nested.yaml");
    auto events = content["simulation_events"];
    auto time0_generators = find_generator_blocks_for_time(0, events);
    auto time5_generators = find_generator_blocks_for_time(5, events);
    auto time10_generators = find_generator_blocks_for_time(10, events);
    BOOST_CHECK(time0_generators.size() == 2);
    BOOST_CHECK(time5_generators.size() == 2);
    BOOST_CHECK(time10_generators.size() == 2);
    BOOST_CHECK(time0_generators[1] == time5_generators[0]);
    BOOST_CHECK(time5_generators[0] == time10_generators[0]);
    BOOST_CHECK(time0_generators[0] != time0_generators[1]);
    BOOST_CHECK(time5_generators[0] != time5_generators[1]);
    BOOST_CHECK(time10_generators[0] != time10_generators[1]);
}

BOOST_AUTO_TEST_CASE(generators_parsing_non_nested) {
    YAML::Node content = read_yaml("test_resources/simulation_events_non_nested.yaml");
    auto events = content["simulation_events"];
    auto results = parse_simulation_events(events);
    BOOST_CHECK(results.size() == 3);

    auto time0_root = results.at(0);
    auto time5_root = results.at(5);
    auto time10_root = results.at(10);
    BOOST_CHECK(time0_root.get_nested_generator_prototypes().size() == 2);
    BOOST_CHECK(time0_root.get_event_prototypes().empty() == true);
    BOOST_CHECK(time0_root.get_nested_generator_prototypes()[0].get_event_prototypes().size() == 1);
    BOOST_CHECK(time0_root.get_nested_generator_prototypes()[0].get_event_prototypes()[0].first == "alpha");

    BOOST_CHECK(time5_root.get_nested_generator_prototypes().size() == 2);

    BOOST_CHECK(time10_root.get_nested_generator_prototypes().size() == 2);
    BOOST_CHECK(time10_root.get_event_prototypes().empty() == true);
    BOOST_CHECK(time10_root.get_nested_generator_prototypes()[1].get_event_prototypes().size() == 1);
    BOOST_CHECK(time10_root.get_nested_generator_prototypes()[1].get_event_prototypes()[0].first == "omega");
}

BOOST_AUTO_TEST_CASE(generators_parsing_nested) {
    YAML::Node content = read_yaml("test_resources/simulation_events_nested.yaml");
    auto events = content["simulation_events"];
    auto results = parse_simulation_events(events);
    auto time5_root = results.at(5);
    BOOST_CHECK(time5_root.get_nested_generator_prototypes().size() == 1);
    BOOST_CHECK(time5_root.get_type() == "sequence");

    auto nested = time5_root.get_nested_generator_prototypes()[0].get_nested_generator_prototypes();
    BOOST_CHECK(nested[0].get_type() == "alternatives");
    BOOST_CHECK(nested[0].get_nested_generator_prototypes().empty() == true);
    BOOST_CHECK(nested[0].get_event_prototypes().size() == 1);
    BOOST_CHECK(nested[0].get_event_prototypes()[0].first == "choke_point");

    BOOST_CHECK(nested[1].get_type() == "sequence");
    BOOST_CHECK(nested[1].get_nested_generator_prototypes().size() == 2);
    BOOST_CHECK(nested[1].get_event_prototypes().empty() == true);

    auto level2 = nested[1].get_nested_generator_prototypes();
    BOOST_CHECK(level2[0].get_type() == "sequence");
    BOOST_CHECK(level2[0].get_event_prototypes().size() == 1);
    BOOST_CHECK(level2[0].get_event_prototypes()[0].first == "liquidation");
    BOOST_CHECK(level2[0].get_nested_generator_prototypes().empty() == true);
    BOOST_CHECK(level2[1].get_type() == "alternatives");
    BOOST_CHECK(level2[1].get_event_prototypes().size() == 2);
    BOOST_CHECK(level2[1].get_event_prototypes()[0].first == "bankrupt");
    BOOST_CHECK(level2[1].get_event_prototypes()[1].first == "solvent");

    BOOST_CHECK(nested[2].get_type() == "alternatives");
    BOOST_CHECK(nested[2].get_nested_generator_prototypes().empty() == true);
    BOOST_CHECK(nested[2].get_event_prototypes().size() == 1);
    BOOST_CHECK(nested[2].get_event_prototypes()[0].first == "give_me_custody_or_give_me_death");
}

BOOST_AUTO_TEST_CASE(test_alias_resolver_closure) {
    auto content = read_yaml("test_resources/event_aliasing.yaml");
    auto resolver = alias_resolver_closure(content);
    auto events_node = content["simulation_events"];
    auto results = parse_simulation_events(events_node);
    auto events = results.at(0).get_nested_generator_prototypes()[0].get_event_prototypes();
    BOOST_CHECK(events.size() == 7);

    {
        auto event = resolver(events[0]);
        EventParameters params{{"param1", "4"},
                               {"param2", "1"},
                               {"param3", "1"},
                               {"param4", "1"}};
        EventLabelWithParameters expected{"base_event", params};
        BOOST_CHECK(event == expected);
    }
    {
        auto event = resolver(events[1]);
        EventParameters params{{"param1", "1"},
                                {"param2", "2"},
                                {"param3", "4"},
                                {"param4", "1"},
                                {"param5", "2"}};
        EventLabelWithParameters expected{"base_event", params};
        BOOST_CHECK(event == expected);
    }
    {
        auto event = resolver(events[2]);
        EventParameters params{{"param1", "1"},
                                {"param2", "2"},
                                {"param3", "2"},
                                {"param4", "1"},
                                {"param5", "2"},
                                {"param6", "4"}};
        EventLabelWithParameters expected{"base_event", params};
        BOOST_CHECK(event == expected);
    }
    {
        auto event = resolver(events[3]);
        EventParameters params{};
        EventLabelWithParameters expected{"some_other_event", params};
        BOOST_CHECK(event == expected);
    }
    {
        auto event = resolver(events[4]);
        EventParameters params{{"param1", "1"},
                               {"param2", "1"},
                               {"param3", "1"},
                               {"param4", "1"}};
        EventLabelWithParameters expected{"base_event", params};
        BOOST_CHECK(event == expected);
    }
    {
        auto event = resolver(events[5]);
        EventParameters params{};
        EventLabelWithParameters expected{"some_other_event", params};
        BOOST_CHECK(event == expected);
    }
    {
        auto event = resolver(events[6]);
        EventParameters params{};
        EventLabelWithParameters expected{"bad_format_parameters_event", params};
        BOOST_CHECK(event == expected);
    }
}

template<typename T> StateReference<T> increment_param(StateReference<int> val, EventParameters params) {
    int amount = std::stoi(params["amount"]);
    *val += amount;
    return val;
};

template<typename T> StateReference<T> decrement_param(StateReference<T> val, EventParameters params) {
    int amount = std::stoi(params["amount"]);
    *val -= amount;
    return val;
};

template<typename T> ParameterizedEventFn<T> base_event_resolver(const EventLabelWithParameters& event_prototype) {
    if(event_prototype.first == "inc") {
        return ::increment_param<T>;
    }
    else {
        return ::decrement_param<T>;
    }
}

/**
 * Run a full simulation based on a known control YAML file with known event functions. The simulation is a nested
 * generator simulation with more than one time point and event aliasing. This test implements a simple runner and
 * result collector for the simulation.
 */
BOOST_AUTO_TEST_CASE(full_simulation_test) {
    auto control_yaml = read_yaml("test_resources/full_simulation_test.yaml");
    AliasResolver alias_resolver = alias_resolver_closure(control_yaml);
    EventFnResolver<int> event_resolver = parameterized_event_resolver_closure<int>(
            alias_resolver,
            ::base_event_resolver<int>);
    auto events = prepare_simulation<int>(event_resolver, control_yaml["simulation_events"]);

    StateReference<int> initial_state = std::make_shared<int>(0);
    ResultStates<int> results{initial_state};
    for(auto time_pair : events) {
        ResultStates<int> time_point_results;
        for(auto state : results) {
            auto next_states = time_pair.second->evaluate_depth(state);
            time_point_results.insert(time_point_results.end(), next_states.begin(), next_states.end());
        }
        results = time_point_results;
    }
    BOOST_CHECK(results.size() == 4);
    BOOST_CHECK(*results[0] == 14);
    BOOST_CHECK(*results[1] == 8);
    BOOST_CHECK(*results[2] == 8);
    BOOST_CHECK(*results[3] == 2);
}