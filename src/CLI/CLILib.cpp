#include "CLILib.h"
#include <iostream>
#include <vector>
#include <string>

namespace CHTL {

void print_usage() {
    std::cout << "Usage: chtl [options] <input_file> [-o <output_file>]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  --inline            Generate a single HTML file with inline CSS and JS." << std::endl;
    std::cout << "  --inline-css        Generate an HTML file with inline CSS." << std::endl;
    std::cout << "  --inline-js         Generate an HTML file with inline JS." << std::endl;
    std::cout << "  --default-struct    Generate a default HTML structure (head, body)." << std::endl;
    std::cout << "  -o, --output        Specify the output file." << std::endl;
    std::cout << "  --help              Show this help message." << std::endl;
}

CommandLineOptions parse_arguments(int argc, char* argv[]) {
    CommandLineOptions opts;
    std::vector<std::string> args(argv + 1, argv + argc);

    for (size_t i = 0; i < args.size(); ++i) {
        if (args[i] == "--inline") {
            opts.inline_all = true;
        } else if (args[i] == "--inline-css") {
            opts.inline_css = true;
        } else if (args[i] == "--inline-js") {
            opts.inline_js = true;
        } else if (args[i] == "--default-struct") {
            opts.default_struct = true;
        } else if (args[i] == "--help") {
            print_usage();
            exit(0);
        } else if (args[i] == "-o" || args[i] == "--output") {
            if (i + 1 < args.size()) {
                opts.output_file = args[++i];
            } else {
                std::cerr << "Error: --output option requires an argument." << std::endl;
                print_usage();
                exit(1);
            }
        } else {
            if (opts.input_file.empty()) {
                opts.input_file = args[i];
            } else {
                std::cerr << "Error: Multiple input files specified." << std::endl;
                print_usage();
                exit(1);
            }
        }
    }

    if (opts.input_file.empty()) {
        std::cerr << "Error: No input file specified." << std::endl;
        print_usage();
        exit(1);
    }

    return opts;
}

}
