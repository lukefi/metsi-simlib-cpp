#ifndef OVERLAID_OBJECT_HPP
#define OVERLAID_OBJECT_HPP
#include <boost/any.hpp>
#include <functional>
#include <optional>

/**
 * Concept for a type being array subscriptable i.e. usable with [] operator to resolve std::string parameter names
 * into boost::any values.
 */
template<typename T>
concept AnyStore = requires(T a, std::string k) {
    { a[k] } -> std::convertible_to<boost::any>;
};

/**
 * Concept for a type being copy constructible to conform with boost::any ValueType description.
 */
template<typename T>
concept ValueType = std::copy_constructible<T>;

/**
 * Overlay for an AnyStore prototype. Instances of OverlaidObject<T> form a stack of layers. These layers shadow
 * {std::string,boost::any} mappings from previous layers or expand upon them. Layers ultimately resolve into a
 * prototype AnyStore serving as the base truth for mappings.
 * @tparam T an AnyStore type
 */
template<AnyStore T> class OverlaidObject {
    std::optional<std::shared_ptr<T>> prototype{std::nullopt};
    mutable std::map<std::string, boost::any> values;
    std::optional<std::shared_ptr<OverlaidObject<T>>> previous{std::nullopt};
    bool immutable = false;
protected:
public:
    explicit OverlaidObject(std::shared_ptr<T>);
    explicit OverlaidObject(std::shared_ptr<OverlaidObject<T>>);
    template<ValueType U> U get(const std::string& name) const;
    template<ValueType U> void set(std::pair<std::string, U>);
    void set_immutable() { immutable = true; }
};

/**
 * Get U type value for given name key from this object, or recurse into previous OverlaidObject if not found.
 * Ultimately recurses into a prototype object of type T
 *
 * @tparam T an AnyStore template type
 * @tparam U scalar type contained by boost::any
 * @param name key name for a stored value
 * @return a scalar value obtainable from boost::any
 */
template<AnyStore T> template<ValueType U> U OverlaidObject<T>::get(const std::string& name) const {
    if(values.contains(name)) {
        return boost::any_cast<U>(values.at(name));
    }
    else if(previous) {
        OverlaidObject<T>* prev = previous.value().get();
        return prev->get<U>(name);
    }
    else if(prototype) {
        T* proto = prototype.value().get();
        try {
            U value = boost::any_cast<U>((*proto)[name]);
            return value;
        }
        catch(boost::bad_any_cast&) {
            throw std::out_of_range("No value exists for parameter " + name);
        }
    }
    else {
        return boost::any_cast<U>(values[name]);
    }
}

/**
 * Construct an OverlaidObject<T> with a T prototype
 * @tparam T an AnyStore template type
 * @param prototype an instance of an AnyStore T prototype
 */
template<AnyStore T> OverlaidObject<T>::OverlaidObject(std::shared_ptr<T> prototype) {
    this->prototype = {prototype};
    this->previous = std::nullopt;
}

/**
 * Construct an OverlaidObject<T> with an existing previous OverlaidObject<T>
 * @tparam T an AnyStore template type
 * @param previous an existing OverlaidObject<T> instance which the new instance will shadow
 */
template<AnyStore T> OverlaidObject<T>::OverlaidObject(std::shared_ptr<OverlaidObject<T>> previous) {
    this->prototype = std::nullopt;
    previous->set_immutable();
    this->previous = {previous};
}

/**
 * Attempt to set given key-value pair to this OverlaidObject's store. Succeeds if this instance had not been set as
 * immutable.
 *
 * @throws std::domain_error upon attempting to set values on such OverlaidObject instances which are already a previous layer for other objects.
 * @tparam T an AnyStore template type
 * @tparam U scalar ValueType template type contained by boost::any
 * @param kv key-value pair
 */
template<AnyStore T> template<ValueType U> void OverlaidObject<T>::set(std::pair<std::string, U> kv) {
    if(immutable) {
        throw(std::domain_error("Attempted to mutate a locked overlay layer with key " + kv.first));
    }
    values.insert(kv);
}

#endif