#define BOOST_TEST_MODULE event_graph
#include <functional>
#include <boost/test/unit_test.hpp>
#include "event_graph.hpp"

int op(int a) {
    return a + 1;
};

BOOST_AUTO_TEST_CASE(event_graph_initialization)
{
    EventDAG<int> g(op);
    std::vector<EventDAG<int>> leaves = g.find_leaf_nodes();
    BOOST_CHECK(leaves.size() == 0);
};

BOOST_AUTO_TEST_CASE(evalute_sequence)
{
    EventDAG<int> root(op);
    root.add_branch(op);
    std::vector<int> depth_results = root.evaluate_depth(0);
    BOOST_CHECK(depth_results.size() == 1);
    BOOST_CHECK(depth_results.front() == 2);
};