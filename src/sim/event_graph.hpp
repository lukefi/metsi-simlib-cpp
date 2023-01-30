#include <functional>
#include <vector>


template<typename T>
struct Operation {
    typedef std::function<T(T)> fn;
};

template<typename T>
class EventDAG {
    private:
    typename Operation<T>::fn operation;
    std::vector<EventDAG<T>> followers;

    public:
    EventDAG(typename Operation<T>::fn);
    void add_branch(typename Operation<T>::fn);
    std::vector<EventDAG<T>> find_leaf_nodes();
    std::vector<T> evaluate_depth(T);
    
    

};

template<typename T>
EventDAG<T>::EventDAG(typename Operation<T>::fn op): operation(op) {};

template<typename T> 
void EventDAG<T>::add_branch(typename Operation<T>::fn op) 
{
    EventDAG next(op);
    this->followers.push_back(next);
}

template<typename T>
std::vector<EventDAG<T>> EventDAG<T>::find_leaf_nodes() {
    return this->followers;
}

template<typename T>
std::vector<T> EventDAG<T>::evaluate_depth(T payload) {
    std::vector<T> results;
    T current = this->operation(payload);
    if(this->followers.size() == 0) {
        results.push_back(current);
    }
    else {
        for(auto& n : this->followers) {
            std::vector<T> sub_results = n.evaluate_depth(current);
            results.insert(results.end(), sub_results.begin(), sub_results.end());
        }        
    }
    return results;
}