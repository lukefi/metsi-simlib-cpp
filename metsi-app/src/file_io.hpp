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
        using iterator_category = std::input_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = ForestStand;
        using pointer = ForestStand*;
        using reference = ForestStand&;

        ForestStand current;
        csv::CSVReader::iterator csv_iter;
        csv::CSVReader* reader;
        csv::CSVRow next;
        csv::internals::ThreadSafeDeque<ForestStand> store;

        void parse_stand();
    public:
        struct sentinel{};
        explicit iterator(csv::CSVReader*);
        reference operator*();
        auto operator++();
        iterator operator++(int);
        friend bool operator==(iterator, sentinel);
        friend bool operator!=(iterator, sentinel);
    };

public:
    ForestStandCSVReader(const std::string& filepath) {
        reader = new csv::CSVReader(filepath);
    }
    [[nodiscard]] auto begin() const {
        return iterator(reader);
    }
    [[nodiscard]] auto end() const {
        return iterator::sentinel{};
    }
};

#endif