#define BOOST_TEST_MODULE branching_generators
#include <boost/test/unit_test.hpp>
#include <set>
#include "branching_generators.hpp"

std::shared_ptr<int> increment(std::shared_ptr<int> a) {
    *a += 1;
    return a;
}

std::shared_ptr<int> decrement(std::shared_ptr<int> a) {
    *a -= 1;
    return a;
}

BOOST_AUTO_TEST_CASE(sequence_works) {
    OperationChain<int> operations;
    operations.emplace_back(increment);
    operations.emplace_back(increment);
    operations.emplace_back(increment);

    EventNode<int> root = EventDAG<int>::new_node(increment);
    EventNode<int> b1 = EventDAG<int>::new_node(increment);
    EventNode<int> b2 = EventDAG<int>::new_node(increment);
    root->add_node(b1);
    root->add_node(b2);

    LeafNodes<int> leafs = root->collect_leaf_nodes();
    BOOST_CHECK(leafs.size() == 2);
    BOOST_CHECK(leafs.contains(b1) == true);
    BOOST_CHECK(leafs.contains(b2) == true);

    LeafNodes<int> new_leafs = sequence<int>(leafs, operations);
    BOOST_CHECK(new_leafs.size() == 1);

    auto payload = std::make_shared<int>(0);
    OperationResults<int> results = root->evaluate_depth(payload);
    BOOST_CHECK(results.size() == 2);
    BOOST_CHECK(*results[0] == 5);
    BOOST_CHECK(*results[1] == 5);
}

BOOST_AUTO_TEST_CASE(alternatives_works) {
    OperationChain<int> operations;
    operations.emplace_back(increment);
    operations.emplace_back(increment);

    EventNode<int> root = EventDAG<int>::new_node(increment);
    LeafNodes<int> leafs = root->collect_leaf_nodes();
    BOOST_CHECK(leafs.size() == 1);
    BOOST_CHECK(leafs.contains(root));

    LeafNodes<int> new_leafs = alternatives<int>(leafs, operations);
    BOOST_CHECK(new_leafs.size() == 2);

    auto payload = std::make_shared<int>(0);
    OperationResults<int> results = root->evaluate_depth(payload);
    BOOST_CHECK(results.size() == 2);
    BOOST_CHECK(*results[0] == 2);
    BOOST_CHECK(*results[1] == 2);
}

BOOST_AUTO_TEST_CASE(generator_combination_works) {
    OperationChain<int> operations;
    operations.emplace_back(increment);
    operations.emplace_back(increment);

    EventNode<int> root = EventDAG<int>::new_node(increment);
    LeafNodes<int> level_0 = root->collect_leaf_nodes();
    BOOST_CHECK(level_0.size() == 1);
    LeafNodes<int> level_1 = sequence<int>(level_0, operations);
    BOOST_CHECK(level_1.size() == 1);
    LeafNodes<int> level_2 = alternatives<int>(level_1, operations);
    BOOST_CHECK(level_2.size() == 2);
    LeafNodes<int> level_3 = sequence<int>(level_2, operations);
    BOOST_CHECK(level_3.size() == 1);
    LeafNodes<int> level_4 = alternatives<int>(level_3, operations);
    BOOST_CHECK(level_4.size() == 2);

    LeafNodes<int> full_tree_leafs = root->collect_leaf_nodes();
    BOOST_CHECK(full_tree_leafs.size() == 2);

    auto payload = std::make_shared<int>(0);
    OperationResults<int> results = root->evaluate_depth(payload);
    BOOST_CHECK(results.size() == 4);
    BOOST_CHECK(*results[0] == 7);
    BOOST_CHECK(*results[1] == 7);
    BOOST_CHECK(*results[2] == 7);
    BOOST_CHECK(*results[3] == 7);
}

BOOST_AUTO_TEST_CASE(multiple_operations_work) {
    OperationChain<int> operations;
    operations.emplace_back(increment);
    operations.emplace_back(decrement);

    EventNode<int> root = EventDAG<int>::new_node(increment);
    LeafNodes<int> level_0 = root->collect_leaf_nodes();
    LeafNodes<int> level_1 = sequence<int>(level_0, operations);
    LeafNodes<int> level_2 = alternatives<int>(level_1, operations);
    LeafNodes<int> level_3 = sequence<int>(level_2, operations);
    LeafNodes<int> level_4 = alternatives<int>(level_3, operations);
    BOOST_CHECK(level_4.size() == 2); // tree would have 2*2 leafs, equivalent directed graph has 2

    auto payload = std::make_shared<int>(0);
    OperationResults<int> results = root->evaluate_depth(payload);
    BOOST_CHECK(results.size() == 4); // 4 total unique paths from root to leafs via the 2*2 alternatives
    // sequences result in no change as increment, decrement.
    BOOST_CHECK(*results[0] == 3);  // alternatives increment, increment
    BOOST_CHECK(*results[1] == 1);  // alternatives increment, decrement
    BOOST_CHECK(*results[2] == 1);  // alternatives decrement, increment
    BOOST_CHECK(*results[3] == -1); // alternatives decrement, decrement
}

BOOST_AUTO_TEST_CASE(generator_resolution) {
    OperationChain<int> operations;
    operations.emplace_back(increment);
    operations.emplace_back(increment);

    EventNode<int> root = EventDAG<int>::new_node(increment);
    LeafNodes<int> level_0 = root->collect_leaf_nodes();
    BOOST_CHECK(level_0.size() == 1);
    LeafNodes<int> level_1 = generator_by_name<int>("sequence").value()(level_0, operations);
    BOOST_CHECK(level_1.size() == 1);
    LeafNodes<int> level_2 = generator_by_name<int>("alternatives").value()(level_1, operations);
    BOOST_CHECK(level_2.size() == 2);
    auto payload = std::make_shared<int>(0);
    OperationResults<int> results = root->evaluate_depth(payload);
    BOOST_CHECK(results.size() == 2);
    BOOST_CHECK(*results[0] == 4);
    BOOST_CHECK(*results[1] == 4);

    auto none = generator_by_name<int>("unknown");
    BOOST_CHECK(none.has_value() == false);
}
