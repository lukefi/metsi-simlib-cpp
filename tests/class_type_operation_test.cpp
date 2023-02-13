#define BOOST_TEST_MODULE class_type_operation_test
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

TestExample increment(TestExample payload) {
    payload.set(payload.get() + 1);
    return payload;
};

TestExample remember(TestExample payload) {
    payload.memorize(payload.get());
    return payload;
};

BOOST_AUTO_TEST_CASE(simple_sequence_with_object) {
    OperationChain<TestExample> ops{increment, increment};
    TestExample payload(1);
    EventNode<TestExample> root = EventDAG<TestExample>::new_node(increment);
    LeafNodes<TestExample> l0 = root->collect_leaf_nodes();
    LeafNodes<TestExample> l1 = sequence<TestExample>(l0, ops);
    OperationResults<TestExample> results = root->evaluate_depth(payload);
    BOOST_CHECK(results[0].get() == 4);
}

BOOST_AUTO_TEST_CASE(simple_alternatives_with_object) {
    OperationChain<TestExample> ops{increment, increment};
    TestExample payload(1);
    EventNode<TestExample> root = EventDAG<TestExample>::new_node(increment);
    LeafNodes<TestExample> l0 = root->collect_leaf_nodes();
    LeafNodes<TestExample> l1 = sequence<TestExample>(l0, ops);
    LeafNodes<TestExample> l2 = alternatives<TestExample>(l1, ops);
    OperationResults<TestExample> results = root->evaluate_depth(payload);
    BOOST_CHECK(results[0].get() == 5);
    BOOST_CHECK(results[1].get() == 5);

}

BOOST_AUTO_TEST_CASE(copy_immutability_between_branches) {
    OperationChain<TestExample> ops{increment, increment};
    OperationChain<TestExample> checkpoint{remember};
    TestExample payload(1);
    EventNode<TestExample> root = EventDAG<TestExample>::new_node(increment);
    LeafNodes<TestExample> l0 = root->collect_leaf_nodes();
    LeafNodes<TestExample> l1 = sequence<TestExample>(l0, ops);
    LeafNodes<TestExample> l2 = sequence<TestExample>(l1, checkpoint);
    LeafNodes<TestExample> l3 = alternatives<TestExample>(l2, ops);
    LeafNodes<TestExample> l4 = sequence<TestExample>(l3, ops);
    LeafNodes<TestExample> l5 = sequence<TestExample>(l4, checkpoint);
    LeafNodes<TestExample> l6 = alternatives<TestExample>(l5, ops);
    LeafNodes<TestExample> l7 = sequence<TestExample>(l6, ops);
    LeafNodes<TestExample> l8 = sequence<TestExample>(l7, checkpoint);
    OperationResults<TestExample> results = root->evaluate_depth(payload);

    BOOST_CHECK(results.size() == 4);
    for(int i=0; i<4; i++) {
        BOOST_CHECK(results[i].memories().size() == 3);
        BOOST_CHECK(results[i].memories()[0] == 4);
        BOOST_CHECK(results[i].memories()[1] == 7);
        BOOST_CHECK(results[i].memories()[2] == 10);
    }
}