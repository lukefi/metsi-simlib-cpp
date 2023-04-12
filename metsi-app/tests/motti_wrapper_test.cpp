#define BOOST_TEST_MODULE motti_wrapper_test
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include <motti_wrapper.hpp>

BOOST_AUTO_TEST_CASE(motti_wrapper_init) {
#ifdef WITH_MOTTI4
	MottiWrapper motti("libmotti4dll");
	BOOST_CHECK(&(motti.motti_lib) != nullptr);
	BOOST_CHECK(&(motti.Motti4SiteInit) != nullptr);
#else
	BOOST_CHECK_THROW(MottiWrapper("libmotti4dll"), std::exception);
#endif
}