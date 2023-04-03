#include "file_io.hpp"
#include "state_model.hpp"

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
        {"dbh", 5},
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

ForestStandCSVReader::iterator::iterator(csv::CSVReader* reader) {
    this->reader = reader;
    this->csv_iter = reader->begin();
}

/**
 * Utilize the underlying CSV file iterator to parse a ForestStand from current stand row down to the following stand
 * row or the end of file.
 *
 * @return ForestStand that has been parsed
 * @throws std::out_of_range upon attempting to parse at end of file
 * @throws std::domain_error upon attempting to parse stand from an unexpected non-stand row
 */
ForestStand ForestStandCSVReader::iterator::parse_stand() {
    if(csv_iter == reader->end()) {
        throw std::out_of_range("Attempted to parse ForestStand beyond end of content.");
    }

    if((*csv_iter)[0] == "stand") {
        auto current = ForestStand(vars_from_row(*csv_iter, stand_indices));
        csv_iter++;
        while( csv_iter != reader->end() && (*csv_iter)[0] != "stand") {
            if((*csv_iter)[0] == "tree") {
                current.create_tree(vars_from_row(*csv_iter, tree_indices));
            }
            csv_iter++;
        }
        return current;
    }
    else {
        throw std::domain_error("Unable to parse ForestStand from file. Malformed CSV?");
    }
}

ForestStandCSVReader::ForestStandCSVReader(const std::string& filepath) {
    csv::CSVFormat format;
    format.delimiter(';').no_header();
    reader = new csv::CSVReader(filepath, format);
}