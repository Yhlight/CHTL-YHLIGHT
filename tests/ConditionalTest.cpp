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

std::string compile(const std::string& source) {
    Lexer lexer(source);
    Parser parser(lexer.tokenize());
    auto program = parser.parse();
    Analyser analyser(*program);
    analyser.analyse();
    Generator generator(*program);
    return generator.generate();
}

TEST(ConditionalTest, SimpleTernaryTrue) {
    std::string source = R"(
        div {
            style {
                width: 1 > 0 ? 100px : 50px;
            }
        }
    )";
    std::string result = compile(source);
    std::string expected = R"(<div style="width: 100px;"></div>)";
    EXPECT_EQ(removeWhitespace(result), removeWhitespace(expected));
}

TEST(ConditionalTest, SimpleTernaryFalse) {
    std::string source = R"(
        div {
            style {
                background-color: 10 > 20 ? "red" : "blue";
            }
        }
    )";
    std::string result = compile(source);
    std::string expected = R"(<div style="background-color: "blue";"></div>)";
    EXPECT_EQ(removeWhitespace(result), removeWhitespace(expected));
}

TEST(ConditionalTest, LogicalAnd) {
    std::string source = R"(
        div {
            style {
                height: 10 > 5 && 3 < 4 ? 200px : 100px;
            }
        }
    )";
    std::string result = compile(source);
    std::string expected = R"(<div style="height: 200px;"></div>)";
    EXPECT_EQ(removeWhitespace(result), removeWhitespace(expected));
}

TEST(ConditionalTest, LogicalOr) {
    std::string source = R"(
        div {
            style {
                border: 1 > 5 || 3 < 4 ? "1px solid black" : "none";
            }
        }
    )";
    std::string result = compile(source);
    std::string expected = R"(<div style="border: "1px solid black";"></div>)";
    EXPECT_EQ(removeWhitespace(result), removeWhitespace(expected));
}

TEST(ConditionalTest, StringComparison) {
    std::string source = R"(
        div {
            style {
                color: "hello" == "hello" ? green : "red";
            }
        }
    )";
    std::string result = compile(source);
    std::string expected = R"(<div style="color: green;"></div>)";
    EXPECT_EQ(removeWhitespace(result), removeWhitespace(expected));
}

TEST(ConditionalTest, WithPropertyReference) {
    std::string source = R"(
        p {
            id: "ref";
            style {
                font-size: 16px;
            }
        }
        div {
            style {
                padding: #ref.font-size > 10px ? 20px : 5px;
            }
        }
    )";
    std::string result = compile(source);
    std::string expected = R"(<p id="ref" style="font-size: 16px;"></p><div style="padding: 20px;"></div>)";
    EXPECT_EQ(removeWhitespace(result), removeWhitespace(expected));
}
