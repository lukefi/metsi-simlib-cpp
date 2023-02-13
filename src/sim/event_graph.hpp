#include <functional>
#include <vector>
#include <set>
#include <memory>

template<typename T> class EventDAG;
template<typename T> using EventNode = std::shared_ptr<EventDAG<T>>;
template<typename T> using EventNodes = std::vector<EventNode<T>>;
template<typename T> using LeafNodes = std::set<EventNode<T>>;
template<typename T> using OperationResults = std::vector<T>;
template<typename T> using SimOperation = std::function<T(T)>;
template<typename T> using OperationChain = std::vector<SimOperation<T>>;

template<typename T> class EventDAG : public std::enable_shared_from_this<EventDAG<T>> {
    private:
    SimOperation<T> operation;
    EventNodes<T> followers;
    bool is_leaf();

    public:
    explicit EventDAG(SimOperation<T>);
    void add_node(EventNode<T>);
    LeafNodes<T> collect_leaf_nodes();
    OperationResults<T> evaluate_depth(T);
    static EventNode<T> new_node(std::function<T(T)>);
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

template<typename T> OperationResults<T> EventDAG<T>::evaluate_depth(T payload) {
    OperationResults<T> results;
    T current = this->operation(payload);
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
                OperationResults<T> sub_results = node->evaluate_depth(current);
                results.insert(results.end(), sub_results.begin(), sub_results.end());
            }
        }
    }
    return results;
}

template<typename T> EventNode<T> EventDAG<T>::new_node(SimOperation<T> op) {
    return std::make_shared<EventDAG<T>>(op);
}
