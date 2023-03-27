#define BOOST_TEST_MODULE state_model
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include <state_model.hpp>
#include <metsi-simlib/core_types.hpp>

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

StateReference<SimulationState> mock_grow(StateReference<SimulationState> state) {
    auto& ds = state->get_vars("tree#dbh");
    for(int i=0;i<state->get_stand().trees.size();i++) {
        ds[i] += 0.1f;
    }
    return state;
}

BOOST_AUTO_TEST_CASE(simlike_branching_safety) {
    // establish a branching scenario like happens with EventDAG and observe inter-branch change safety
    ForestStand stand;
    stand.set<std::string>("identifier", "stand-1");
    stand.set<std::string>("dummy_var", "original");

    const ReferenceTree& tree_1 = stand.create_tree();
    tree_1.set<std::string>("identifier", "stand-1-tree-1");
    tree_1.set<float>("dbh", 6.2f);

    const ReferenceTree& tree_2 = stand.create_tree();
    tree_2.set<std::string>("identifier", "stand-1-tree-2");
    tree_2.set<float>("dbh", 4.2f);

    StateReference<SimulationState> root_state = std::make_shared<SimulationState>(stand);
    auto stand_var = root_state->get_stand().get<std::string>("dummy_var");
    auto seq = mock_grow(root_state);
    auto alternative1_1 = mock_grow(std::make_shared<SimulationState>(*seq));
    alternative1_1->get_stand().set<std::string>("dummy_var", "changed");
    auto alternative1_2 = mock_grow(alternative1_1);
    auto alternative2_1 = mock_grow(std::make_shared<SimulationState>(*seq));

    auto seq_dbh = seq->get_vars("tree#dbh");
    auto branch1_dbh = alternative1_2->get_vars("tree#dbh");
    auto branch2_dbh = alternative2_1->get_vars("tree#dbh");

    std::vector<float> expected_seq{6.3f, 4.3f};
    std::vector<float> expected_branch1{6.5f, 4.5f};
    std::vector<float> expected_branch2{6.4f, 4.4f};

    BOOST_CHECK(seq->get_stand().get<std::string>("dummy_var") == "original");
    BOOST_CHECK(alternative1_1->get_stand().get<std::string>("dummy_var") == "changed");
    BOOST_CHECK(alternative2_1->get_stand().get<std::string>("dummy_var") == "original");
    for(int i=0;i<expected_seq.size();i++) {
        BOOST_CHECK_CLOSE(seq_dbh[i], expected_seq[i], 0.01f);
        BOOST_CHECK_CLOSE(branch1_dbh[i], expected_branch1[i], 0.01f);
        BOOST_CHECK_CLOSE(branch2_dbh[i], expected_branch2[i], 0.01f);
    }
}
