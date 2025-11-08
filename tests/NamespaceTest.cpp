#include <gtest/gtest.h>
#include "CHTLLexer/Lexer.h"
#include "CHTLParser/Parser.h"
#include "CHTLGenerator/Generator.h"

using namespace CHTL;

TEST(NamespaceTest, SimpleNamespace) {
    std::string source = R"(
        [Namespace] mySpace {
            [Template] @Element MyElement {
                div {
                    text { "Hello from namespace" }
                }
            }
        }
        body {
            @Element MyElement from mySpace;
        }
    )";
    Lexer lexer(source);
    Parser parser(lexer);
    Generator generator;
    std::string result = generator.generate(*parser.parse());
    ASSERT_EQ(result, "<body><div>Hello from namespace</div></body>");
}

TEST(NamespaceTest, NestedNamespace) {
    std::string source = R"(
        [Namespace] outerSpace {
            [Namespace] innerSpace {
                [Template] @Element MyElement {
                    div {
                        text { "Hello from nested namespace" }
                    }
                }
            }
        }
        body {
            @Element MyElement from outerSpace.innerSpace;
        }
    )";
    Lexer lexer(source);
    Parser parser(lexer);
    Generator generator;
    std::string result = generator.generate(*parser.parse());
    ASSERT_EQ(result, "<body><div>Hello from nested namespace</div></body>");
}

TEST(NamespaceTest, VariableNamespace) {
    std::string source = R"(
        [Namespace] myVars {
            [Template] @Var mySet {
                myColor: "red";
            }
        }
        body {
            style {
                color: mySet(myColor) from myVars;
            }
        }
    )";
    Lexer lexer(source);
    Parser parser(lexer);
    Generator generator;
    std::string result = generator.generate(*parser.parse());
    ASSERT_EQ(result, "<body style=\"color:red;\"></body>");
}
