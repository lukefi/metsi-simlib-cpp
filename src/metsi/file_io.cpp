#include "file_io.hpp"

YAML::Node read_yaml(const std::string& file_path) {
    return YAML::LoadFile(file_path);
}