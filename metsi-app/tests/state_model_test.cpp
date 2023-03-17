#define BOOST_TEST_MODULE state_model
#include <boost/test/unit_test.hpp>
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