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

        std::string expected = R"(<html><head><style></style></head><body><div id="main">Hello, CHTL!</div><script></script></body></html>)";
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

        std::string expected = R"(<html><head><style>color: "red";</style></head><body><!-- This is a comment --><div id="main">Hello, CHTL!</div><script>console.log('Hello');</script></body></html>)";
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

        std::string expected = R"(<html><head><style>color: "black";
                line-height: 1.6;</style></head><body><div></div><script></script></body></html>)";
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

        std::string expected = R"(<html><head><style></style></head><body><span>This is a box.</span></body><script></script></html>)";
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

        std::string expected = "<html><head><style>background-color: \"rgb(255, 192, 203)\";</style></head><body><div></div><script></script></body></html>";
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

        std::string expected = "<html><head><style>color: red;font-size: 16px;</style></head><body><div></div><script></script></body></html>";
        ASSERT(result == expected);
    }});

    tests.push_back({"Test Origin Block", []() {
        std::string source = R"(
            [Origin] @Html
            {
                <h1>This is a raw HTML block</h1>
            }
        )";

        Lexer lexer(source);
        Parser parser(lexer.tokenize(), source);
        Generator generator(parser.parse());
        std::string result = generator.generate();

        std::string expected = "<html><head><style></style></head><body><h1>This is a raw HTML block</h1><script></script></body></html>";
        ASSERT(result == expected);
    }});

    tests.push_back({"Test Import Statement", []() {
        std::string source = R"(
            [Import] @Chtl from "my.chtl"
        )";

        Lexer lexer(source);
        Parser parser(lexer.tokenize(), source);
        Generator generator(parser.parse());
        std::string result = generator.generate();

        std::string expected = "<html><head><style></style></head><body><script></script></body></html>";
        ASSERT(result == expected);
    }});
}
