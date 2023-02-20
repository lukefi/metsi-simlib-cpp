#define BOOST_TEST_MODULE sim_configuration
#include <boost/test/unit_test.hpp>
#include <sim_configuration.hpp>
#include <file_io.hpp>

BOOST_AUTO_TEST_CASE(default_parameters_parsing) {
    YAML::Node content = read_yaml("test_control.yaml");
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
    YAML::Node content = read_yaml("test_control.yaml");
    YAML::Node aliases_node = content["operation_aliases"];
    OperationAliasMap operation_aliases = parse_operation_aliases(aliases_node);
    BOOST_CHECK(operation_aliases.size() == 3);

    auto alias1 = operation_aliases["alias"];
    auto alias2 = operation_aliases["alias2"];
    auto alias3 = operation_aliases["alias3"];
    Parameters aliased_param1{{"param1", "3"}};
    OperationsToParameters expected1{{"operation", aliased_param1}};
    Parameters aliased_param2{{"param2", "4"}};
    OperationsToParameters expected2{{"operation", aliased_param2}};
    Parameters aliased_param3{{"param1", "10"}};
    OperationsToParameters expected3{{"alias2", aliased_param3}};
    BOOST_CHECK(alias1 == expected1);
    BOOST_CHECK(alias2 == expected2);
    BOOST_CHECK(alias3 == expected3);
}