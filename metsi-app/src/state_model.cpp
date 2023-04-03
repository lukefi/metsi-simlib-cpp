#include "state_model.hpp"

/**
 * Default constructor for initializing the underlaying OverlaidObject<Properties> with an
 * empty property map.
 */
WithOverlay::WithOverlay() {
    overlay = OverlaidObject<Properties>::create(std::make_shared<Properties>());
}
/**
 * Constructor for initializing the underlying OverlaidObject<Properties> with a previously
 * populated property map.
 * @param properties
 */

WithOverlay::WithOverlay(const Properties& properties) {
    overlay = OverlaidObject<Properties>::create(std::make_shared<Properties>(properties));
}

/**
 * Initialize a new overlay level for this instances existing overlay.
 *
 * @return
 */
void WithOverlay::new_layer() const {
    this->overlay = std::make_shared<OverlaidObject<Properties>>(this->overlay);
}

/**
 * Wrapper for underlying OverlaidObject<Properties> operator[]
 * @param key
 * @return
 */
const std::string& WithOverlay::operator [](const std::string& key) const {
        return (*overlay)[key];
}

/**
 * Trigger creation of new layer for each contained reference tree and tree stratum entity along with self.
 */
void ForestStand::new_layer() const {
    std::for_each(trees.begin(), trees.end(),
                  [](const ReferenceTree& tree) { tree.new_layer(); });
    std::for_each(strata.begin(), strata.end(),
                  [](const TreeStratum& stratum) { stratum.new_layer(); });
    WithOverlay::new_layer();
}

const ReferenceTree& ForestStand::create_tree(const Properties& props) {
    trees.emplace_back(props);
    return trees.back();
}

const TreeStratum &ForestStand::create_stratum(const Properties& props) {
    strata.emplace_back(props);
    return strata.back();
}

SimulationState::SimulationState(const ForestStand& stand): stand(stand) {}

/**
 * Copy constructor which flushes the variable cache of previous instance and triggers explicit copy construction of
 * underlying state model.
 * @param previous
 */
SimulationState::SimulationState(const SimulationState& previous) {
    previous.flush_cache();
    this->stand = ForestStand(previous.stand);
    this->stand.new_layer();
}

std::pair<std::string, std::string> SimulationState::parse_variable_tokens(const std::string& mapping) {
    auto dpos = mapping.find('#');
    if(dpos == std::string::npos) {
        return {mapping, ""};
    }
    else {
        return {
            mapping.substr(0, dpos),
            mapping.substr(dpos + 1, mapping.size() - dpos + 1)
        };
    }
}

/**
 * Flush the current variable cache into the underlying state objects.
 */
void SimulationState::flush_cache() const {
    for(auto& cache_item : var_cache) {
        auto tokens = parse_variable_tokens(cache_item.first);
        if(tokens.second.empty()) {
            stand.set<float>(tokens.first, cache_item.second.front());
        }
        else if(tokens.first == "tree") {
            for (int i = 0; i < stand.trees.size(); i++) {
                stand.trees[i].set<float>(tokens.second, cache_item.second[i]);
            }
        }
        else if(tokens.first == "stratum") {
            for (int i = 0; i < stand.strata.size(); i++) {
                stand.strata[i].set<float>(tokens.second, cache_item.second[i]);
            }
        }
    }
    if(!var_cache.empty()) {
        var_cache.clear();
    }
}
/**
 * Obtain a reference vector of floaty properties for a given entity type. The property and the entity is established
 * via the given mapping, which is an encoded string of the format [property | entity#property]. Currently entity may be
 * either 'tree' or 'stratum'. Property may be a named property as recorded in the underlying state types. When no
 * entity given, return a single element vector from a ForestStand named property. The requested property is assumed
 * to be convertible to float by boost::lexical_cast. The underlying implementation will throw boost::bad_lexical_cast
 * upon attempting to obtain a non-floaty value.
 *
 * @param mapping an encoded named property
 * @return a mutable content vector reference
 */
std::vector<float>& SimulationState::get_vars(const std::string& mapping) {
    if(var_cache.contains(mapping)) {
        return var_cache[mapping];
    }
    else {
        auto tokens = parse_variable_tokens(mapping);
        auto& target = var_cache[mapping];
        if(tokens.second.empty()) {
            target.emplace_back(stand.get<float>(tokens.first));
        }
        else if(tokens.first == "tree") {
            for (const ReferenceTree& tree: stand.trees) {
                target.emplace_back(tree.get<float>(tokens.second));
            }
        }
        else if(tokens.first == "stratum") {
            for(const TreeStratum& stratum : stand.strata) {
                target.emplace_back(stratum.get<float>(tokens.second));
            }
        }
        return target;
    }
}
