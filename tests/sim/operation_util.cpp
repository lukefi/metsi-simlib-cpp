#define BOOST_TEST_MODULE operation_util
#include <boost/test/unit_test.hpp>
#include <operation_util.hpp>

std::shared_ptr<int> increment_param(std::shared_ptr<int> val, std::map<std::string, std::string> params) {
    int amount = std::stoi(params["amount"]);
    *val += amount;
    return val;
}

BOOST_AUTO_TEST_CASE(parametered_operation_preparation) {
    std::map<std::string, std::string> params{{"amount", "2"}};
    SimOperation<int> op = parameter_bound_operation<int>(increment_param, params);
    auto start = std::make_shared<int>(1);
    auto l1 = op(start);
    auto l2 = op(l1);
    BOOST_CHECK(*l2 == 5);
}

BOOST_AUTO_TEST_CASE(unaliased_operation_default_parameters) {
    /* Test a simple
     * operation_params:
     *   base_operation:
     *     value: 1
     * simulation_events:
     *   - time_points: [0]
     *     generators:
     *       - sequence:
     *         - base_operation
     * */
    Parameters default_parameters{{"value", "1"}};
    OperationsToParameters defaults{{"base_operation", default_parameters}};

    Parameters expected_parameters{{"value", "1"}};
    auto expected_result = std::make_pair(std::string{"base_operation"}, expected_parameters);
    auto result = resolve_operation_parameters("base_operation", defaults, {}, {});
    BOOST_CHECK(result == expected_result);
}

BOOST_AUTO_TEST_CASE(operation_single_alias) {
    /*
     * operation_aliases:
     *   alias_operation:
     *     base_operation:
     *       value: 1
     * simulation_events:
     *   - time_points: [0]
     *     generators:
     *       - sequence:
     *         - alias_operation
     * */
    Parameters alias_override{{"value", "1"}};
    OperationWithParameters alias{"base_operation", alias_override};
    OperationAliasMap aliases{{"alias_operation", alias}};

    auto expected_result = std::make_pair(std::string{"base_operation"}, alias_override);
    auto result = resolve_operation_parameters("alias_operation", {}, aliases, {});
    BOOST_CHECK(result == expected_result);
}

BOOST_AUTO_TEST_CASE(operation_chained_alias) {
    /*
     * operation_aliases:
     *   top_operation:
     *     alias1_operation:
     *       value: 7             <-- highest precedence override
     *   alias1_operation:
     *     alias2_operation:
     *       value: 2
     *       other: 1             <-- highest precedence override
     *   alias2_operation:
     *     base_operation:
     *       value: 5
     *       other: 10
     *       another: 20
     * simulation_events:
     *   - time_points: [0]
     *     generators:
     *       - sequence:
     *         - top_operation
     * */
    Parameters alias2_override{{"value", "5"}, {"other", "10"}, {"another", "20"}};
    Parameters alias1_override{{"value", "2"}, {"other", "1"}};
    Parameters top_override{{"value", "7"}};
    OperationWithParameters alias2{"base_operation", alias2_override};
    OperationWithParameters alias1{"alias2_operation", alias1_override};
    OperationWithParameters top{"alias1_operation", top_override};
    OperationAliasMap aliases{
        {"top_operation", top},
        {"alias1_operation", alias1},
        {"alias2_operation", alias2}
    };

    Parameters expected_result_parameters{{"value", "7"}, {"other", "1"}, {"another", "20"}};
    auto expected_result = std::make_pair(std::string{"base_operation"}, expected_result_parameters);

    auto result = resolve_operation_parameters("top_operation", {}, aliases, {});
    BOOST_CHECK(result == expected_result);
}

BOOST_AUTO_TEST_CASE(operation_chained_alias_with_inline_parameters) {
    /*
     * operation_aliases:
     *   top_operation:
     *     alias1_operation:
     *       value: 7
     *   alias1_operation:
     *     alias2_operation:
     *       value: 2
     *       other: 1             <-- highest precedence override
     *   alias2_operation:
     *     base_operation:
     *       value: 5
     *       other: 10
     *       another: 20
     * simulation_events:
     *   - time_points: [0]
     *     generators:
     *       - sequence:
     *         - top_operation:
     *             value: 50      <-- highest precedence override
     * */
    Parameters alias2_override{{"value", "5"}, {"other", "10"}, {"another", "20"}};
    Parameters alias1_override{{"value", "2"}, {"other", "1"}};
    Parameters top_override{{"value", "7"}};
    Parameters top_inline{{"value", "50"}};
    OperationWithParameters alias2{"base_operation", alias2_override};
    OperationWithParameters alias1{"alias2_operation", alias1_override};
    OperationWithParameters top{"alias1_operation", top_override};
    OperationAliasMap aliases{
            {"top_operation", top},
            {"alias1_operation", alias1},
            {"alias2_operation", alias2}
    };

    Parameters expected_result_parameters{{"value", "50"}, {"other", "1"}, {"another", "20"}};
    auto expected_result = std::make_pair(std::string{"base_operation"}, expected_result_parameters);

    auto result = resolve_operation_parameters("top_operation", {}, aliases, top_inline);
    BOOST_CHECK(result == expected_result);
}

BOOST_AUTO_TEST_CASE(operation_chained_alias_with_inline_parameters_and_default_parameters) {
    /*
     * operation_params:
     *   base_operation:
     *     number_of_the_beast: 666
     *   some_unused_operation:
     *     it_is_over: 9000
     * operation_aliases:
     *   top_operation:
     *     alias1_operation:
     *       value: 7
     *   alias1_operation:
     *     alias2_operation:
     *       value: 2
     *       other: 1             <-- highest precedence override
     *   alias2_operation:
     *     base_operation:
     *       value: 5
     *       other: 10
     *       another: 20
     * simulation_events:
     *   - time_points: [0]
     *     generators:
     *       - sequence:
     *         - top_operation:
     *             value: 50      <-- highest precedence override
     * */
    Parameters base_parameters{{"number_of_the_beast", "666"}};
    Parameters unused_parameters{{"it_is_over", "9000"}};
    OperationsToParameters default_parameters{
            {"base_operation", base_parameters},
            {"some_unused_operation", unused_parameters}
    };

    Parameters alias2_override{{"value", "5"}, {"other", "10"}, {"another", "20"}};
    Parameters alias1_override{{"value", "2"}, {"other", "1"}};
    Parameters top_override{{"value", "7"}};
    Parameters top_inline{{"value", "50"}};
    OperationWithParameters alias2{"base_operation", alias2_override};
    OperationWithParameters alias1{"alias2_operation", alias1_override};
    OperationWithParameters top{"alias1_operation", top_override};
    OperationAliasMap aliases{
            {"top_operation", top},
            {"alias1_operation", alias1},
            {"alias2_operation", alias2}
    };

    Parameters expected_result_parameters{{"value", "50"}, {"other", "1"}, {"another", "20"}, {"number_of_the_beast", "666"}};
    auto expected_result = std::make_pair(std::string{"base_operation"}, expected_result_parameters);

    auto result = resolve_operation_parameters("top_operation", default_parameters, aliases, top_inline);
    BOOST_CHECK(result == expected_result);
}
