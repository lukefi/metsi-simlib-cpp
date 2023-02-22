#define BOOST_TEST_MODULE sim_configuration
#include <boost/test/unit_test.hpp>
#include <sim_configuration.hpp>
#include <file_io.hpp>

BOOST_AUTO_TEST_CASE(default_parameters_parsing) {
    YAML::Node content = read_yaml("operation_aliasing.yaml");
    YAML::Node params_node = content["operation_params"];
    OperationsToParameters default_params = parse_default_parameters(params_node);
    BOOST_CHECK(default_params.contains("operation1") == true);
    BOOST_CHECK(default_params.contains("operation2") == false);

    Parameters params = default_params["operation1"];
    auto param1 = params["param1"];
    auto param2 = params["param2"];
    BOOST_CHECK(param1 == "1");
    BOOST_CHECK(param2 == "2");
}

BOOST_AUTO_TEST_CASE(aliased_operations_parsing) {
    YAML::Node content = read_yaml("operation_aliasing.yaml");
    YAML::Node aliases_node = content["operation_aliases"];
    OperationAliasMap operation_aliases = parse_operation_aliases(aliases_node);
    BOOST_CHECK(operation_aliases.size() == 4);

    auto alias1 = operation_aliases["alias"];
    auto alias2 = operation_aliases["alias2"];
    auto alias3 = operation_aliases["alias3"];
    auto alias4 = operation_aliases["alias4"];
    Parameters aliased_param1{{"param1", "3"}};
    OperationWithParameters expected1{"operation", aliased_param1};
    Parameters aliased_param2{{"param2", "4"}};
    OperationWithParameters expected2{"operation", aliased_param2};
    Parameters aliased_param3{{"param1", "10"}};
    OperationWithParameters expected3{"alias2", aliased_param3};
    OperationWithParameters expected4{"alias2", {}};
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
    YAML::Node content = read_yaml("simulation_events_non_nested.yaml");
    auto events = content["simulation_events"];
    auto results = parse_time_points_from_events_structure(events);
    std::set<int> expected{0,5,10};
    BOOST_CHECK(results == expected);
}

BOOST_AUTO_TEST_CASE(generators_parsing_by_time) {
    YAML::Node content = read_yaml("simulation_events_non_nested.yaml");
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
    YAML::Node content = read_yaml("simulation_events_non_nested.yaml");
    auto events = content["simulation_events"];
    auto results = parse_simulation_events(events);
    BOOST_CHECK(results.size() == 3);

    auto time0_root = results.at(0);
    auto time5_root = results.at(5);
    auto time10_root = results.at(10);
    BOOST_CHECK(time0_root.get_nested().size() == 2);
    BOOST_CHECK(time0_root.get_operations().empty() == true);
    BOOST_CHECK(time0_root.get_nested()[0].get_operations().size() == 1);
    BOOST_CHECK(time0_root.get_nested()[0].get_operations()[0].first == "alpha");

    BOOST_CHECK(time5_root.get_nested().size() == 2);

    BOOST_CHECK(time10_root.get_nested().size() == 2);
    BOOST_CHECK(time10_root.get_operations().empty() == true);
    BOOST_CHECK(time10_root.get_nested()[1].get_operations().size() == 1);
    BOOST_CHECK(time10_root.get_nested()[1].get_operations()[0].first == "omega");
}

BOOST_AUTO_TEST_CASE(generators_parsing_nested) {
    YAML::Node content = read_yaml("simulation_events_nested.yaml");
    auto events = content["simulation_events"];
    auto results = parse_simulation_events(events);
    auto time5_root = results.at(5);
    BOOST_CHECK(time5_root.get_nested().size() == 1);
    BOOST_CHECK(time5_root.get_type() == "sequence");

    auto nested = time5_root.get_nested()[0].get_nested();
    BOOST_CHECK(nested[0].get_type() == "alternatives");
    BOOST_CHECK(nested[0].get_nested().empty() == true);
    BOOST_CHECK(nested[0].get_operations().size() == 1);
    BOOST_CHECK(nested[0].get_operations()[0].first == "choke_point");

    BOOST_CHECK(nested[1].get_type() == "sequence");
    BOOST_CHECK(nested[1].get_nested().size() == 2);
    BOOST_CHECK(nested[1].get_operations().empty() == true);

    auto level2 = nested[1].get_nested();
    BOOST_CHECK(level2[0].get_type() == "sequence");
    BOOST_CHECK(level2[0].get_operations().size() == 1);
    BOOST_CHECK(level2[0].get_operations()[0].first == "liquidation");
    BOOST_CHECK(level2[0].get_nested().empty() == true);
    BOOST_CHECK(level2[1].get_type() == "alternatives");
    BOOST_CHECK(level2[1].get_operations().size() == 2);
    BOOST_CHECK(level2[1].get_operations()[0].first == "bankrupt");
    BOOST_CHECK(level2[1].get_operations()[1].first == "solvent");

    BOOST_CHECK(nested[2].get_type() == "alternatives");
    BOOST_CHECK(nested[2].get_nested().empty() == true);
    BOOST_CHECK(nested[2].get_operations().size() == 1);
    BOOST_CHECK(nested[2].get_operations()[0].first == "give_me_custody_or_give_me_death");
}