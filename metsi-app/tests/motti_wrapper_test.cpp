#define BOOST_TEST_MODULE motti_wrapper_test
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include <motti_wrapper.hpp>
#include "test_utils.hpp"


BOOST_AUTO_TEST_CASE(motti_wrapper_init) {
	auto& motti = MottiWrapper::get_instance();
	BOOST_CHECK(&(motti.Init) != nullptr);
	BOOST_CHECK(&(motti.SiteInit) != nullptr);
	BOOST_CHECK(&(motti.Growth) != nullptr);
}

BOOST_AUTO_TEST_CASE(grow_motti_test) {
    EventParameters p{{"step", "5"}};

    auto root_state = fixture();
    auto res = grow_motti(root_state, p);

    //test_assertions(res,{6.3f, 4.3f},{11.2f, 16.0f},{15.0f, 15.0f});
}