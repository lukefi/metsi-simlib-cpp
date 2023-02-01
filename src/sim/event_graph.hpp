#include <functional>
#include <vector>
#include <set>

template<typename T>
struct Operation {
    typedef std::function<T(T)> fn;
};

template<typename T>
class EventDAG {
    private:
    typename Operation<T>::fn operation;
    std::vector<EventDAG<T>*> followers;

    public:
    EventDAG(typename Operation<T>::fn);
    void add_branch(typename Operation<T>::fn);
    void add_node(EventDAG<T>*);
    bool is_leaf();
    std::set<EventDAG<T>*> collect_leaf_nodes();
    std::vector<T> evaluate_depth(T);
    
    

};

template<typename T>
EventDAG<T>::EventDAG(typename Operation<T>::fn op): operation(op) {};

template<typename T> 
void EventDAG<T>::add_branch(typename Operation<T>::fn op) {
    EventDAG next(op);
    this->followers.push_back(&next);
}

template<typename T>
std::set<EventDAG<T>*> EventDAG<T>::collect_leaf_nodes() {
    std::set<EventDAG<T>*> results;
    
    if(this->is_leaf()) {
        results.insert(this);
    }
    else {
        for(auto& n : this->followers) {
            if(n->is_leaf()) {
                results.insert(n);
            }
            else {
                std::set<EventDAG<T>*> sub_results = n->collect_leaf_nodes();
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
void EventDAG<T>::add_node(EventDAG<T>* next) {
    this->followers.push_back(next);
}

template<typename T>
std::vector<T> EventDAG<T>::evaluate_depth(T payload) {
    std::vector<T> results;
    T current = this->operation(payload);
    if(this->is_leaf()) {
        results.push_back(current);
    }
    else {
        for(auto n : this->followers) {
            std::vector<T> sub_results = n->evaluate_depth(current);
            results.insert(results.end(), sub_results.begin(), sub_results.end());
        }        
    }
    return results;
}