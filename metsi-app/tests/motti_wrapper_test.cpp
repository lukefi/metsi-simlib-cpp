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

BOOST_AUTO_TEST_CASE(motti_site_init_fn) {
    auto& motti = MottiWrapper::get_instance();
    auto root_state = fixture();
    auto ms = new Motti4State(root_state);
    motti.SiteInit(&ms->yy, &ms->yy.Y, &ms->yy.X, &ms->yy.Z, ms->rv);
    int expected_rv[4]{0,0,0,0};
    BOOST_CHECK_EQUAL_COLLECTIONS(expected_rv, expected_rv+3, ms->rv, ms->rv+3);
}

BOOST_AUTO_TEST_CASE(motti_growth_fn) {
    auto& motti = MottiWrapper::get_instance();
    int step = 5;
    auto root_state = fixture();
    auto ms = new Motti4State(root_state);

    auto d = root_state->get_stand().trees[0].get<float>("height");
    motti.SiteInit(&ms->yy, &ms->yy.Y, &ms->yy.X, &ms->yy.Z, ms->rv);
    motti.Growth(&ms->yy, &ms->yp, &ms->ut, &ms->kor, &ms->vcr, &ms->apv, &ms->numtrees, &ms->fer, &ms->numfer, &ms->control, &step, ms->rv);
    int expected_rv[4]{0,0,0,0};
    BOOST_CHECK_EQUAL_COLLECTIONS(expected_rv, expected_rv+3, ms->rv, ms->rv+3);
    BOOST_CHECK_GT(ms->yp[0].h, d);
}

BOOST_AUTO_TEST_CASE(grow_motti_test) {
    EventParameters p{{"step", "5"}};

    auto root_state = fixture();
    auto res = grow_motti(root_state, p);

    //test_assertions(res,{6.3f, 4.3f},{11.2f, 16.0f},{15.0f, 15.0f});
}