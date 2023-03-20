#define BOOST_TEST_MODULE yaml_tester
#include <boost/test/unit_test.hpp>
#include <yaml-cpp/yaml.h>
#include <string>
#include <vector>
#include <tuple>

BOOST_AUTO_TEST_CASE(yaml_file_load) {
    YAML::Node test = YAML::LoadFile("test_resources/test.yaml");
    BOOST_CHECK(test.Type() == YAML::NodeType::Map);

    YAML::Node obj = test["object"];
    BOOST_CHECK(obj.Type() == YAML::NodeType::Map);
}

BOOST_AUTO_TEST_CASE(yaml_elementary_types) {
    YAML::Node test = YAML::LoadFile("test_resources/test.yaml");
    YAML::Node obj = test["object"];

    auto integer = obj["integer"].as<int>();
    BOOST_CHECK(integer == 1);

    auto string = obj["string"].as<std::string>();
    BOOST_CHECK(string == "abc");

    auto fl = obj["float"].as<float>();
    BOOST_CHECK(fl == 1.0);

}

BOOST_AUTO_TEST_CASE(yaml_list) {
    YAML::Node test = YAML::LoadFile("test_resources/test.yaml");
    YAML::Node obj = test["object"];

    // [1, 2, 3]
    auto list = obj["list"].as<std::vector<int>>();
    BOOST_CHECK(list[0] == 1);
    BOOST_CHECK(list[1] == 2);
    BOOST_CHECK(list[2] == 3);
}

BOOST_AUTO_TEST_CASE(yaml_mixed_type_list) {
    YAML::Node test = YAML::LoadFile("test_resources/test.yaml");
    YAML::Node obj = test["object"];

    // [1, 2, three]
    std::vector<int> tupleints;
    std::string tuplestr;
    for(YAML::const_iterator it=obj["tuple"].begin();it!=obj["tuple"].end();++it) {
        auto res = it->as<std::string>();
        try {
            int v = std::stoi(res);
            tupleints.emplace_back(v);
            continue;
        }
        catch(...) {
            tuplestr = res;
        }

    }
    BOOST_CHECK(tupleints[0] == 1);
    BOOST_CHECK(tupleints[1] == 2);
    BOOST_CHECK(tuplestr == "three");
}

BOOST_AUTO_TEST_CASE(yaml_mixed_type_list_nested_object) {
    YAML::Node test = YAML::LoadFile("test_resources/test.yaml");
    YAML::Node obj = test["object"];

    // {nested: [one, two, {three: [four, five]}]}
    YAML::Node l1 = obj["nested"];
    std::vector<std::string> strings;
    std::vector<std::string> content_strings;
    std::string nested_name;
    for(auto it : l1) {
        switch(it.Type()) {
            case YAML::NodeType::Scalar:
                strings.emplace_back(it.as<std::string>());
                break;
            case YAML::NodeType::Map:
                for(auto map_it : it) {
                    nested_name = map_it.first.as<std::string>();
                    for (auto content: map_it.second) {
                        content_strings.emplace_back(content.as<std::string>());
                    }
                }
                break;
            default:
                break;
        }
    }
    BOOST_CHECK(strings[0] == "one");
    BOOST_CHECK(strings[1] == "two");
    BOOST_CHECK(nested_name == "three");
    BOOST_CHECK(content_strings[0] == "four");
    BOOST_CHECK(content_strings[1] == "five");
}

BOOST_AUTO_TEST_CASE(direct_map_transformation) {
    YAML::Node test = YAML::LoadFile("test_resources/test.yaml");
    auto result = test["just_map"].as<std::map<std::string, std::string>>();
    BOOST_CHECK(result.size() == 2);
    BOOST_CHECK(result["abc"] == "1");
    BOOST_CHECK(result["def"] == "2");
}