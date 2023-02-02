#define BOOST_TEST_MODULE branching_generators
#include <functional>
#include <boost/test/unit_test.hpp>
#include <vector>
#include <set>
#include <memory>
#include "branching_generators.hpp"

int increment(int a) {
    return a + 1;
};

int decrement(int a) {
    return a - 1;
}

BOOST_AUTO_TEST_CASE(sequence_works) 
{
    std::vector<typename Operation<int>::fn> operations;
    operations.push_back(increment);
    operations.push_back(increment);
    operations.push_back(increment);

    std::shared_ptr<EventDAG<int>> root = std::make_shared<EventDAG<int>>(increment);
    std::shared_ptr<EventDAG<int>> b1 = std::make_shared<EventDAG<int>>(increment);
    std::shared_ptr<EventDAG<int>> b2 = std::make_shared<EventDAG<int>>(increment);
    root->add_node(b1);
    root->add_node(b2);

    std::set<std::shared_ptr<EventDAG<int>>> leafs = root->collect_leaf_nodes();
    BOOST_CHECK(leafs.size() == 2);
    BOOST_CHECK(leafs.contains(b1) == true);
    BOOST_CHECK(leafs.contains(b2) == true);

    std::set<std::shared_ptr<EventDAG<int>>> new_leafs = sequence<int>(leafs, operations);
    BOOST_CHECK(new_leafs.size() == 1);

    std::vector<int> results = root->evaluate_depth(0);
    BOOST_CHECK(results.size() == 2);
    BOOST_CHECK(results[0] == 5);
    BOOST_CHECK(results[1] == 5);
};

BOOST_AUTO_TEST_CASE(alternatives_works) 
{
    std::vector<typename Operation<int>::fn> operations;
    operations.push_back(increment);
    operations.push_back(increment);

    std::shared_ptr<EventDAG<int>> root = std::make_shared<EventDAG<int>>(increment);
    std::set<std::shared_ptr<EventDAG<int>>> leafs = root->collect_leaf_nodes();
    BOOST_CHECK(leafs.size() == 1);
    BOOST_CHECK(leafs.contains(root));

    std::set<std::shared_ptr<EventDAG<int>>> new_leafs = alternatives<int>(leafs, operations);
    BOOST_CHECK(new_leafs.size() == 2);

    std::vector<int> results = root->evaluate_depth(0);
    BOOST_CHECK(results.size() == 2);
    BOOST_CHECK(results[0] == 2);
    BOOST_CHECK(results[1] == 2);

    
};

BOOST_AUTO_TEST_CASE(generator_combination_works)
{
    std::vector<typename Operation<int>::fn> operations;
    operations.push_back(increment);
    operations.push_back(increment);

    std::shared_ptr<EventDAG<int>> root = std::make_shared<EventDAG<int>>(increment);
    std::set<std::shared_ptr<EventDAG<int>>> level_0 = root->collect_leaf_nodes();
    BOOST_CHECK(level_0.size() == 1);
    std::set<std::shared_ptr<EventDAG<int>>> level_1 = sequence<int>(level_0, operations);
    BOOST_CHECK(level_1.size() == 1);
    std::set<std::shared_ptr<EventDAG<int>>> level_2 = alternatives<int>(level_1, operations);
    BOOST_CHECK(level_2.size() == 2);
    std::set<std::shared_ptr<EventDAG<int>>> level_3 = sequence<int>(level_2, operations);
    BOOST_CHECK(level_3.size() == 1);
    std::set<std::shared_ptr<EventDAG<int>>> level_4 = alternatives<int>(level_3, operations);
    BOOST_CHECK(level_4.size() == 2);

    std::set<std::shared_ptr<EventDAG<int>>> full_tree_leafs = root->collect_leaf_nodes();
    BOOST_CHECK(full_tree_leafs.size() == 2);

    std::vector<int> results = root->evaluate_depth(0);
    BOOST_CHECK(results.size() == 4);
    BOOST_CHECK(results[0] == 7);
    BOOST_CHECK(results[1] == 7);
    BOOST_CHECK(results[2] == 7);
    BOOST_CHECK(results[3] == 7);
};

BOOST_AUTO_TEST_CASE(multiple_operations_work)
{
    std::vector<typename Operation<int>::fn> operations;
    operations.push_back(increment);
    operations.push_back(decrement);

    std::shared_ptr<EventDAG<int>> root = std::make_shared<EventDAG<int>>(increment);
    std::set<std::shared_ptr<EventDAG<int>>> level_0 = root->collect_leaf_nodes();
    std::set<std::shared_ptr<EventDAG<int>>> level_1 = sequence<int>(level_0, operations);
    std::set<std::shared_ptr<EventDAG<int>>> level_2 = alternatives<int>(level_1, operations);
    std::set<std::shared_ptr<EventDAG<int>>> level_3 = sequence<int>(level_2, operations);
    std::set<std::shared_ptr<EventDAG<int>>> level_4 = alternatives<int>(level_3, operations);

    std::vector<int> results = root->evaluate_depth(0);
    BOOST_CHECK(results.size() == 4);
    // sequences result in no change as increment, decrement.
    BOOST_CHECK(results[0] == 3);  // alternatives increment, increment
    BOOST_CHECK(results[1] == 1);  // alternatives increment, decrement
    BOOST_CHECK(results[2] == 1);  // alternatives decrement, increment
    BOOST_CHECK(results[3] == -1); // alternatives decrement, decrement
}