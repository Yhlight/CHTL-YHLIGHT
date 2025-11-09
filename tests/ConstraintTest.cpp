#include "gtest/gtest.h"
#include "CHTLLexer/Lexer.h"
#include "CHTLParser/Parser.h"
#include "CHTLGenerator/Generator.h"
#include "CHTL/Configuration.h"

TEST(ConstraintTest, ElementConstraint) {
    std::string source = R"(
        div {
            except span;
            span {}
        }
    )";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto program = parser.parse();
    CHTL::Configuration config;
    CHTL::Generator generator(config);
    ASSERT_THROW(generator.generate(*program), std::runtime_error);
}

TEST(ConstraintTest, TemplateConstraint) {
    std::string source = R"(
        [Template] @Element MyElement {}
        div {
            except [Template] @Element MyElement;
            @Element MyElement;
        }
    )";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto program = parser.parse();
    CHTL::Configuration config;
    CHTL::Generator generator(config);
    ASSERT_THROW(generator.generate(*program), std::runtime_error);
}
