#ifndef TEST_UTILS_HPP
#define TEST_UTILS_HPP

#include "sim_events.hpp"

StateReference<SimulationState> fixture() {
    ForestStand stand;
    stand.set<std::string>("identifier", "stand-1");
    stand.set<std::string>("year", "2017");
    stand.set<std::string>("lat", "6885711.5702");
    stand.set<std::string>("lon", "615778.0098");
    stand.set<std::string>("masl", "30.4");
    stand.set<std::string>("dd", "970.0");
    stand.set<std::string>("land_use_category", "1.0");
    stand.set<std::string>("site_type_category", "1.0");
    stand.set<std::string>("soil_peatland_category", "1.0");
    stand.set<std::string>("tax_class", "1.0");
    stand.set<std::string>("tax_class_reduction", "1.0");

    const ReferenceTree& tree_1 = stand.create_tree();
    tree_1.set<std::string>("identifier", "stand-1-tree-1");
    tree_1.set<float>("species", 2.0f);
    tree_1.set<float>("origin", 1.0f);
    tree_1.set<float>("stems_ha", 50.0f);
    tree_1.set<float>("dbh", 6.2f);
    tree_1.set<float>("height", 10.2f);
    tree_1.set<float>("age_bh", 7.0f);
    tree_1.set<float>("age", 10.0f);

    const ReferenceTree& tree_2 = stand.create_tree();
    tree_2.set<std::string>("identifier", "stand-1-tree-2");
    tree_2.set<float>("species", 2.0f);
    tree_2.set<float>("origin", 1.0f);
    tree_2.set<float>("stems_ha", 30.0f);
    tree_2.set<float>("dbh", 4.2f);
    tree_2.set<float>("height", 20.0f);
    tree_2.set<float>("age_bh", 13.0f);
    tree_2.set<float>("age", 19.0f);

    return std::make_shared<SimulationState>(stand);
}

void test_assertions(
        StateReference<SimulationState> res,
        std::vector<float> expected_ds,
        std::vector<float> expected_hs,
        std::vector<float> expected_as
) {
    auto& ds = res->get_vars("tree#dbh");
    auto& hs = res->get_vars("tree#height");
    auto& as = res->get_vars("tree#age");

    for(int i=0;i<res->get_stand().trees.size();i++) {
        BOOST_CHECK_CLOSE(ds[i], expected_ds[i], 0.01f);
        BOOST_CHECK_CLOSE(hs[i], expected_hs[i], 0.01f);
        BOOST_CHECK_CLOSE(as[i], expected_as[i], 0.01f);
    }
}

#endif