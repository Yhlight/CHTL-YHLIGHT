#include <gtest/gtest.h>
#include "CHTL/CHTLImporter/Importer.h"
#include <fstream>
#include <filesystem>

TEST(ImporterTest, ResolvesRelativePath) {
    std::filesystem::create_directory("test_dir");
    std::ofstream("test_dir/_test_import.chtl") << "div {}";

    CHTL::Importer importer;
    std::string content = importer.importFile("test.chtl", "test_dir/_test_import.chtl");
    EXPECT_EQ(content, "div {}");

    std::filesystem::remove_all("test_dir");
}
