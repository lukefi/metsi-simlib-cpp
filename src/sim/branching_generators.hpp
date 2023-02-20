#include <memory>
#include "event_graph.hpp"

template<typename OP> using GeneratorFn = std::function<LeafNodes<OP>(LeafNodes<OP>, std::vector<SimOperation<OP>>)>;

/**
 * Extend given simulator event graph leaf nodes with new event nodes. Given operation functions represent the events.
 * The functions are OP=>OP continuation functions. They are attached to the leaf nodes as a single linear sequence of
 * follower nodes, extending the depth of the graph by the length of the sequence.
 *
 * @tparam OP a type representing the simulation state
 * @param previous a collection of simulation event graph leaf nodes
 * @param operations a collection of OP=>OP functions representing simulation events
 * @return a collection of event graph leaf nodes as expanded by this generator function
 */
template<typename OP> LeafNodes<OP> sequence(LeafNodes<OP> previous, OperationChain<OP> operations) {
    if(operations.empty()) {
        return previous;
    }
    else {
        EventNode<OP> sequence_root = nullptr;
        EventNode<OP> leaf = nullptr;
        for(SimOperation<OP> op : operations) {
            auto next = EventDAG<OP>::new_node(op);
            if(!sequence_root) {
                sequence_root = next;
                leaf = next;
                for(EventNode<OP> prev : previous) {
                   prev->add_node(sequence_root);
                }
            }
            else {
                leaf->add_node(next);
                leaf = next;
            }
        }
        LeafNodes<OP> retval;
        retval.insert(leaf);
        return retval;
    }
}

/**
 * Extend given simulator event graph leaf nodes with new event nodes. Given operation functions represent the events.
 * The functions are OP=>OP continuation functions. They are attached to the leaf nodes as a new branches representing
 * alternative simulation paths, extending the depth of the graph by 1.
 *
 * @tparam OP a type representing the simulation state
 * @param previous a collection of simulation event graph leaf nodes
 * @param operations a collection of OP=>OP functions representing simulation events
 * @return a collection of event graph leaf nodes as expanded by this generator function
 */
template<typename OP> LeafNodes<OP> alternatives(LeafNodes<OP> previous, OperationChain<OP> operations) {
    if(operations.empty()) {
        return previous;
    }
    else {
        LeafNodes<OP> leafs;
        for(SimOperation<OP> op : operations) {
            EventNode<OP> branch = EventDAG<OP>::new_node(op);
            leafs.insert(branch);
            for(EventNode<OP> prev : previous) {
                prev->add_node(branch);
            }
        }
        return leafs;
    }
}

template<typename OP> std::optional<GeneratorFn<OP>> generator_by_name(const std::string& name) {
    if(name == "sequence") {
        return {sequence<OP>};
    }
    else if(name == "alternatives") {
        return {alternatives<OP>};
    }
    return {};
}
