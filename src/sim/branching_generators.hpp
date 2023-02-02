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
        for(auto op : operations) {
            auto next = std::make_shared<EventDAG<T>>(op);
            if(!sequence_root) {
                sequence_root = next;
                leaf = next;
                for(auto prev : previous) {
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
        for(auto op : operations) {
            EventNode<T> branch = std::make_shared<EventDAG<T>>(op);
            leafs.insert(branch);
            for(auto prev : previous) {
                prev->add_node(branch);
            }
        }
        return leafs;
    }
}