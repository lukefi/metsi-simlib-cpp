#include "event_graph.hpp"

template<typename T>
std::set<EventDAG<T>*> sequence(std::set<EventDAG<T>*> previous, std::vector<typename Operation<T>::fn> operations) {
    if(operations.empty()) {
        return previous;
    }
    else {
        EventDAG<T>* sequence_root = nullptr;
        EventDAG<T>* leaf = nullptr;
        for(auto op : operations) {
            EventDAG<T>* next = new EventDAG<T>(op);
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
        std::set<EventDAG<T>*> retval;
        retval.insert(leaf);
        return retval;
    }
}

template<typename T>
std::set<EventDAG<T>*> alternatives(std::set<EventDAG<T>*> previous, std::vector<typename Operation<T>::fn> operations) {
    if(operations.empty()) {
        return previous;
    }
    else {
        std::set<EventDAG<T>*> leafs;
        for(auto op : operations) {
            EventDAG<T>* branch = new EventDAG<T>(op);
            leafs.insert(branch);
            for(auto prev : previous) {
                prev->add_node(branch);
            }
        }
        return leafs;
    }
}