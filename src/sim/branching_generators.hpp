#include <memory>
#include "event_graph.hpp"

template<typename T>
std::set<std::shared_ptr<EventDAG<T>>> sequence(std::set<std::shared_ptr<EventDAG<T>>> previous, std::vector<typename Operation<T>::fn> operations) {
    if(operations.empty()) {
        return previous;
    }
    else {
        std::shared_ptr<EventDAG<T>> sequence_root = nullptr;
        std::shared_ptr<EventDAG<T>> leaf = nullptr;
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
        std::set<std::shared_ptr<EventDAG<T>>> retval;
        retval.insert(leaf);
        return retval;
    }
}

template<typename T>
std::set<std::shared_ptr<EventDAG<T>>> alternatives(std::set<std::shared_ptr<EventDAG<T>>> previous, std::vector<typename Operation<T>::fn> operations) {
    if(operations.empty()) {
        return previous;
    }
    else {
        std::set<std::shared_ptr<EventDAG<T>>> leafs;
        for(auto op : operations) {
            std::shared_ptr<EventDAG<T>> branch = std::make_shared<EventDAG<T>>(op);
            leafs.insert(branch);
            for(auto prev : previous) {
                prev->add_node(branch);
            }
        }
        return leafs;
    }
}