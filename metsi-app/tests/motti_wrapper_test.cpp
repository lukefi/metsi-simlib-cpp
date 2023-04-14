#define BOOST_TEST_MODULE motti_wrapper_test
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include <motti_wrapper.hpp>


BOOST_AUTO_TEST_CASE(motti_wrapper_init) {
#ifdef WITH_MOTTI4
	auto& motti = MottiWrapper::get_instance();
	BOOST_CHECK(&(motti.Init) != nullptr);
	BOOST_CHECK(&(motti.SiteInit) != nullptr);
	BOOST_CHECK(&(motti.Growth) != nullptr);
#else
	BOOST_CHECK_THROW(MottiWrapper::get_instance("libmotti4dll"), std::exception);
#endif
}