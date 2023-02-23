#define BOOST_TEST_MODULE class_type_event_test
#include <boost/test/unit_test.hpp>
#include <branching_generators.hpp>

class TestExample {
private:
    int a;
    std::vector<int> memory;
public:
    TestExample(int _a): a(_a){};
    int get() { return a; }
    void set(int _a) { a = _a; }
    void memorize(int _b) { memory.emplace_back(_b); }
    std::vector<int> memories() { return memory; }
};

std::shared_ptr<TestExample> increment(std::shared_ptr<TestExample> sim_state) {
    sim_state->set(sim_state->get() + 1);
    return sim_state;
};

std::shared_ptr<TestExample> remember(std::shared_ptr<TestExample> sim_state) {
    sim_state->memorize(sim_state->get());
    return sim_state;
};

BOOST_AUTO_TEST_CASE(simple_sequence_with_object) {
    EventChain<TestExample> events{increment, increment};
    auto sim_state = std::make_shared<TestExample>(1);
    EventNode<TestExample> root = EventDAG<TestExample>::new_node(increment);
    LeafNodes<TestExample> l0 = root->collect_leaf_nodes();
    LeafNodes<TestExample> l1 = sequence<TestExample>(l0, events);
    ResultStates<TestExample> results = root->evaluate_depth(sim_state);
    BOOST_CHECK(results[0]->get() == 4);
}

BOOST_AUTO_TEST_CASE(simple_alternatives_with_object) {
    EventChain<TestExample> events{increment, increment};
    auto sim_state = std::make_shared<TestExample>(1);
    EventNode<TestExample> root = EventDAG<TestExample>::new_node(increment);
    LeafNodes<TestExample> l0 = root->collect_leaf_nodes();
    LeafNodes<TestExample> l1 = sequence<TestExample>(l0, events);
    LeafNodes<TestExample> l2 = alternatives<TestExample>(l1, events);
    ResultStates<TestExample> results = root->evaluate_depth(sim_state);
    BOOST_CHECK(results[0]->get() == 5);
    BOOST_CHECK(results[1]->get() == 5);

}

BOOST_AUTO_TEST_CASE(copy_immutability_between_branches) {
    // Test that alternative branches receive a copy of the TestExample.memories vector.
    // The "remember" event mutates it. Immutability is proven by contents of the vectors
    // being identical in all branches (i.e. the events do not add to a same shared vector).
    EventChain<TestExample> events{increment, increment};
    EventChain<TestExample> checkpoint{remember};
    auto sim_state = std::make_shared<TestExample>(1);
    EventNode<TestExample> root = EventDAG<TestExample>::new_node(increment);
    LeafNodes<TestExample> l0 = root->collect_leaf_nodes();
    LeafNodes<TestExample> l1 = sequence<TestExample>(l0, events);
    LeafNodes<TestExample> l2 = sequence<TestExample>(l1, checkpoint);
    LeafNodes<TestExample> l3 = alternatives<TestExample>(l2, events);
    LeafNodes<TestExample> l4 = sequence<TestExample>(l3, events);
    LeafNodes<TestExample> l5 = sequence<TestExample>(l4, checkpoint);
    LeafNodes<TestExample> l6 = alternatives<TestExample>(l5, events);
    LeafNodes<TestExample> l7 = sequence<TestExample>(l6, events);
    LeafNodes<TestExample> l8 = sequence<TestExample>(l7, checkpoint);
    ResultStates<TestExample> results = root->evaluate_depth(sim_state);

    BOOST_CHECK(results.size() == 4);
    for(int i=0; i<4; i++) {
        BOOST_CHECK(results[i]->memories().size() == 3);
        BOOST_CHECK(results[i]->memories()[0] == 4);
        BOOST_CHECK(results[i]->memories()[1] == 7);
        BOOST_CHECK(results[i]->memories()[2] == 10);
    }
}