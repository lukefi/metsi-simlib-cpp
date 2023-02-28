#define BOOST_TEST_MODULE file_io
#include <boost/test/unit_test.hpp>
#include <file_io.hpp>

BOOST_AUTO_TEST_CASE(read_yaml_file) {
    YAML::Node test = read_yaml("resources/test.yaml");
    BOOST_CHECK(test.Type() == YAML::NodeType::Map);
}