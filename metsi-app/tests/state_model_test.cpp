#define BOOST_TEST_MODULE state_model
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include <state_model.hpp>

BOOST_AUTO_TEST_CASE(data_model_simple_init) {
    ForestStand stand;
    stand.set<std::string>("identifier", "stand-1");

    const ReferenceTree& tree_1 = stand.create_tree();
    tree_1.set<std::string>("identifier", "stand-1-tree-1");
    tree_1.set<float>("dbh", 6.2f);

    const ReferenceTree& tree_2 = stand.create_tree();
    tree_2.set<std::string>("identifier", "stand-1-tree-2");
    tree_2.set<float>("dbh", 4.2f);

    BOOST_CHECK(stand.get<std::string>("identifier") == "stand-1");
    BOOST_CHECK_CLOSE(stand.trees[0].get<float>("dbh"), 6.2f, 0.1f);
    BOOST_CHECK_CLOSE(stand.trees[1].get<float>("dbh"), 4.2f, 0.1f);
}

BOOST_AUTO_TEST_CASE(data_model_copy_immutability) {
    ForestStand stand;
    stand.set<std::string>("identifier", "stand-1");

    const ReferenceTree& tree_1 = stand.create_tree();
    tree_1.set<std::string>("identifier", "stand-1-tree-1");
    tree_1.set<float>("dbh", 6.2f);

    const ReferenceTree& tree_2 = stand.create_tree();
    tree_2.set<std::string>("identifier", "stand-1-tree-2");
    tree_2.set<float>("dbh", 4.2f);

    ForestStand copy1(stand);
    copy1.set<std::string>("identifier", "stand-2");
    copy1.trees[0].set("dbh", 10.0f);

    ForestStand copy2(stand);
    copy2.set<std::string>("identifier", "stand-3");
    copy2.trees[0].set("dbh", 5.0f);

    // handling copy1 and copy 2 should keep stand unaffected
    BOOST_CHECK(stand.get<std::string>("identifier") == "stand-1");
    BOOST_CHECK_CLOSE(stand.trees[0].get<float>("dbh"), 6.2f, 0.1f);

    // copy1 behaves independently
    BOOST_CHECK(copy1.get<std::string>("identifier") == "stand-2");
    BOOST_CHECK(copy1.trees[0].get<std::string>("identifier") == "stand-1-tree-1");
    BOOST_CHECK_CLOSE(copy1.trees[0].get<float>("dbh"), 10.0f, 0.1f);

    // copy2 behaves independently
    BOOST_CHECK(copy2.get<std::string>("identifier") == "stand-3");
    BOOST_CHECK(copy2.trees[0].get<std::string>("identifier") == "stand-1-tree-1");
    BOOST_CHECK_CLOSE(copy2.trees[0].get<float>("dbh"), 5.0f, 0.1f);
}

BOOST_AUTO_TEST_CASE(data_model_with_var_cache) {
    ForestStand stand;
    stand.set<std::string>("identifier", "stand-1");

    const ReferenceTree& tree_1 = stand.create_tree();
    tree_1.set<std::string>("identifier", "stand-1-tree-1");
    tree_1.set<float>("dbh", 6.2f);

    const ReferenceTree& tree_2 = stand.create_tree();
    tree_2.set<std::string>("identifier", "stand-1-tree-2");
    tree_2.set<float>("dbh", 4.2f);

    SimulationState state(stand);
    std::vector<float> expected_initial{6.2f, 4.2f};
    auto& dbh_initial = state.get_vars("tree#dbh");
    BOOST_CHECK_EQUAL_COLLECTIONS(dbh_initial.begin(), dbh_initial.end(), expected_initial.begin(), expected_initial.end());

    dbh_initial[0] += 1.0f;
    dbh_initial[1] += 1.0f;

    std::vector<float> expected_modified{7.2f, 5.2f};
    auto& dbh_modified = state.get_vars("tree#dbh");
    BOOST_CHECK_EQUAL_COLLECTIONS(dbh_modified.begin(), dbh_modified.end(), expected_modified.begin(), expected_modified.end());

    SimulationState copy_state(state);
    ForestStand& copy_stand = copy_state.get_stand();

    // original object reference should contain original values
    BOOST_CHECK_CLOSE(stand.trees[0].get<float>("dbh"), 6.2f, 0.1f);
    // copied state should result in changes having been flushed into the overlay of underlying entity
    BOOST_CHECK_CLOSE(copy_stand.trees[0].get<float>("dbh"), 7.2f, 0.1f);
}