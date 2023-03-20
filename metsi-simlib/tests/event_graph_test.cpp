#define BOOST_TEST_MODULE event_graph
#include <functional>
#include <boost/test/unit_test.hpp>
#include <metsi-simlib/event_graph.hpp>

StateReference<int> event(StateReference<int> a) {
    *a += 1;
    return a;
}

BOOST_AUTO_TEST_CASE(event_graph_initialization) {
    EventNode<int> g = EventDAG<int>::new_node(event);
    LeafNodes<int> leaves = g->collect_leaf_nodes();
    BOOST_CHECK(leaves.size() == 1);
}

BOOST_AUTO_TEST_CASE(evalute_sequence) {
    EventNode<int> root = EventDAG<int>::new_node(event);
    EventNode<int> follower = EventDAG<int>::new_node(event);
    root->add_node(follower);

    auto sim_state = std::make_shared<int>(0);
    ResultStates<int> depth_results = root->evaluate_depth(sim_state);
    BOOST_CHECK(depth_results.size() == 1);
    BOOST_CHECK(*depth_results.front() == 2);
}

BOOST_AUTO_TEST_CASE(evaluate_three_paths_graph) {
    /*
    root -> b1   -> leaf1           => 3
         -> b2s1 -> leaf1           => 3
                 -> b2s2   -> leaf2 => 4
    */
    EventNode<int> root = EventDAG<int>::new_node(event);
    EventNode<int> b1 = EventDAG<int>::new_node(event);
    EventNode<int> b2s1 = EventDAG<int>::new_node(event);
    EventNode<int> b2s2 = EventDAG<int>::new_node(event);
    EventNode<int> leaf1 = EventDAG<int>::new_node(event);
    EventNode<int> leaf2 = EventDAG<int>::new_node(event);

    root->add_node(b1);
    b1->add_node(leaf1);

    root->add_node(b2s1);
    b2s1->add_node(leaf1);
    b2s1->add_node(b2s2);
    b2s2->add_node(leaf2);

    auto sim_state = std::make_shared<int>(0);
    ResultStates<int> depth_results = root->evaluate_depth(sim_state);
    BOOST_CHECK(root->collect_leaf_nodes().size() == 2);
    BOOST_CHECK(b1->collect_leaf_nodes().size() == 1);
    BOOST_CHECK(b2s1->collect_leaf_nodes().size() == 2);
    BOOST_CHECK(b2s2->collect_leaf_nodes().size() == 1);
    BOOST_CHECK(leaf1->collect_leaf_nodes().size() == 1);
    BOOST_CHECK(leaf2->collect_leaf_nodes().size() == 1);
    BOOST_CHECK(depth_results.size() == 3);
    BOOST_CHECK(*depth_results[0] == 3);
    BOOST_CHECK(*depth_results[1] == 3);
    BOOST_CHECK(*depth_results[2] == 4);
}
