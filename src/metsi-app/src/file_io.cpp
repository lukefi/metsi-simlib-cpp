#include "file_io.hpp"
#include "state_model.hpp"
#include <csv.hpp>

YAML::Node read_yaml(const std::string& file_path) {
    return YAML::LoadFile(file_path);
}

static std::map<std::string, int> stand_indices{
        {"identifier", 1},
        {"year", 3}
};

static std::map<std::string, int> tree_indices{
        {"identifier", 1},
        {"species", 2},
        {"stems_ha", 4},
        {"diameter_bh", 5},
        {"height", 6},
        {"age_bh", 7},
        {"age", 8}
};

std::optional<std::string> maybe_value(const std::string& val) {
    std::set<std::string> invalid{"", "null", "None", " "};
    if(invalid.contains(val)) {
        return std::nullopt;
    }
    else {
        return {val};
    }
}

Properties vars_from_row(csv::CSVRow& row, const std::map<std::string, int>& indices) {
    Properties retval;

    for(const auto& pair : indices) {
        auto maybe = maybe_value(row[pair.second].get<std::string>());
        if(maybe.has_value()) {
            retval.insert({pair.first, maybe.value()});
        }
    }
    return retval;
}

/**
 * Build a vector of ForestStand objects with nested ReferenceTree objects using a Forest Data Model CSV
 * file as source.
 *
 * @param file_path relative path to a CSV file
 * @return vector of ForestStand objects
 */
std::vector<ForestStand> read_forest_csv(const std::string& file_path) {
    csv::CSVFormat format;
    format.delimiter(';').no_header();
    csv::CSVReader reader(file_path, format);
    std::vector<ForestStand> stands;

    for(csv::CSVRow& row : reader) {
        if(row[0] == "stand") {
            auto vals = vars_from_row(row, stand_indices);
            stands.emplace_back(vals);
        }
        else if(row[0] == "tree") {
            auto vals = vars_from_row(row, tree_indices);
            stands.back().create_tree(vals);
        }
    }
    return stands;
}