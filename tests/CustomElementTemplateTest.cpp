#include "gtest/gtest.h"
#include "CHTLLexer/Lexer.h"
#include "CHTLParser/Parser.h"
#include "CHTLGenerator/Generator.h"
#include "CHTL/Configuration.h"
#include "CHTLNode/ProgramNode.h"
#include <memory>

TEST(CustomElementTemplateTest, SimpleSpecialization) {
    std::string source = R"(
        [Custom] @Element Box {
            div {
                style {
                    width: 100px;
                }
            }
        }

        body {
            @Element Box {
                div {
                    style {
                        height: 200px;
                    }
                }
            }
        }
    )";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto program = parser.parse();
    CHTL::Configuration config;
    CHTL::Generator generator(config);
    std::string result = generator.generate(*program);
    std::string expected = R"(<body><div style="width:100px;height:200px;"></div></body>)";
    ASSERT_EQ(result, expected);
}

TEST(CustomElementTemplateTest, IndexedSpecialization) {
    std::string source = R"(
        [Custom] @Element Box {
            div {}
            div {}
        }

        body {
            @Element Box {
                div[1] {
                    style {
                        height: 200px;
                    }
                }
            }
        }
    )";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto program = parser.parse();
    CHTL::Configuration config;
    CHTL::Generator generator(config);
    std::string result = generator.generate(*program);
    std::string expected = R"(<body><div></div><div style="height:200px;"></div></body>)";
    ASSERT_EQ(result, expected);
}

TEST(CustomElementTemplateTest, Insertion) {
    std::string source = R"(
        [Custom] @Element Box {
            div {}
        }

        body {
            @Element Box {
                insert at top {
                    span {}
                }
                insert at bottom {
                    p {}
                }
                insert after div {
                    h1 {}
                }
                insert before div {
                    h2 {}
                }
                insert replace div {
                    h3 {}
                }
            }
        }
    )";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto program = parser.parse();
    CHTL::Configuration config;
    CHTL::Generator generator(config);
    std::string result = generator.generate(*program);
    std::string expected = R"(<body><span></span><h2></h2><h3></h3><h1></h1><p></p></body>)";
    ASSERT_EQ(result, expected);
}

TEST(CustomElementTemplateTest, DeletionByTagName) {
    std::string source = R"(
        [Template] @Element Base {
            div {}
            span {}
        }

        [Custom] @Element Derived < Base {
            delete span;
        }

        body {
            @Element Derived {}
        }
    )";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto program = parser.parse();
    CHTL::Configuration config;
    CHTL::Generator generator(config);
    std::string result = generator.generate(*program);
    std::string expected = R"(<body><div></div></body>)";
    ASSERT_EQ(result, expected);
}

TEST(CustomElementTemplateTest, DeletionByIndexedTagName) {
    std::string source = R"(
        [Template] @Element Base {
            div {}
            div {}
            span {}
        }

        [Custom] @Element Derived < Base {
            delete div[0];
        }

        body {
            @Element Derived {}
        }
    )";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto program = parser.parse();
    CHTL::Configuration config;
    CHTL::Generator generator(config);
    std::string result = generator.generate(*program);
    std::string expected = R"(<body><div></div><span></span></body>)";
    ASSERT_EQ(result, expected);
}

TEST(CustomElementTemplateTest, DeletionByTemplate) {
    std::string source = R"(
        [Template] @Element Part {
            p {}
        }
        [Template] @Element Base {
            div {}
            @Element Part {}
        }

        [Custom] @Element Derived < Base {
            delete @Element Part;
        }

        body {
            @Element Derived {}
        }
    )";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto program = parser.parse();
    CHTL::Configuration config;
    CHTL::Generator generator(config);
    std::string result = generator.generate(*program);
    std::string expected = R"(<body><div></div></body>)";
    ASSERT_EQ(result, expected);
}
