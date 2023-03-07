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
 * Copy constructor of a WithOverlay object. Initializes overlay as an "overlay copy" of
 * the previous instance's overlay reference. Refer to OverlayObject<T> documentation for
 * further details.
 * @param previous
 */
WithOverlay::WithOverlay(const WithOverlay& previous) {
    this->overlay = std::make_shared<OverlaidObject<Properties>>(previous.overlay);
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
 * Copy constructor for ForestStand to also trigger copy constructing nested objects and
 * to perform other relevant logic.
 * @param previous
 */
ForestStand::ForestStand(const ForestStand& previous) : WithOverlay(previous) {
    this->trees = std::vector<ReferenceTree>(previous.trees);
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

SimulationState::SimulationState(const SimulationState& previous) {
    this->stand = ForestStand(previous.stand);
}