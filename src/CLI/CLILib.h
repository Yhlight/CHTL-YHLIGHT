#pragma once

#include <string>
#include <vector>

namespace CHTL {

struct CommandLineOptions {
    bool inline_all = false;
    bool inline_css = false;
    bool inline_js = false;
    bool default_struct = false;
    std::string input_file;
    std::string output_file;
};

void print_usage();
CommandLineOptions parse_arguments(int argc, char* argv[]);

}
