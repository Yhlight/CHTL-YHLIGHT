#include "gtest/gtest.h"
#include "CHTL/Lexer.h"
#include "CHTL/CHTLParser/Parser.h"
#include "CHTL/CHTLGenerator/Generator.h"
#include <string>
#include <fstream>

class ImportTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create dummy files for testing imports
        std::ofstream("test.html") << "<h1>Imported HTML</h1>";
        std::ofstream("test.css") << "body { color: red; }";
        std::ofstream("test.js") << "console.log('imported');";
    }

    std::string parseAndGenerate(const std::string& input) {
        CHTL::Lexer lexer(input);
        CHTL::Parser parser(lexer);
        auto program = parser.parse();
        CHTL::Generator generator(program, parser.getStyleTemplates(), parser.getElementTemplates(), parser.getVarTemplates(), parser.getOriginBlocks());
        return generator.generate();
    }
};

TEST_F(ImportTest, ImportHtml) {
    std::string chtlCode = R"(
        [Import] @Html from "test.html" as myHtml;
        [Origin] @Html myHtml;
    )";
    std::string expectedHtml = "<h1>Imported HTML</h1>";
    ASSERT_EQ(parseAndGenerate(chtlCode), expectedHtml);
}

TEST_F(ImportTest, ImportCss) {
    std::string chtlCode = R"(
        [Import] @Style from "test.css" as myCss;
        [Origin] @Style myCss;
    )";
    std::string expectedCss = "body { color: red; }";
    ASSERT_EQ(parseAndGenerate(chtlCode), expectedCss);
}

TEST_F(ImportTest, ImportJs) {
    std::string chtlCode = R"(
        [Import] @JavaScript from "test.js" as myJs;
        [Origin] @JavaScript myJs;
    )";
    std::string expectedJs = "console.log('imported');";
    ASSERT_EQ(parseAndGenerate(chtlCode), expectedJs);
}
