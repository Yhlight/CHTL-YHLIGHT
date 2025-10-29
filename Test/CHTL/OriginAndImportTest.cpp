
#include "gtest/gtest.h"
#include "CHTL/Lexer.h"
#include "CHTL/CHTLParser/Parser.h"
#include "CHTL/CHTLGenerator/Generator.h"
#include <fstream>

class OriginAndImportTest : public ::testing::Test {
protected:
    static void SetUpTestSuite() {
        // Create dummy files for testing imports
        std::ofstream htmlFile("test.html");
        htmlFile << "<h1>Hello from HTML</h1>";
        htmlFile.close();

        std::ofstream cssFile("test.css");
        cssFile << "body { color: red; }";
        cssFile.close();

        std::ofstream jsFile("test.js");
        jsFile << "console.log('Hello from JS');";
        jsFile.close();
    }

    static void TearDownTestSuite() {
        // Clean up dummy files
        remove("test.html");
        remove("test.css");
        remove("test.js");
    }
};

TEST_F(OriginAndImportTest, OriginBlock) {
    std::string source = R"(
        [Origin] @Html {
            <h1>This is a raw HTML block</h1>
        }
    )";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto program = parser.parse();
    CHTL::Generator generator(program, parser.getStyleTemplates(), parser.getElementTemplates(), parser.getVarTemplates());
    std::string result = generator.generate();
    ASSERT_EQ(result, "\n            <h1>This is a raw HTML block</h1>\n        ");
}

TEST_F(OriginAndImportTest, NamedOriginBlock) {
    std::string source = R"(
        [Origin] @Html MyHtmlBlock {
            <h1>This is a named raw HTML block</h1>
        }
    )";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto program = parser.parse();
    ASSERT_TRUE(parser.getOriginTemplates().count("MyHtmlBlock"));
}

TEST_F(OriginAndImportTest, ImportHtml) {
    std::string source = R"(
        [Import] @Html from "test.html" as MyHtml;
    )";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    parser.parse();
    ASSERT_TRUE(parser.getOriginTemplates().count("MyHtml"));
    ASSERT_EQ(parser.getOriginTemplates().at("MyHtml")->getContent(), "<h1>Hello from HTML</h1>");
}

TEST_F(OriginAndImportTest, ImportCss) {
    std::string source = R"(
        [Import] @Style from "test.css" as MyCss;
    )";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    parser.parse();
    ASSERT_TRUE(parser.getOriginTemplates().count("MyCss"));
    ASSERT_EQ(parser.getOriginTemplates().at("MyCss")->getContent(), "body { color: red; }");
}

TEST_F(OriginAndImportTest, ImportJs) {
    std::string source = R"(
        [Import] @JavaScript from "test.js" as MyJs;
    )";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    parser.parse();
    ASSERT_TRUE(parser.getOriginTemplates().count("MyJs"));
    ASSERT_EQ(parser.getOriginTemplates().at("MyJs")->getContent(), "console.log('Hello from JS');");
}
