#ifndef OVERLAID_OBJECT_HPP
#define OVERLAID_OBJECT_HPP
#include <boost/lexical_cast.hpp>
#include <functional>
#include <optional>
#include <map>
#include <memory>
#include <string>

/**
 * Concept for a type being array subscriptable i.e. usable with [] operator to resolve std::string parameter names
 * into std::string values.
 */
template<typename T>
concept AnyStore = requires(T a, std::string k) {
    { a[k] } -> std::convertible_to<std::string>;
};

/**
 * Overlay for an AnyStore prototype. Instances of OverlaidObject<T> form a stack of layers. These layers shadow
 * {std::string,std::string} mappings from previous layers or expand upon them. Layers ultimately resolve into a
 * prototype AnyStore serving as the base truth for mappings.
 * @tparam T an AnyStore type
 */
template<AnyStore T> class OverlaidObject {
    std::optional<std::shared_ptr<T>> prototype{std::nullopt};
    mutable std::map<std::string, std::string> values;
    std::optional<std::shared_ptr<OverlaidObject<T>>> previous{std::nullopt};
    bool immutable = false;
protected:
public:
    explicit OverlaidObject(std::shared_ptr<T>);
    explicit OverlaidObject(std::shared_ptr<OverlaidObject<T>>);
    template<typename U> U get(const std::string&) const;
    const std::string& operator[](std::string) const;
    template<typename U> void set(std::string, U) const;
    void set_immutable() { immutable = true; }
    static std::shared_ptr<OverlaidObject<T>> create(std::shared_ptr<T>);
};

/**
 * Wrapper for OverlaidObject<T>::operator[] to find the value for the given key. Utilizes
 * boost::lexical_cast<U>() to obtain the contained type U property value. Throws boost::bad_lexical_cast error upon
 * value not being convertible to type U.
 *
 * @tparam T an AnyStore template type
 * @tparam U a type conversible from property value string
 * @param key key name for a stored property
 * @return a scalar value obtainable from std::string
 * @throws boost::bad_lexical_cast if value not castable to type U
 */
template<AnyStore T> template<typename U> U OverlaidObject<T>::get(const std::string& key) const {
    return boost::lexical_cast<U>((*this)[key]);
}

/**
 * Get the string value for a given property. Search the property storage of this instance. Recurse into previous
 * OverlaidObject if not found. Ultimately recurses into a prototype object of type T. Throws std::out_of_range error
 * if property not found.
 *
 * @tparam T an AnyStore template type
 * @param key key name for a stored property
 * @return the string value of a stored property
 * @throws std::out_of_range if property not found.
 */
template<AnyStore T> const std::string& OverlaidObject<T>::operator[](std::string key) const {
    const std::string* retval = nullptr;
    if(values.contains(key)) {
        retval = &values[key];
    }
    else if(previous) {
        OverlaidObject<T>* prev = previous.value().get();
        retval = &(*prev)[key];
    }
    else if(prototype) {
        T* proto = prototype.value().get();
        retval = &(*proto)[key];
    }

    if(!retval || retval->empty()) {
        throw std::out_of_range("No value exists for parameter " + key);
    }
    else {
        return *retval;
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
 * @tparam T an AnyStore template type
 * @tparam U a value type convertible to string
 * @param key property name
 * @param value property value
 * @throws std::domain_error upon attempting to set values on such OverlaidObject instances which are already a previous layer for other objects.
 * @throws boost::bad_lexical_cast upon U value not being conversible to string
 */
template<AnyStore T> template<typename U> void OverlaidObject<T>::set(std::string key, U value) const {
    if(immutable) {
        throw(std::domain_error("Attempted to mutate a locked overlay layer with key " + key));
    }
    values.insert(std::make_pair(key, boost::lexical_cast<std::string>(value)));
}

template<AnyStore T> std::shared_ptr<OverlaidObject<T>> OverlaidObject<T>::create(std::shared_ptr<T> prototype) {
    return std::make_shared<OverlaidObject<T>>(prototype);
}

#endif