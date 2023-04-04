#define BOOST_TEST_MODULE domain_functions_test
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include <domain_functions.hpp>

BOOST_AUTO_TEST_CASE(area_works) {
    float d = 23.3f; // in m
    float result = circle_area(d); // in m^2
    BOOST_CHECK_CLOSE(213.2f, result, 1);
}

BOOST_AUTO_TEST_CASE(area_works_with_scaling) {
    float d = 23.3f; // now in dm
    float result = circle_area(d, -2); // in m^2
    BOOST_CHECK_CLOSE(0.02132f, result, 1);
}

BOOST_AUTO_TEST_CASE(median_works) {
    std::vector<float> v1{1.0f, 3.0f, 3.0f};
    std::vector<float> v2{1.0f, 3.0f, 4.0f, 5.0f};
    std::vector<float> v3;
    float result1 = median(v1);
    float result2 = median(v2);
    BOOST_CHECK_EQUAL(3.0f, result1);
    BOOST_CHECK_CLOSE(3.5f, result2, 0.1);
    BOOST_CHECK_THROW(median(v3), std::domain_error);
}

BOOST_AUTO_TEST_CASE(median_is_pure) {
    std::vector<float> v{1.0f, 4.0f, 3.0f, 5.0f}; // unsorted vector must remain unsorted
    float result = median(v); // pass by value constructs a copy
    BOOST_CHECK_EQUAL(v[1], 4.0f);
    BOOST_CHECK_EQUAL(v[2], 3.0f);
}

BOOST_AUTO_TEST_CASE(basal_area_works) {
    std::vector<float> ds{10.0f, 20.0f};
    std::vector<float> stems{50.0f, 30.0f};
    std::vector<float> expected_gs{0.196, 0.471f};
    auto gs = basal_areas(stems, ds);
    for(int i = 0; i < expected_gs.size(); i++) {
        BOOST_CHECK_CLOSE(expected_gs[i], gs[i], 1);
    }
    BOOST_CHECK_THROW(basal_areas({1.0f}, {1.0f, 1.0f}), std::domain_error); // nonsense siuation
}