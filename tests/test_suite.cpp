#define BOOST_TEST_MODULE metsi
#include <boost/test/unit_test.hpp>
#include "add.hpp"

BOOST_AUTO_TEST_CASE(simple_addition_works)
{
    int a = 1;
    int b = 2;
    int result = add(a, b);
    BOOST_CHECK(result == a + b);
};