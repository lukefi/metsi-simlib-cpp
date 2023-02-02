#include <functional>
#include <vector>
#include <set>
#include <memory>

template<typename T>
struct Operation {
    typedef std::function<T(T)> fn;
};


template<typename T>
class EventDAG : public std::enable_shared_from_this<EventDAG<T>> {
    private:
    typename Operation<T>::fn operation;
    std::vector<std::shared_ptr<EventDAG<T>>> followers;
    bool is_leaf();

    public:
    EventDAG(typename Operation<T>::fn);
    void add_node(std::shared_ptr<EventDAG<T>>);
    std::set<std::shared_ptr<EventDAG<T>>> collect_leaf_nodes();
    std::vector<T> evaluate_depth(T);
    static std::shared_ptr<EventDAG<T>> new_node(typename Operation<T>::fn);
};

template<typename T>
using EventNode = std::shared_ptr<EventDAG<T>>;
template<typename T>
using LeafNodes = std::set<EventNode<T>>;
template<typename T>
using OperationResults = std::vector<T>;

template<typename T>
EventDAG<T>::EventDAG(typename Operation<T>::fn op): operation(op) {};

template<typename T>
LeafNodes<T> EventDAG<T>::collect_leaf_nodes() {
    LeafNodes<T> results;
    
    if(this->is_leaf()) {
        results.insert(this->shared_from_this());
    }
    else {
        for(auto& n : this->followers) {
            if(n->is_leaf()) {
                results.insert(n);
            }
            else {
                LeafNodes<T> sub_results = n->collect_leaf_nodes();
                for(auto& sub_result : sub_results) {
                    results.insert(sub_result);
                }
            }
        }
    }
    
    return results;
}

template<typename T>
bool EventDAG<T>::is_leaf() {
    return this->followers.empty();
}

template<typename T>
void EventDAG<T>::add_node(EventNode<T> next) {
    this->followers.push_back(next);
}

template<typename T>
OperationResults<T> EventDAG<T>::evaluate_depth(T payload) {
    OperationResults<T> results;
    T current = this->operation(payload);
    if(this->is_leaf()) {
        results.push_back(current);
    }
    else {
        for(auto n : this->followers) {
            OperationResults<T> sub_results = n->evaluate_depth(current);
            results.insert(results.end(), sub_results.begin(), sub_results.end());
        }        
    }
    return results;
}

template<typename T>
EventNode<T> EventDAG<T>::new_node(typename Operation<T>::fn op) {
    return std::make_shared<EventDAG<T>>(op);
}