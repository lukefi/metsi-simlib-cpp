#include <memory>
#include <optional>
#include "event_graph.hpp"
#include "core_types.hpp"
/**
 * Extend given simulator event graph leaf nodes with new event nodes. Given event functions represent the events.
 * The functions are T=>T functions. They are attached to the leaf nodes as a single linear sequence of
 * follower nodes, extending the depth of the graph by the length of the sequence.
 *
 * @tparam T a type representing the simulation state
 * @param previous a collection of simulation event graph leaf nodes
 * @param event labels a collection of T=>T functions representing simulation events
 * @return a collection of event graph leaf nodes as expanded by this generator function
 */
template<typename T> LeafNodes<T> sequence(LeafNodes<T> previous, EventChain<T> events) {
    if(events.empty()) {
        return previous;
    }
    else {
        EventNode<T> sequence_root = nullptr;
        EventNode<T> leaf = nullptr;
        for(EventFn<T> event : events) {
            auto next = EventDAG<T>::new_node(event);
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

/**
 * Extend given simulator event graph leaf nodes with new event nodes. Given event functions represent the events.
 * The functions are T=>T functions. They are attached to the leaf nodes as a new branches representing
 * alternative simulation paths, extending the depth of the graph by 1.
 *
 * @tparam T a type representing the simulation state
 * @param previous a collection of simulation event graph leaf nodes
 * @param events a collection of T=>T functions representing simulation events
 * @return a collection of event graph leaf nodes as expanded by this generator function
 */
template<typename T> LeafNodes<T> alternatives(LeafNodes<T> previous, EventChain<T> events) {
    if(events.empty()) {
        return previous;
    }
    else {
        LeafNodes<T> leafs;
        for(EventFn<T> event : events) {
            EventNode<T> branch = EventDAG<T>::new_node(event);
            leafs.insert(branch);
            for(EventNode<T> prev : previous) {
                prev->add_node(branch);
            }
        }
        return leafs;
    }
}

template<typename T> std::optional<GeneratorFn<T>> generator_by_name(const std::string& name) {
    if(name == "sequence") {
        return {sequence<T>};
    }
    else if(name == "alternatives") {
        return {alternatives<T>};
    }
    return {};
}
