#include "app_io.hpp"

using namespace boost::program_options;

/**
 * Find command line argument values for
 * input file path (mandatory)
 * output directory path (mandatory)
 * control file path (optional)
 *
 * @param argc C-style main function argc
 * @param argv C-style main function argv
 * @return map of results
 */
std::map<std::string, std::string> positional_arguments(int argc, char** argv) {
    options_description layout;
    layout.add_options()
            ("input-path", value<std::string>()->required(), "Simulation source data file path.")
            ("output-path", value<std::string>()->required(), "Application output directory path.")
            ("control-file", value<std::string>()->default_value("control.yaml"), "Application control file path.");
    positional_options_description pos;
    pos.add("input-path", 1);
    pos.add("output-path", 1);
    pos.add("control-file", 1);

    auto parsed = command_line_parser(argc, argv)
            .options(layout)
            .positional(pos)
            .run();

    variables_map result;
    store(parsed, result);
    notify(result);

    std::map<std::string, std::string> retval;
    for(auto item : result) {
        retval.insert({item.first, item.second.as<std::string>()});
    }

    return retval;
}
