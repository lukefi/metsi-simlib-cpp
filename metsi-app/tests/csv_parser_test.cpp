#define BOOST_TEST_MODULE csv_parser_test
#include <boost/test/unit_test.hpp>
#include <vector>
#include "csv_parser.hpp"

BOOST_AUTO_TEST_CASE(simple_csv_read) {
    csv::CSVFormat format;
    format.delimiter(',').no_header();
    csv::CSVReader reader("test_resources/test.csv", format);
    std::vector<std::string> results;
    for(csv::CSVRow& row : reader) {
        for(csv::CSVField& field : row) {
            results.emplace_back(field.get<std::string>());
        }
    }
    std::vector<std::string> expected{"one", "two", "three", "1", "2", "3", "", ""};

    BOOST_CHECK(results.size() == 8);
    BOOST_CHECK(results == expected);
}
