#include "gtest/gtest.h"
#include "Util/FileUtil.h"
#include <fstream>

TEST(FileUtilTest, ReadFileContents) {
    // Create a dummy file to read
    std::string test_filename = "test_file.txt";
    std::string expected_content = "Hello, CHTL!";
    std::ofstream test_file(test_filename);
    test_file << expected_content;
    test_file.close();

    // Read the file contents
    auto actual_content = CHTL::Util::FileUtil::readFileContents(test_filename);

    // Check that the contents are correct
    ASSERT_TRUE(actual_content.has_value());
    EXPECT_EQ(actual_content.value(), expected_content);

    // Clean up the dummy file
    remove(test_filename.c_str());
}

TEST(FileUtilTest, ReadNonExistentFile) {
    // Try to read a file that doesn't exist
    auto actual_content = CHTL::Util::FileUtil::readFileContents("non_existent_file.txt");

    // Check that the result is empty
    ASSERT_FALSE(actual_content.has_value());
}
