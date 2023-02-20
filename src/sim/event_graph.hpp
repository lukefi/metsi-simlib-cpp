#include <functional>
#include <vector>
#include <set>
#include <memory>

template<typename OP> class EventDAG;
template<typename OP> using EventNode = std::shared_ptr<EventDAG<OP>>;
template<typename OP> using EventNodes = std::vector<EventNode<OP>>;
template<typename OP> using LeafNodes = std::set<EventNode<OP>>;
template<typename OP> using OperationResults = std::vector<std::shared_ptr<OP>>;
template<typename OP> using SimOperation = std::function<std::shared_ptr<OP>(std::shared_ptr<OP>)>;
template<typename OP> using OperationChain = std::vector<SimOperation<OP>>;

/**
 * A node in the directed graph of simulation events. Holds a simulation event function (OP=>OP continuation). Holds a
 * list of follow-up nodes. This structure describes a full, computable simulation.
 * @tparam OP a type representing the simulation state
 */
template<typename OP> class EventDAG : public std::enable_shared_from_this<EventDAG<OP>> {
    private:
    SimOperation<OP> operation;
    EventNodes<OP> followers;
    bool is_leaf();

    public:
    /**
     * Constructor creating a bare event node graph with a OP=>OP simulation event function. Use the class static function
     * EventDAG::new_node(SimOperation<OP>) to obtain a shared EventNode represenation for this object.
     */
    explicit EventDAG(SimOperation<OP>);

    /**
     * Attach an existing graph node as a follower of this node
     */
    void add_node(EventNode<OP>);

    /**
     * Depth-first search for the leaf nodes of this graph.
     * @return a set of leaf nodes for this directed graph
     */
    LeafNodes<OP> collect_leaf_nodes();

    /**
     * Depth-first evaluation of the simulation represented by this graph.
     * @return a vector of simulation states OP as given after leaf node continuation
     */
    OperationResults<OP> evaluate_depth(std::shared_ptr<OP>);

    /**
     * A static factory for new nodes of an EventDAG graph
     * @return a graph node constructred with given OP=>OP continuation function
     */
    static EventNode<OP> new_node(SimOperation<OP>);
};

template<typename T> EventDAG<T>::EventDAG(SimOperation<T> op): operation(op) {}

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

template<typename T> OperationResults<T> EventDAG<T>::evaluate_depth(std::shared_ptr<T> payload) {
    OperationResults<T> results;
    auto current = this->operation(payload);
    if(this->is_leaf()) {
        results.push_back(current);
    }
    else {
        if(this->followers.size() == 1) {
            OperationResults<T> sub_results = this->followers.front()->evaluate_depth(current);
            results.insert(results.end(), sub_results.begin(), sub_results.end());
        }
        else if(this->followers.size() > 1) {
            for (EventNode<T> node: this->followers) {
                OperationResults<T> sub_results = node->evaluate_depth(std::make_shared<T>(*current));
                results.insert(results.end(), sub_results.begin(), sub_results.end());
            }
        }
    }
    return results;
}

template<typename T> EventNode<T> EventDAG<T>::new_node(SimOperation<T> op) {
    return std::make_shared<EventDAG<T>>(op);
}
