#include <gtest/gtest.h>
#include "CHTLLexer/Lexer.h"
#include "CHTLParser/Parser.h"
#include "CHTLGenerator/Generator.h"
#include "CHTL/Configuration.h"
#include <fstream>

using namespace CHTL;

TEST(ImportTest, CHTLImport) {
    std::string source = R"(
        [Import] @Chtl from "../../tests/import_test.chtl";
        body {
            @Element MyElement;
        }
    )";
    Lexer lexer(source);
    Parser parser(lexer);
    Configuration config;
    Generator generator(config);
    std::string result = generator.generate(*parser.parse());
    ASSERT_EQ(result, "<body><div>Imported Element</div></body>");
}

TEST(ImportTest, SelectiveCHTLImport) {
    std::string source = R"(
        [Import] [Template] @Element MyElement from "../../tests/import_test.chtl";
        body {
            @Element MyElement;
        }
    )";
    Lexer lexer(source);
    Parser parser(lexer);
    Configuration config;
    Generator generator(config);
    std::string result = generator.generate(*parser.parse());
    ASSERT_EQ(result, "<body><div>Imported Element</div></body>");
}

TEST(ImportTest, HTMLImport) {
    std::string source = R"(
        [Import] @Html from "../../tests/import_test.html" as myFile;
        body {
            @Html myFile;
        }
    )";
    Lexer lexer(source);
    Parser parser(lexer);
    Configuration config;
    Generator generator(config);
    std::string result = generator.generate(*parser.parse());
    ASSERT_EQ(result, "<body><div>Imported HTML</div></body>");
}

TEST(ImportTest, CSSImport) {
    std::string source = R"(
        [Import] @Style from "../../tests/import_test.css" as myFile;
        body {
            @Style myFile;
        }
    )";
    Lexer lexer(source);
    Parser parser(lexer);
    Configuration config;
    Generator generator(config);
    std::string result = generator.generate(*parser.parse());
    ASSERT_EQ(result, "<style>.imported-class { color: blue; }</style><body></body>");
}

TEST(ImportTest, JSImport) {
    std::string source = R"(
        [Import] @JavaScript from "../../tests/import_test.js" as myFile;
        body {
            @JavaScript myFile;
        }
    )";
    Lexer lexer(source);
    Parser parser(lexer);
    Configuration config;
    Generator generator(config);
    std::string result = generator.generate(*parser.parse());
    ASSERT_EQ(result, "<body>console.log(\"Imported JS\");</body>");
}

TEST(ImportTest, FileNotFound) {
    std::string source = R"(
        [Import] @Chtl from "nonexistent.chtl";
    )";
    Lexer lexer(source);
    Parser parser(lexer);
    Configuration config;
    Generator generator(config);
    ASSERT_THROW(generator.generate(*parser.parse()), std::runtime_error);
}
