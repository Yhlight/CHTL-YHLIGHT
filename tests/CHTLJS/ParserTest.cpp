#include "gtest/gtest.h"
#include "CHTLJS/CHTLLexer/Lexer.h"
#include "CHTLJS/CHTLParser/Parser.h"
#include "CHTLJS/CHTLNode/BinaryExprNode.h"
#include "CHTLJS/CHTLNode/LiteralNode.h"
#include "CHTLJS/CHTLNode/SelectorExprNode.h"

TEST(CHTLJSParserTest, ParsesLiteral) {
    std::string source = "123";
    CHTLJS::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    CHTLJS::Parser parser(tokens);
    auto expr = parser.parse();

    ASSERT_NE(expr, nullptr);
    ASSERT_EQ(expr->getType(), CHTLJS::ASTNodeType::Literal);
    auto literal = static_cast<CHTLJS::LiteralNode*>(expr.get());
    EXPECT_EQ(literal->value, "123");
}

TEST(CHTLJSParserTest, ParsesSelector) {
    std::string source = "{{mySelector}}";
    CHTLJS::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    CHTLJS::Parser parser(tokens);
    auto expr = parser.parse();

    ASSERT_NE(expr, nullptr);
    ASSERT_EQ(expr->getType(), CHTLJS::ASTNodeType::SelectorExpr);
    auto selector = static_cast<CHTLJS::SelectorExprNode*>(expr.get());
    EXPECT_EQ(selector->selector, "mySelector");
}

TEST(CHTLJSParserTest, ParsesBinaryExpression) {
    std::string source = "1 + 2";
    CHTLJS::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    CHTLJS::Parser parser(tokens);
    auto expr = parser.parse();

    ASSERT_NE(expr, nullptr);
    ASSERT_EQ(expr->getType(), CHTLJS::ASTNodeType::BinaryExpr);
    auto binaryExpr = static_cast<CHTLJS::BinaryExprNode*>(expr.get());
    EXPECT_EQ(binaryExpr->op.type, CHTLJS::TokenType::Plus);

    ASSERT_EQ(binaryExpr->left->getType(), CHTLJS::ASTNodeType::Literal);
    auto left = static_cast<CHTLJS::LiteralNode*>(binaryExpr->left.get());
    EXPECT_EQ(left->value, "1");

    ASSERT_EQ(binaryExpr->right->getType(), CHTLJS::ASTNodeType::Literal);
    auto right = static_cast<CHTLJS::LiteralNode*>(binaryExpr->right.get());
    EXPECT_EQ(right->value, "2");
}

TEST(CHTLJSParserTest, HandlesPrecedence) {
    std::string source = "1 + 2 * 3";
    CHTLJS::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    CHTLJS::Parser parser(tokens);
    auto expr = parser.parse();

    ASSERT_NE(expr, nullptr);
    ASSERT_EQ(expr->getType(), CHTLJS::ASTNodeType::BinaryExpr);
    auto topExpr = static_cast<CHTLJS::BinaryExprNode*>(expr.get());
    EXPECT_EQ(topExpr->op.type, CHTLJS::TokenType::Plus);

    ASSERT_EQ(topExpr->right->getType(), CHTLJS::ASTNodeType::BinaryExpr);
    auto rightExpr = static_cast<CHTLJS::BinaryExprNode*>(topExpr->right.get());
    EXPECT_EQ(rightExpr->op.type, CHTLJS::TokenType::Star);
}
