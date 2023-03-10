#include <iostream>

#include "app_io.hpp"
#include "file_io.hpp"

int main(int argc, char** argv) {
    auto args = positional_arguments(argc, argv);

    std::cout << "Input data file: " << args["input-path"] << '\n';
    std::cout << "Output data directory: " << args["output-path"] << '\n';
    std::cout << "Control file: " << args["control-file"] << '\n';

    YAML::Node control_data;
    try {
        control_data = read_yaml(args["control-file"]);
    }
    catch(std::exception&) {
        std::cout << "Unable to read control file. Aborting.\n";
        return 1;
    }

    std::vector<ForestStand> stands;
    try {
        stands = read_forest_csv(args["input-path"]);
    }
    catch(std::exception&) {
        std::cout << "Unable to read input file. Aborting.\n";
        return 1;
    }

    return 0;
}