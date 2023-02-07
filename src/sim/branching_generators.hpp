#include <memory>
#include "event_graph.hpp"

template<typename T>
LeafNodes<T> sequence(LeafNodes<T> previous, std::vector<typename Operation<T>::fn> operations) {
    if(operations.empty()) {
        return previous;
    }
    else {
        EventNode<T> sequence_root = nullptr;
        EventNode<T> leaf = nullptr;
        for(typename Operation<T>::fn op : operations) {
            auto next = EventDAG<T>::new_node(op);
            if(!sequence_root) {
                sequence_root = next;
                leaf = next;
                for(EventNode<T> prev : previous) {
                   prev->add_node(sequence_root);
                }
            }
            else {
                leaf->add_node(next);
                leaf = next;
            } 
        }
        LeafNodes<T> retval;
        retval.insert(leaf);
        return retval;
    }
}

template<typename T>
LeafNodes<T> alternatives(LeafNodes<T> previous, std::vector<typename Operation<T>::fn> operations) {
    if(operations.empty()) {
        return previous;
    }
    else {
        LeafNodes<T> leafs;
        for(typename Operation<T>::fn op : operations) {
            EventNode<T> branch = EventDAG<T>::new_node(op);
            leafs.insert(branch);
            for(EventNode<T> prev : previous) {
                prev->add_node(branch);
            }
        }
        return leafs;
    }
}
