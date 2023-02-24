#define BOOST_TEST_MODULE framework_util
#include <boost/test/unit_test.hpp>
#include <framework_util.hpp>

std::shared_ptr<int> increment_param(std::shared_ptr<int> val, std::map<std::string, std::string> params) {
    int amount = std::stoi(params["amount"]);
    *val += amount;
    return val;
}

BOOST_AUTO_TEST_CASE(parametered_event_preparation) {
    std::map<std::string, std::string> params{{"amount", "2"}};
    EventFn<int> event = parameterized_event_closure<int>(increment_param, params);
    auto start = std::make_shared<int>(1);
    auto l1 = event(start);
    auto l2 = event(l1);
    BOOST_CHECK(*l2 == 5);
}

BOOST_AUTO_TEST_CASE(unaliased_event_default_parameters) {
    /* Test a simple
     * event_parameters:
     *   base_event:
     *     value: 1
     * simulation_events:
     *   - time_points: [0]
     *     generators:
     *       - sequence:
     *         - base_event
     * */
    EventParameters default_parameters{{"value", "1"}};
    EventLabelsWithParameters defaults{{"base_event", default_parameters}};

    EventParameters expected_parameters{{"value", "1"}};
    auto expected_result = std::make_pair(std::string{"base_event"}, expected_parameters);
    auto result = resolve_event_aliasing("base_event", defaults, {}, {});
    BOOST_CHECK(result == expected_result);
}

BOOST_AUTO_TEST_CASE(event_single_alias) {
    /*
     * event_aliases:
     *   alias_event:
     *     base_event:
     *       value: 1
     * simulation_events:
     *   - time_points: [0]
     *     generators:
     *       - sequence:
     *         - alias_event
     * */
    EventParameters alias_override{{"value", "1"}};
    EventLabelWithParameters alias{"base_event", alias_override};
    EventLabelAliases aliases{{"alias_event", alias}};

    auto expected_result = std::make_pair(std::string{"base_event"}, alias_override);
    auto result = resolve_event_aliasing("alias_event", {}, aliases, {});
    BOOST_CHECK(result == expected_result);
}

BOOST_AUTO_TEST_CASE(event_chained_alias) {
    /*
     * event_aliases:
     *   top_event:
     *     alias1_event:
     *       value: 7             <-- highest precedence override
     *   alias1_event:
     *     alias2_event:
     *       value: 2
     *       other: 1             <-- highest precedence override
     *   alias2_event:
     *     base_event:
     *       value: 5
     *       other: 10
     *       another: 20
     * simulation_events:
     *   - time_points: [0]
     *     generators:
     *       - sequence:
     *         - top_event
     * */
    EventParameters alias2_override{{"value", "5"}, {"other", "10"}, {"another", "20"}};
    EventParameters alias1_override{{"value", "2"}, {"other", "1"}};
    EventParameters top_override{{"value", "7"}};
    EventLabelWithParameters alias2{"base_event", alias2_override};
    EventLabelWithParameters alias1{"alias2_event", alias1_override};
    EventLabelWithParameters top{"alias1_event", top_override};
    EventLabelAliases aliases{
        {"top_event", top},
        {"alias1_event", alias1},
        {"alias2_event", alias2}
    };

    EventParameters expected_result_parameters{{"value", "7"}, {"other", "1"}, {"another", "20"}};
    auto expected_result = std::make_pair(std::string{"base_event"}, expected_result_parameters);

    auto result = resolve_event_aliasing("top_event", {}, aliases, {});
    BOOST_CHECK(result == expected_result);
}

BOOST_AUTO_TEST_CASE(event_chained_alias_with_inline_parameters) {
    /*
     * event_aliases:
     *   top_event:
     *     alias1_event:
     *       value: 7
     *   alias1_event:
     *     alias2_event:
     *       value: 2
     *       other: 1             <-- highest precedence override
     *   alias2_event:
     *     base_event:
     *       value: 5
     *       other: 10
     *       another: 20
     * simulation_events:
     *   - time_points: [0]
     *     generators:
     *       - sequence:
     *         - top_event:
     *             value: 50      <-- highest precedence override
     * */
    EventParameters alias2_override{{"value", "5"}, {"other", "10"}, {"another", "20"}};
    EventParameters alias1_override{{"value", "2"}, {"other", "1"}};
    EventParameters top_override{{"value", "7"}};
    EventParameters top_inline{{"value", "50"}};
    EventLabelWithParameters alias2{"base_event", alias2_override};
    EventLabelWithParameters alias1{"alias2_event", alias1_override};
    EventLabelWithParameters top{"alias1_event", top_override};
    EventLabelAliases aliases{
            {"top_event", top},
            {"alias1_event", alias1},
            {"alias2_event", alias2}
    };

    EventParameters expected_result_parameters{{"value", "50"}, {"other", "1"}, {"another", "20"}};
    auto expected_result = std::make_pair(std::string{"base_event"}, expected_result_parameters);

    auto result = resolve_event_aliasing("top_event", {}, aliases, top_inline);
    BOOST_CHECK(result == expected_result);
}

BOOST_AUTO_TEST_CASE(event_chained_alias_with_inline_parameters_and_default_parameters) {
    /*
     * event_parameters:
     *   base_event:
     *     number_of_the_beast: 666
     *   some_unused_event:
     *     it_is_over: 9000
     * event_aliases:
     *   top_event:
     *     alias1_event:
     *       value: 7
     *   alias1_event:
     *     alias2_event:
     *       value: 2
     *       other: 1             <-- highest precedence override
     *   alias2_event:
     *     base_event:
     *       value: 5
     *       other: 10
     *       another: 20
     * simulation_events:
     *   - time_points: [0]
     *     generators:
     *       - sequence:
     *         - top_event:
     *             value: 50      <-- highest precedence override
     * */
    EventParameters base_parameters{{"number_of_the_beast", "666"}};
    EventParameters unused_parameters{{"it_is_over", "9000"}};
    EventLabelsWithParameters default_parameters{
            {"base_event", base_parameters},
            {"some_unused_event", unused_parameters}
    };

    EventParameters alias2_override{{"value", "5"}, {"other", "10"}, {"another", "20"}};
    EventParameters alias1_override{{"value", "2"}, {"other", "1"}};
    EventParameters top_override{{"value", "7"}};
    EventParameters top_inline{{"value", "50"}};
    EventLabelWithParameters alias2{"base_event", alias2_override};
    EventLabelWithParameters alias1{"alias2_event", alias1_override};
    EventLabelWithParameters top{"alias1_event", top_override};
    EventLabelAliases aliases{
            {"top_event", top},
            {"alias1_event", alias1},
            {"alias2_event", alias2}
    };

    EventParameters expected_result_parameters{{"value", "50"}, {"other", "1"}, {"another", "20"}, {"number_of_the_beast", "666"}};
    auto expected_result = std::make_pair(std::string{"base_event"}, expected_result_parameters);

    auto result = resolve_event_aliasing("top_event", default_parameters, aliases, top_inline);
    BOOST_CHECK(result == expected_result);
}

BOOST_AUTO_TEST_CASE(leaf_generator_prototype) {
    NestableGeneratorPrototype gen{"sequence"};
    EventParameters params{{"param1", "1"}};
    std::vector<EventLabelWithParameters> events{{"event1", params}, {"event2", params}};
    gen.add_event_prototypes(events);
    BOOST_CHECK(gen.get_event_prototypes().size() == 2);
    BOOST_CHECK(gen.get_nested_generator_prototypes().empty() == true);
    BOOST_CHECK(gen.is_leaf() == true);
}

BOOST_AUTO_TEST_CASE(nested_generator_prototype_with_existing_events) {
    NestableGeneratorPrototype gen{"sequence"};
    NestableGeneratorPrototype gen2{"alternatives"};
    EventParameters params{{"param1", "1"}};
    std::vector<EventLabelWithParameters> events{{"event1", params}, {"event2", params}};
    gen.add_event_prototypes(events);
    gen2.add_event_prototypes(events);
    gen.add_nested_generator(gen2);
    BOOST_CHECK(gen.get_event_prototypes().empty() == true);
    BOOST_CHECK(gen.get_nested_generator_prototypes().size() == 2);
    BOOST_CHECK(gen.is_leaf() == false);
}

BOOST_AUTO_TEST_CASE(graph_constructing_from_nested_generator_prototype) {
    auto dummy_resolver = [](EventLabelWithParameters _) { return [](std::shared_ptr<int> val) {
        *val += 1;
        return val;
    };};
    EventParameters params{};
    EventLabelWithParameters event{"inc", params};
    std::vector<EventLabelWithParameters> events{event};
    auto state = std::make_shared<int>(0);
    EventNode<int> root = EventDAG<int>::new_node(dummy_resolver(event));
    LeafNodes<int> nodes{root};

    /*
     * This describes the following event structure and should result in 3 alternative results. The test proves this
     * by performing the computation.
     *                              # initial, 0
     * - inc                        # root, 1
     * - sequence:              # level 0
     *     - sequence:          # level 1_1
     *         - inc                # 2
     *     - alternatives:      # level 1_2
     *         - inc            # wrapped level 1_2_1, 3
     *         - sequence:      # level 1_2_2
     *             - inc            # 3
     *             - inc            # 4
     *         - sequence:      # level 1_2_3
     *             - inc            # 3
     *             - inc            # 4
     *             - inc            # 5
     *     - sequence:          # level 1_3
     *         - inc                # 4, 5, 6
     */
    NestableGeneratorPrototype l0{"sequence"};
    NestableGeneratorPrototype l1_1{"sequence"};
    NestableGeneratorPrototype l1_2{"alternatives"};
    NestableGeneratorPrototype l1_2_2{"sequence"};
    NestableGeneratorPrototype l1_2_3{"sequence"};
    NestableGeneratorPrototype l1_3{"sequence"};

    l1_1.add_event_prototypes(events);
    l1_2.add_event_prototypes(events);

    l1_2_2.add_event_prototypes(events);
    l1_2_2.add_event_prototypes(events);

    l1_2_3.add_event_prototypes(events);
    l1_2_3.add_event_prototypes(events);
    l1_2_3.add_event_prototypes(events);

    l1_2.add_nested_generator(l1_2_2);
    l1_2.add_nested_generator(l1_2_3);

    l1_3.add_event_prototypes(events);

    l0.add_nested_generator(l1_1);
    l0.add_nested_generator(l1_2);
    l0.add_nested_generator(l1_3);

    LeafNodes<int> leafs = construct_event_graph<int>(l0, dummy_resolver, nodes);
    BOOST_CHECK(leafs.size() == 1); // graph final node is from a sequence

    ResultStates<int> results = root->evaluate_depth(state);
    BOOST_CHECK(*results[0] == 4);
    BOOST_CHECK(*results[1] == 5);
    BOOST_CHECK(*results[2] == 6);
}
