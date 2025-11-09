#include <gtest/gtest.h>
#include "CHTLLexer/Lexer.h"
#include "CHTLParser/Parser.h"
#include "CHTLGenerator/Generator.h"
#include "CHTL/Configuration.h"

TEST(ConditionalRenderingTest, SimpleIf) {
    std::string source = R"(
        if {
            condition: 1 < 2;
            div { text { "True" } }
        }
    )";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto program = parser.parse();

    CHTL::Configuration config;
    CHTL::Generator generator(config);
    std::string result = generator.generate(*program);

    ASSERT_EQ(result, "<div>True</div>");
}

TEST(ConditionalRenderingTest, IfElse) {
    std::string source = R"(
        if {
            condition: 1 > 2;
            div { text { "True" } }
        } else {
            div { text { "False" } }
        }
    )";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto program = parser.parse();

    CHTL::Configuration config;
    CHTL::Generator generator(config);
    std::string result = generator.generate(*program);

    ASSERT_EQ(result, "<div>False</div>");
}

TEST(ConditionalRenderingTest, IfElseIf) {
    std::string source = R"(
        if {
            condition: 1 > 2;
            div { text { "True" } }
        } else if {
            condition: 2 > 1;
            div { text { "Else If" } }
        }
    )";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto program = parser.parse();

    CHTL::Configuration config;
    CHTL::Generator generator(config);
    std::string result = generator.generate(*program);

    ASSERT_EQ(result, "<div>Else If</div>");
}

TEST(ConditionalRenderingTest, IfElseIfElse) {
    std::string source = R"(
        if {
            condition: 1 > 2;
            div { text { "True" } }
        } else if {
            condition: 2 > 3;
            div { text { "Else If" } }
        } else {
            div { text { "False" } }
        }
    )";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto program = parser.parse();

    CHTL::Configuration config;
    CHTL::Generator generator(config);
    std::string result = generator.generate(*program);

    ASSERT_EQ(result, "<div>False</div>");
}
