#define BOOST_TEST_MODULE event_graph
#include <functional>
#include <boost/test/unit_test.hpp>
#include "event_graph.hpp"

int op(int a) {
    return a + 1;
};

BOOST_AUTO_TEST_CASE(event_graph_initialization)
{
    EventNode<int> g = std::make_shared<EventDAG<int>>(op);
    LeafNodes<int> leaves = g->collect_leaf_nodes();
    BOOST_CHECK(leaves.size() == 1);
};

BOOST_AUTO_TEST_CASE(evalute_sequence)
{
    EventNode<int> root = std::make_shared<EventDAG<int>>(op);
    EventNode<int> follower = std::make_shared<EventDAG<int>>(op);
    root->add_node(follower);
    OperationResults<int> depth_results = root->evaluate_depth(0);
    BOOST_CHECK(depth_results.size() == 1);
    BOOST_CHECK(depth_results.front() == 2);
};

BOOST_AUTO_TEST_CASE(evaluate_three_paths_graph)
{
    /*
    root -> b1   -> leaf1           => 3
         -> b2s1 -> leaf1           => 3
                 -> b2s2   -> leaf2 => 4
    */
    EventNode<int> root = std::make_shared<EventDAG<int>>(op);
    EventNode<int> b1 = std::make_shared<EventDAG<int>>(op);
    EventNode<int> b2s1 = std::make_shared<EventDAG<int>>(op);
    EventNode<int> b2s2 = std::make_shared<EventDAG<int>>(op);
    EventNode<int> leaf1 = std::make_shared<EventDAG<int>>(op);
    EventNode<int> leaf2 = std::make_shared<EventDAG<int>>(op);

    root->add_node(b1);
    b1->add_node(leaf1);

    root->add_node(b2s1);
    b2s1->add_node(leaf1);
    b2s1->add_node(b2s2);
    b2s2->add_node(leaf2);

    OperationResults<int> depth_results = root->evaluate_depth(0);
    BOOST_CHECK(root->collect_leaf_nodes().size() == 2);
    BOOST_CHECK(b1->collect_leaf_nodes().size() == 1);
    BOOST_CHECK(b2s1->collect_leaf_nodes().size() == 2);
    BOOST_CHECK(b2s2->collect_leaf_nodes().size() == 1);
    BOOST_CHECK(leaf1->collect_leaf_nodes().size() == 1);
    BOOST_CHECK(leaf2->collect_leaf_nodes().size() == 1);
    BOOST_CHECK(depth_results.size() == 3);
    BOOST_CHECK(depth_results[0] == 3);
    BOOST_CHECK(depth_results[1] == 3);
    BOOST_CHECK(depth_results[2] == 4);
};