#define BOOST_TEST_MODULE operation_util
#include <boost/test/unit_test.hpp>
#include <operation_util.hpp>

int increment_param(int val, std::map<std::string, std::string> params) {
    int amount = std::stoi(params["amount"]);
    return val + amount;
}

BOOST_AUTO_TEST_CASE(parametered_operation_preparation) {
    std::map<std::string, std::string> params{{"amount", "2"}};
    SimOperation<int> op = parameter_bound_operation<int>(increment_param, params);
    int start = 1;
    int l1 = op(start);
    int l2 = op(l1);
    BOOST_CHECK(l1 == 3);
    BOOST_CHECK(l2 == 5);
}
