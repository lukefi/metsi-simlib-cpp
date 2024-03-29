#ifndef STATE_MODEL_HPP
#define STATE_MODEL_HPP
#include <map>
#include <memory>
#include <vector>
#include "overlaid_object.hpp"

using Properties = std::map<std::string, std::string>;

/**
 * OverlaidObject<Properties> wrapper superclass for domain entities.
 */
class WithOverlay {
protected:
    mutable std::shared_ptr<OverlaidObject<Properties>> overlay;
public:
    WithOverlay();
    explicit WithOverlay(const Properties&);
    virtual void new_layer() const;
    const std::string& operator [](const std::string&) const;
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
    overlay->set<U>(key, value);
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

/**
 * ReferenceTree is an entity representing a set of trees of a certain tree species
 * with known age, approximate height and approximate breast height diameter
 * ReferenceTree has a record of stem count per hectare for trees matching this
 * description.
 *
 * An instance is a sample from a statistical distribution generated
 * from a TreeStratum, or a representation for physically measured trees within a
 * ForestStand.
 *
 * May contain further metadata and statistical values regarding the reference tree.
 *
 * This class inherits the WithOverlay mechanism for dynamic object property storage.
 * Copied instances address partial or extended overlays of previous instances.
 */
class ReferenceTree : public WithOverlay {
public:
    explicit ReferenceTree(const Properties& props) : WithOverlay(props) {};
};

/**
 * TreeStratum is an entity representing a set of trees of a certain species by mean
 * diameter, mean height and stem count per hectare. May contain further metadata
 * and statistical values regarding the stratum.
 *
 * This class inherits the WithOverlay mechanism for dynamic object property storage.
 * Copied instances address partial or extended overlays of previous instances.
 */
class TreeStratum : public WithOverlay {
public:
    explicit TreeStratum(const Properties& props) : WithOverlay(props) {};
};


/**
 * A ForestStand, aka. site is a geographical area of interest for forestry. Container
 * for mostly static site metadata, and reference trees and tree strata describing the
 * standing trees of the site. Growth and forestry operation events may modify site
 * metadata records.
 *
 * This class inherits the WithOverlay mechanism for dynamic object property storage.
 * Copied instances address partial or extended overlays of previous instances.
 */
class ForestStand : public WithOverlay {
public:
    std::vector<ReferenceTree> trees;
    std::vector<TreeStratum> strata;
    ForestStand() = default;
    explicit ForestStand(const Properties& props) : WithOverlay(props) {};
    void new_layer() const override;
    const ReferenceTree& create_tree(const Properties& = {});
    const TreeStratum& create_stratum(const Properties& = {});
};

class SimulationState {
    ForestStand stand;
    mutable std::map<std::string, std::vector<float>> var_cache;
    static std::pair<std::string, std::string> parse_variable_tokens(const std::string&);
protected:
    void flush_cache() const;
public:
    explicit SimulationState(const ForestStand&);
    SimulationState(const SimulationState&);
    std::vector<float>& get_vars(const std::string&);
    const ForestStand& get_stand() { return stand; };
};

#endif
