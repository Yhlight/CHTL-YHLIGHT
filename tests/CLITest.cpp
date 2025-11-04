#include <gtest/gtest.h>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <fstream>

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

TEST(CLITest, BasicFileProcessing) {
    std::string test_file = "test.chtl";
    std::ofstream outfile(test_file);
    outfile << "div { text { \"hello\" } }";
    outfile.close();

    std::string command = "../src/chtl " + test_file;
    std::string output = exec(command.c_str());

    std::string expected_output = "<div>\n   \"hello\" \n</div>\n\n";

    EXPECT_EQ(output, expected_output);

    remove(test_file.c_str());
}

TEST(CLITest, DefaultStructFlag) {
    std::string test_file = "test_struct.chtl";
    std::ofstream outfile(test_file);
    outfile << "div { }";
    outfile.close();

    std::string command = "../src/chtl " + test_file + " --default-struct";
    std::string output = exec(command.c_str());

    std::string expected_output = "<!DOCTYPE html>\n<html>\n<head>\n</head>\n<body>\n<div>\n</div>\n</body>\n</html>\n\n";

    EXPECT_EQ(output, expected_output);

    remove(test_file.c_str());
}

TEST(CLITest, InlineFlag) {
    std::string test_file = "test_inline.chtl";
    std::ofstream outfile(test_file);
    outfile << "div { style { color: red; } }";
    outfile.close();

    std::string command = "../src/chtl " + test_file + " --inline";
    std::string output = exec(command.c_str());

    std::string expected_output = "<style>\ncolor: red;\n</style>\n<div>\n</div>\n\n";

    EXPECT_EQ(output, expected_output);

    remove(test_file.c_str());
}
