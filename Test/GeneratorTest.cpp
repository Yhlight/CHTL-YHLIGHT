#include "Test.h"
#include <stdexcept>
#include <iostream>
#include "CHTLLexer/Lexer.h"
#include "CHTLParser/Parser.h"
#include "CHTLGenerator/Generator.h"

#define ASSERT(condition) \
    if (!(condition)) { \
        throw std::runtime_error("Assertion failed at " + std::string(__FILE__) + ":" + std::to_string(__LINE__)); \
    }

void GeneratorTests() {
    tests.push_back({"Test Basic Generation", []() {
        std::string source = R"(
            div {
                id: "main";
                text { "Hello, CHTL!" }
            }
        )";

        Lexer lexer(source);
        Parser parser(lexer.tokenize(), source);
        Generator generator(parser.parse());
        std::string result = generator.generate();

        std::string expected = R"(<div id="main">Hello, CHTL!</div>)";
        ASSERT(result == expected);
    }});

    tests.push_back({"Test Full Generation", []() {
        std::string source = R"(
            # This is a comment
            div {
                id: "main";
                style {
                    color: "red";
                }
                script {
                    console.log('Hello');
                }
                text { "Hello, CHTL!" }
            }
        )";

        Lexer lexer(source);
        Parser parser(lexer.tokenize(), source);
        Generator generator(parser.parse());
        std::string result = generator.generate();

        std::string expected = R"(<!-- This is a comment --><div id="main"><style>color: "red";</style><script>console.log('Hello');</script>Hello, CHTL!</div>)";
        ASSERT(result == expected);
    }});

    tests.push_back({"Test Style Template", []() {
        std::string source = R"(
            [Template] @Style DefaultText {
                color: "black";
                line-height: 1.6;
            }

            div {
                style {
                    @Style DefaultText;
                }
            }
        )";

        Lexer lexer(source);
        Parser parser(lexer.tokenize(), source);
        Generator generator(parser.parse());
        std::string result = generator.generate();

        std::string expected = R"(<div><style>color: "black";
                line-height: 1.6;</style></div>)";
        ASSERT(result == expected);
    }});

    tests.push_back({"Test Element Template", []() {
        std::string source = R"(
            [Template] @Element Box {
                span {
                    text { "This is a box." }
                }
            }

            body {
                @Element Box;
            }
        )";

        Lexer lexer(source);
        Parser parser(lexer.tokenize(), source);
        Generator generator(parser.parse());
        std::string result = generator.generate();

        std::string expected = R"(<body><span>This is a box.</span></body>)";
        ASSERT(result == expected);
    }});

    tests.push_back({"Test Variable Template", []() {
        std::string source = R"V0G0N(
            [Template] @Var ThemeColor {
                tableColor: "rgb(255, 192, 203)";
            }

            div {
                style {
                    background-color: ThemeColor(tableColor);
                }
            }
        )V0G0N";

        Lexer lexer(source);
        Parser parser(lexer.tokenize(), source);
        Generator generator(parser.parse());
        std::string result = generator.generate();

        std::string expected = "<div><style>background-color: \"rgb(255, 192, 203)\";</style></div>";
        ASSERT(result == expected);
    }});

    tests.push_back({"Test Custom Style Group", []() {
        std::string source = R"(
            [Custom] @Style TextSet {
                color,
                font-size;
            }

            div {
                style {
                    @Style TextSet {
                        color: red;
                        font-size: 16px;
                    }
                }
            }
        )";

        Lexer lexer(source);
        Parser parser(lexer.tokenize(), source);
        Generator generator(parser.parse());
        std::string result = generator.generate();

        std::string expected = "<div><style>color: red;font-size: 16px;</style></div>";
        ASSERT(result == expected);
    }});
}
