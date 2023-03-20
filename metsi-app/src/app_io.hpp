#ifndef APP_IO_HPP
#define APP_IO_HPP
#include <boost/program_options.hpp>
#include <map>

std::map<std::string, std::string> positional_arguments(int argc, char** argv);

#endif