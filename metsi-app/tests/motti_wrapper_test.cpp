#define BOOST_TEST_MODULE motti_wrapper_test
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include <motti_wrapper.hpp>

#if defined(_WIN32) || defined(_WIN64)
#define MOTTI4_FILENAME "libmotti4dll.dll"
#elif defined(__APPLE__)
#define MOTTI4_FILENAME "libmotti4dll.dylib"
#else
#define MOTTI4_FILENAME "libmotti4dll.so"
#endif

BOOST_AUTO_TEST_CASE(motti_wrapper_init) {
#ifdef WITH_MOTTI4
	MottiWrapper motti(MOTTI4_FILENAME);
	BOOST_CHECK(&(motti.motti_lib) != nullptr);
	BOOST_CHECK(&(motti.Init) != nullptr);
	BOOST_CHECK(&(motti.SiteInit) != nullptr);
	BOOST_CHECK(&(motti.Growth) != nullptr);
#else
	BOOST_CHECK_THROW(MottiWrapper("libmotti4dll"), std::exception);
#endif
}