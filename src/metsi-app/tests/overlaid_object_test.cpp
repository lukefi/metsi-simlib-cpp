#define BOOST_TEST_MODULE overlaid_object_test
#include <boost/test/unit_test.hpp>
#include <overlaid_object.hpp>

using Prototype = std::map<std::string, std::string>;

BOOST_AUTO_TEST_CASE(prototype_access) {
    using namespace std::literals::string_literals;
    auto a = std::make_shared<Prototype>();
    BOOST_CHECK((*a)["param1"].empty() == true);
    (*a)["param1"] = "1"s;
    BOOST_CHECK(boost::lexical_cast<std::string>((*a)["param1"]) == "1");
}

BOOST_AUTO_TEST_CASE(overlaid_object_construction) {
    using namespace std::literals::string_literals;
    auto a = std::make_shared<Prototype>();
    (*a)["param1"] = "1";
    auto ol1 =std::make_shared<OverlaidObject<Prototype>>(a);
    ol1->set<int>("param2", 2);
    auto ol2 = std::make_shared<OverlaidObject<Prototype>>(ol1);
    ol2->set<int>("param1", 3);
    ol2->set<int>("param2", 3);

    BOOST_CHECK(boost::lexical_cast<int>(a->at("param1")) == 1);
    BOOST_CHECK(ol1->get<int>("param1") == 1); // from proto
    BOOST_CHECK(ol1->get<int>("param2") == 2); // from proto
    BOOST_CHECK(ol2->get<int>("param1") == 3); // from proto
    BOOST_CHECK(ol2->get<int>("param2") == 3); // from proto

    // ol1 should be immutable after ol2 instantiation
    BOOST_CHECK_THROW(ol1->set<int>("param3", 3), std::domain_error);
}