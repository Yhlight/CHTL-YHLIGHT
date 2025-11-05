#include "gtest/gtest.h"
#include "Lexer.h"
#include "Parser.h"
#include "Generator.h"

TEST(NamespaceTest, SimpleNamespace) {
    std::string source = "[Namespace] test { div {} }";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();
    Parser parser(source, tokens);
    auto program = parser.parse();
    Generator generator(*program);
    std::string expected_html = "<html><head><style></style></head><body><div></div></body></html>";
    ASSERT_EQ(generator.generate(), expected_html);
}

TEST(NamespaceTest, NameCollision) {
    std::string source = R"(
        [Template] @Element MyElement {
            div { text: "global"; }
        }
        [Namespace] test {
            [Template] @Element MyElement {
                div { text: "namespaced"; }
            }
            @Element MyElement;
        }
    )";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();
    Parser parser(source, tokens);
    auto program = parser.parse();
    Generator generator(*program);
    std::string expected_html = "<html><head><style></style></head><body><div>namespaced</div></body></html>";
    ASSERT_EQ(generator.generate(), expected_html);
}
