#ifndef STATE_MODEL_HPP
#define STATE_MODEL_HPP
#include <boost/any.hpp>
#include <overlaid_object.hpp>

using Properties = std::map<std::string, boost::any>;

/**
 * OverlaidObject<Properties> wrapper superclass for domain entities.
 */
class WithOverlay {
protected:
    std::shared_ptr<OverlaidObject<Properties>> overlay;
public:
    WithOverlay();
    explicit WithOverlay(const Properties&);
    WithOverlay(const WithOverlay&);
    const boost::any& operator [](const std::string&) const;
    template<typename U> void set(const std::string&, U) const;
    template<typename U> U get(const std::string&) const;
};


/**
 * Wrapper for underlying OverlaidObject<Prototype> set function.
 * @tparam U property type
 * @param key property name
 * @return property value
 */
template<typename U> void WithOverlay::set(const std::string& key, U value) const {
    overlay->set<U>({key, value});
}

/**
 * Wrapper for underlying OverlaidObject<Prototype> get function.
 * @tparam U property type
 * @param key property name
 * @return property value
 */
template<typename U> U WithOverlay::get(const std::string& key) const {
    return overlay->get<U>(key);
};

class ReferenceTree : public WithOverlay {
public:
    ReferenceTree() = default;
    ReferenceTree(const ReferenceTree&) = default;
};

class TreeStratum : public WithOverlay {
public:
    TreeStratum() = default;
    TreeStratum(const TreeStratum&) = default;
};

class ForestStand : public WithOverlay {
public:
    std::vector<ReferenceTree> trees;
    ForestStand() = default;
    ForestStand(const ForestStand&);
    const ReferenceTree& create_tree();
};

#endif