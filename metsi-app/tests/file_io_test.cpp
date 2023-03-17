#define BOOST_TEST_MODULE file_io
#include <boost/test/unit_test.hpp>
#include <file_io.hpp>

BOOST_AUTO_TEST_CASE(read_yaml_file) {
    YAML::Node test = read_yaml("resources/test.yaml");
    BOOST_CHECK(test.Type() == YAML::NodeType::Map);
}

BOOST_AUTO_TEST_CASE(read_forest_stands) {
    auto result = read_forest_csv("resources/forest_data.csv");
    BOOST_CHECK(result.size() == 1);
    BOOST_CHECK(result[0].get<std::string>("identifier") == "43174747");
    BOOST_CHECK(result[0].get<int>("year") == 2021);

    BOOST_CHECK(result[0].trees[0].get<std::string>("identifier") == "43174747-1-tree");
    BOOST_CHECK(result[0].trees[0].get<int>("species") == 2);
    BOOST_CHECK_CLOSE(result[0].trees[0].get<float>("stems_ha"), 1.78f, 0.1f);
}