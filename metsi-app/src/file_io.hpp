#ifndef FILE_IO_HPP
#define FILE_IO_HPP
#include <iterator>
#include <yaml-cpp/yaml.h>
#include "csv_parser.hpp"
#include "state_model.hpp"

YAML::Node read_yaml(const std::string&);
std::vector<ForestStand> read_forest_csv(const std::string&);

class ForestStandCSVReader {
    csv::CSVReader* reader;
public:
    class iterator {
        csv::CSVReader::iterator csv_iter;
        csv::CSVReader* reader;

        ForestStand parse_stand();
    public:
        struct sentinel{ static bool done(const iterator& it) { return it.csv_iter == it.reader->end(); }; };
        explicit iterator(csv::CSVReader*);
        ForestStand operator*() { return parse_stand(); };
        iterator operator++() { return *this; };
        friend bool operator==(const iterator& a, sentinel) { return ForestStandCSVReader::iterator::sentinel::done(a); };
        friend bool operator!=(const iterator& a, sentinel) { return !ForestStandCSVReader::iterator::sentinel::done(a); };
    };

public:
    explicit ForestStandCSVReader(const std::string&);
    [[nodiscard]] auto begin() const { return iterator(reader); }
    [[nodiscard]] static auto end() { return iterator::sentinel{}; }
};

#endif