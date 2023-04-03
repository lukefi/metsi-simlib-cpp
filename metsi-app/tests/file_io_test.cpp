#define BOOST_TEST_MODULE file_io
#include <boost/test/unit_test.hpp>
#include <file_io.hpp>

BOOST_AUTO_TEST_CASE(read_yaml_file) {
    YAML::Node test = read_yaml("test_resources/test.yaml");
    BOOST_CHECK(test.Type() == YAML::NodeType::Map);
}

BOOST_AUTO_TEST_CASE(read_forest_stands) {
    auto result = read_forest_csv("test_resources/forest_data.csv");
    BOOST_CHECK(result.size() == 1);
    BOOST_CHECK(result[0].get<std::string>("identifier") == "43174747");
    BOOST_CHECK(result[0].get<int>("year") == 2021);

    BOOST_CHECK(result[0].trees[0].get<std::string>("identifier") == "43174747-1-tree");
    BOOST_CHECK(result[0].trees[0].get<int>("species") == 2);
    BOOST_CHECK_CLOSE(result[0].trees[0].get<float>("stems_ha"), 1.78f, 0.1f);
}

BOOST_AUTO_TEST_CASE(forest_stand_iterator) {
    auto fs_iter = ForestStandCSVReader("test_resources/forest_data_multiple.csv");
    int counter = 0;
    std::vector<ForestStand> results;
    for(auto stand : fs_iter) {
        counter++;
        results.emplace_back(stand);
    }
    BOOST_CHECK_EQUAL(counter, 2);
    BOOST_CHECK_EQUAL(results[0].get<std::string>("identifier"), "43174747");
    BOOST_CHECK_EQUAL(results[0].trees.size(), 9);
    BOOST_CHECK_EQUAL(results[1].get<std::string>("identifier"), "43174748");
    BOOST_CHECK_EQUAL(results[1].trees.size(), 9);
}