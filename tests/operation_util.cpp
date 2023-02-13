#define BOOST_TEST_MODULE operation_util
#include <boost/test/unit_test.hpp>
#include <operation_util.hpp>

std::shared_ptr<int> increment_param(std::shared_ptr<int> val, std::map<std::string, std::string> params) {
    int amount = std::stoi(params["amount"]);
    *val += amount;
    return val;
}

BOOST_AUTO_TEST_CASE(parametered_operation_preparation) {
    std::map<std::string, std::string> params{{"amount", "2"}};
    SimOperation<int> op = parameter_bound_operation<int>(increment_param, params);
    auto start = std::make_shared<int>(1);
    auto l1 = op(start);
    auto l2 = op(l1);
    BOOST_CHECK(*l2 == 5);
}
