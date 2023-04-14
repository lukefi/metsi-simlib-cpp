#define BOOST_TEST_MODULE sim_events_test
#include <boost/test/unit_test.hpp>
#include <sim_events.hpp>
#include <metsi-simlib/core_types.hpp>
#include "test_utils.hpp"

BOOST_AUTO_TEST_CASE(grow_acta_test) {
    EventParameters p{{"step", "5"}};

    auto root_state = fixture();
    auto res = grow_acta(root_state, p);

    test_assertions(res,{6.3f, 4.3f},{11.2f, 16.0f},{15.0f, 15.0f});
}
