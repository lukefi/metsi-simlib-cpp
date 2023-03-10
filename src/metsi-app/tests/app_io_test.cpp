#define BOOST_TEST_MODULE app_io_test
#include <boost/test/unit_test.hpp>
#include <string>
#include <app_io.hpp>

BOOST_AUTO_TEST_CASE(cli_positional_mandatory) {
    using namespace std::string_literals;
    std::vector<std::string> arguments = {"exec", "input", "output"};
    std::vector<char*> argv;
    argv.reserve(arguments.size());
for(const auto& s : arguments) {
        argv.push_back((char*)s.data());
    }
    argv.push_back(nullptr);
    auto pos = positional_arguments((int)argv.size() - 1, argv.data());
    BOOST_CHECK(pos["input-path"] == "input");
    BOOST_CHECK(pos["output-path"] == "output");
    BOOST_CHECK(pos["control-file"] == "control.yaml");
}

BOOST_AUTO_TEST_CASE(cli_positional_mandatory_with_control_file) {
    using namespace std::string_literals;
    std::vector<std::string> arguments = {"exec", "input", "output", "control2.yaml"};
    std::vector<char*> argv;
    argv.reserve(arguments.size());
    for(const auto& s : arguments) {
        argv.push_back((char*)s.data());
    }
    argv.push_back(nullptr);
    auto pos = positional_arguments((int)argv.size() - 1, argv.data());
    BOOST_CHECK(pos["input-path"] == "input");
    BOOST_CHECK(pos["output-path"] == "output");
    BOOST_CHECK(pos["control-file"] == "control2.yaml");
}