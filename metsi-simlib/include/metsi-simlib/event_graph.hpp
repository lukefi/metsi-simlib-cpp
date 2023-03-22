#ifndef EVENT_GRAPH_HPP
#define EVENT_GRAPH_HPP

#include <functional>
#include <vector>
#include <set>
#include <memory>
#include "core_types.hpp"

/**
 * A node in the directed graph of simulation events. Holds a simulation event function (T=>T). Holds a
 * list of follow-up nodes. This structure describes a full, computable simulation.
 * @tparam T a type representing the simulation state
 */
template<typename T> class EventDAG : public std::enable_shared_from_this<EventDAG<T>> {
    private:
    EventFn<T> event;
    EventNodes<T> followers;
    bool is_leaf();

    public:
    /**
     * Constructor creating a bare event node graph with a T=>T simulation event function. Use the class static function
     * EventDAG::new_node(EventFn<T>) to obtain a shared EventNode represenation for this object.
     */
    explicit EventDAG(EventFn<T>);

    /**
     * Attach an existing graph node as a follower of this node
     */
    void add_node(EventNode<T>);

    /**
     * Depth-first search for the leaf nodes of this graph.
     * @return a set of leaf nodes for this directed graph
     */
    LeafNodes<T> collect_leaf_nodes();

    /**
     * Depth-first evaluation of the simulation represented by this graph.
     * @return a vector of simulation states T as given after leaf node event
     */
    ResultStates<T> evaluate_depth(StateReference<T>);

    /**
     * A static factory for new nodes of an EventDAG graph
     * @return a graph node constructred with given T=>T function
     */
    static EventNode<T> new_node(EventFn<T>);
};

template<typename T> EventDAG<T>::EventDAG(EventFn<T> event): event(event) {}

template<typename T> LeafNodes<T> EventDAG<T>::collect_leaf_nodes() {
    LeafNodes<T> results;
    
    if(this->is_leaf()) {
        results.insert(this->shared_from_this());
    }
    else {
        for(EventNode<T> node : this->followers) {
            if(node->is_leaf()) {
                results.insert(node);
            }
            else {
                LeafNodes<T> sub_results = node->collect_leaf_nodes();
                for(EventNode<T> sub_result : sub_results) {
                    results.insert(sub_result);
                }
            }
        }
    }
    return results;
}

template<typename T> bool EventDAG<T>::is_leaf() {
    return this->followers.empty();
}

template<typename T> void EventDAG<T>::add_node(EventNode<T> next) {
    this->followers.push_back(next);
}

template<typename T> ResultStates<T> EventDAG<T>::evaluate_depth(StateReference<T> sim_state) {
    ResultStates<T> results;
    auto current = this->event(sim_state);
    if(this->is_leaf()) {
        results.push_back(current);
    }
    else {
        if(this->followers.size() == 1) {
            ResultStates<T> sub_results = this->followers.front()->evaluate_depth(current);
            results.insert(results.end(), sub_results.begin(), sub_results.end());
        }
        else if(this->followers.size() > 1) {
            int aborted_branches = 0;
            for (EventNode<T> node: this->followers) {
                try {
                    ResultStates<T> sub_results = node->evaluate_depth(std::make_shared<T>(*current));
                    results.insert(results.end(), sub_results.begin(), sub_results.end());
                }
                catch(BranchException& e) {
                    aborted_branches++;
                }
            }
            if(this->followers.size() == aborted_branches) {
                throw BranchException("All branches aborted.");
            }
        }
    }
    return results;
}

template<typename T> EventNode<T> EventDAG<T>::new_node(EventFn<T> event) {
    return std::make_shared<EventDAG<T>>(event);
}

#endif
