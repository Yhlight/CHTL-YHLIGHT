#include <gtest/gtest.h>
#include "CHTLJS/CHTLJSLexer/Lexer.h"
#include "CHTLJS/CHTLJSParser/Parser.h"
#include "CHTLJS/CHTLJSNode/ProgramNode.h"
#include "CHTLJS/CHTLJSNode/LiteralExprNode.h"
#include "CHTLJS/CHTLJSNode/SelectorExprNode.h"
#include "CHTLJS/CHTLJSNode/BinaryExprNode.h"

using namespace CHTLJS;

TEST(CHTLJSParserTest, ParseLiteral) {
    std::string source = "123;";
    Lexer lexer(source);
    Parser parser(lexer);

    auto program = parser.parse();
    ASSERT_NE(program, nullptr);

    const auto& expressions = program->getExpressions();
    ASSERT_EQ(expressions.size(), 1);

    const auto& expr = expressions[0];
    ASSERT_EQ(expr->getType(), ExprNodeType::Literal);

    auto* literalExpr = static_cast<LiteralExprNode*>(expr.get());
    EXPECT_EQ(literalExpr->getValue(), "123");
}

TEST(CHTLJSParserTest, ParseSelector) {
    std::string source = "{{#my-id}};";
    Lexer lexer(source);
    Parser parser(lexer);

    auto program = parser.parse();
    ASSERT_NE(program, nullptr);

    const auto& expressions = program->getExpressions();
    ASSERT_EQ(expressions.size(), 1);

    const auto& expr = expressions[0];
    ASSERT_EQ(expr->getType(), ExprNodeType::Selector);

    auto* selectorExpr = static_cast<SelectorExprNode*>(expr.get());
    EXPECT_EQ(selectorExpr->getSelector(), "#my-id");
}

TEST(CHTLJSParserTest, ParseBinaryExpression) {
    std::string source = "1 + 2;";
    Lexer lexer(source);
    Parser parser(lexer);

    auto program = parser.parse();
    ASSERT_NE(program, nullptr);

    const auto& expressions = program->getExpressions();
    ASSERT_EQ(expressions.size(), 1);

    const auto& expr = expressions[0];
    ASSERT_EQ(expr->getType(), ExprNodeType::Binary);

    auto* binaryExpr = static_cast<BinaryExprNode*>(expr.get());

    auto* left = static_cast<const LiteralExprNode*>(binaryExpr->getLeft());
    EXPECT_EQ(left->getValue(), "1");

    EXPECT_EQ(binaryExpr->getOperator().type, TokenType::PLUS);

    auto* right = static_cast<const LiteralExprNode*>(binaryExpr->getRight());
    EXPECT_EQ(right->getValue(), "2");
}
