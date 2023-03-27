#define BOOST_TEST_MODULE sim_events_test
#include <boost/test/unit_test.hpp>
#include <sim_events.hpp>
#include <metsi-simlib/core_types.hpp>

BOOST_AUTO_TEST_CASE(grow_acta_test) {
    EventParameters p{{"step", "5"}};
    ForestStand stand;
    stand.set<std::string>("identifier", "stand-1");

    const ReferenceTree& tree_1 = stand.create_tree();
    tree_1.set<std::string>("identifier", "stand-1-tree-1");
    tree_1.set<float>("dbh", 6.2f);
    tree_1.set<float>("height", 10.2f);
    tree_1.set<float>("age", 10.0f);
    tree_1.set<float>("species", 1.0f);

    const ReferenceTree& tree_2 = stand.create_tree();
    tree_2.set<std::string>("identifier", "stand-1-tree-2");
    tree_2.set<float>("dbh", 4.2f);
    tree_2.set<float>("height", 15.0f);
    tree_2.set<float>("age", 10.0f);
    tree_2.set<float>("species", 2.0f);

    StateReference<SimulationState> root_state = std::make_shared<SimulationState>(stand);
    auto res = grow_acta(root_state, p);


    auto& ds = res->get_vars("tree#dbh");
    auto& hs = res->get_vars("tree#height");
    auto& as = res->get_vars("tree#age");

    std::vector<float> expected_ds{6.3f, 4.3f};
    std::vector<float> expected_hs{11.2f, 16.0f};
    std::vector<float> expected_as{15.0f, 15.0f};

    for(int i=0;i<res->get_stand().trees.size();i++) {
        BOOST_CHECK_CLOSE(ds[i], expected_ds[i], 0.01f);
        BOOST_CHECK_CLOSE(hs[i], expected_hs[i], 0.01f);
        BOOST_CHECK_CLOSE(as[i], expected_as[i], 0.01f);
    }
}