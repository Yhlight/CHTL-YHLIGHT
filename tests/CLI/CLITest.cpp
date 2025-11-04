#include <gtest/gtest.h>
#include "CLI/CLILib.h"

TEST(CLITest, ParseArguments) {
    char* argv[] = {"./chtl", "--inline", "test.chtl"};
    CHTL::CommandLineOptions opts = CHTL::parse_arguments(3, argv);
    EXPECT_TRUE(opts.inline_all);
    EXPECT_FALSE(opts.inline_css);
    EXPECT_FALSE(opts.inline_js);
    EXPECT_FALSE(opts.default_struct);
    EXPECT_EQ(opts.input_file, "test.chtl");
}

TEST(CLITest, ParseMultipleArguments) {
    char* argv[] = {"./chtl", "--inline-css", "--inline-js", "--default-struct", "test.chtl"};
    CHTL::CommandLineOptions opts = CHTL::parse_arguments(5, argv);
    EXPECT_FALSE(opts.inline_all);
    EXPECT_TRUE(opts.inline_css);
    EXPECT_TRUE(opts.inline_js);
    EXPECT_TRUE(opts.default_struct);
    EXPECT_EQ(opts.input_file, "test.chtl");
}

TEST(CLITest, NoInputFile) {
    char* argv[] = {"./chtl", "--inline"};
    EXPECT_DEATH(CHTL::parse_arguments(2, argv), "Error: No input file specified.");
}

TEST(CLITest, MultipleInputFiles) {
    char* argv[] = {"./chtl", "file1.chtl", "file2.chtl"};
    EXPECT_DEATH(CHTL::parse_arguments(3, argv), "Error: Multiple input files specified.");
}

TEST(CLITest, OutputFile) {
    char* argv[] = {"./chtl", "test.chtl", "-o", "output.html"};
    CHTL::CommandLineOptions opts = CHTL::parse_arguments(4, argv);
    EXPECT_EQ(opts.input_file, "test.chtl");
    EXPECT_EQ(opts.output_file, "output.html");
}
