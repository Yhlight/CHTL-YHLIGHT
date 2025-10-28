#include <gtest/gtest.h>
#include "CHTL/CHTLLexer/Lexer.h"
#include "CHTL/CHTLParser/Parser.h"
#include "CHTL/CHTLGenerator/Generator.h"
#include "CHTL/CHTLAnalyser/Analyser.h"
#include <string>
#include <algorithm>

using namespace CHTL;

inline std::string removeWhitespace(std::string str) {
    str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
    str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
    str.erase(std::remove(str.begin(), str.end(), '\r'), str.end());
    str.erase(std::remove(str.begin(), str.end(), '\t'), str.end());
    return str;
}

TEST(ReferenceTest, SimpleReference) {
    std::string source = R"(
        div {
            class: "box";
            style {
                width: 100px;
            }
        }
        p {
            style {
                width: .box.width;
            }
        }
    )";
    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    auto program = parser.parse();
    Analyser analyser(*program);
    analyser.analyse();
    Generator generator(*program);
    std::string result = generator.generate();
    std::string expected = R"(<div class="box" style="width: 100px;"></div><p style="width: 100px;"></p>)";
    EXPECT_EQ(removeWhitespace(result), removeWhitespace(expected));
}

TEST(ReferenceTest, ReferenceWithArithmetic) {
    std::string source = R"(
        div {
            class: "box";
            style {
                width: 100px;
            }
        }
        p {
            style {
                width: .box.width + 50px;
            }
        }
    )";
    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    auto program = parser.parse();
    Analyser analyser(*program);
    analyser.analyse();
    Generator generator(*program);
    std::string result = generator.generate();
    std::string expected = R"(<div class="box" style="width: 100px;"></div><p style="width: 150px;"></p>)";
    EXPECT_EQ(removeWhitespace(result), removeWhitespace(expected));
}

TEST(ReferenceTest, ReferenceById) {
    std::string source = R"(
        div {
            id: "main";
            style {
                height: 200px;
            }
        }
        p {
            style {
                height: #main.height - 50px;
            }
        }
    )";
    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    auto program = parser.parse();
    Analyser analyser(*program);
    analyser.analyse();
    Generator generator(*program);
    std::string result = generator.generate();
    std::string expected = R"(<div id="main" style="height: 200px;"></div><p style="height: 150px;"></p>)";
    EXPECT_EQ(removeWhitespace(result), removeWhitespace(expected));
}

TEST(ReferenceTest, ReferenceByTag) {
    std::string source = R"(
        div {
            style {
                padding: 10px;
            }
        }
        p {
            style {
                margin: div.padding;
            }
        }
    )";
    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    auto program = parser.parse();
    Analyser analyser(*program);
    analyser.analyse();
    Generator generator(*program);
    std::string result = generator.generate();
    std::string expected = R"(<div style="padding: 10px;"></div><p style="margin: 10px;"></p>)";
    EXPECT_EQ(removeWhitespace(result), removeWhitespace(expected));
}

TEST(ReferenceTest, ReferenceCalculatedValue) {
    std::string source = R"(
        div {
            class: "container";
            style {
                width: 100px * 2;
            }
        }
        p {
            style {
                width: .container.width / 4;
            }
        }
    )";
    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    auto program = parser.parse();
    Analyser analyser(*program);
    analyser.analyse();
    Generator generator(*program);
    std::string result = generator.generate();
    std::string expected = R"(<div class="container" style="width: 200px;"></div><p style="width: 50px;"></p>)";
    EXPECT_EQ(removeWhitespace(result), removeWhitespace(expected));
}
