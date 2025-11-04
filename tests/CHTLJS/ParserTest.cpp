#include "gtest/gtest.h"
#include "CHTLJS/CHTLLexer/Lexer.h"
#include "CHTLJS/CHTLParser/Parser.h"
#include "CHTLJS/CHTLNode/BinaryExprNode.h"
#include "CHTLJS/CHTLNode/LiteralNode.h"
#include "CHTLJS/CHTLNode/SelectorExprNode.h"
#include "CHTLJS/CHTLNode/ProgramNode.h"

TEST(CHTLJSParserTest, ParsesLiteral) {
    std::string source = "123";
    CHTLJS::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    CHTLJS::Parser parser(tokens);
    auto program = parser.parse();

    ASSERT_NE(program, nullptr);
    ASSERT_EQ(program->statements.size(), 1);
    auto& expr = program->statements[0];
    ASSERT_EQ(expr->getType(), CHTLJS::ASTNodeType::Literal);
    auto literal = static_cast<CHTLJS::LiteralNode*>(expr.get());
    EXPECT_EQ(literal->value, "123");
}

TEST(CHTLJSParserTest, ParsesTagSelector) {
    std::string source = "{{mySelector}}";
    CHTLJS::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    CHTLJS::Parser parser(tokens);
    auto program = parser.parse();

    ASSERT_NE(program, nullptr);
    ASSERT_EQ(program->statements.size(), 1);
    auto& expr = program->statements[0];
    ASSERT_EQ(expr->getType(), CHTLJS::ASTNodeType::SelectorExpr);
    auto selector = static_cast<CHTLJS::SelectorExprNode*>(expr.get());
    EXPECT_EQ(selector->type, CHTLJS::SelectorType::Tag);
    EXPECT_EQ(selector->baseName, "mySelector");
}

TEST(CHTLJSParserTest, ParsesIdSelector) {
    std::string source = "{{#myId}}";
    CHTLJS::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    CHTLJS::Parser parser(tokens);
    auto program = parser.parse();

    ASSERT_NE(program, nullptr);
    ASSERT_EQ(program->statements.size(), 1);
    auto& expr = program->statements[0];
    ASSERT_EQ(expr->getType(), CHTLJS::ASTNodeType::SelectorExpr);
    auto selector = static_cast<CHTLJS::SelectorExprNode*>(expr.get());
    EXPECT_EQ(selector->type, CHTLJS::SelectorType::Id);
    EXPECT_EQ(selector->baseName, "myId");
}

TEST(CHTLJSParserTest, ParsesClassSelector) {
    std::string source = "{{.myClass}}";
    CHTLJS::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    CHTLJS::Parser parser(tokens);
    auto program = parser.parse();

    ASSERT_NE(program, nullptr);
    ASSERT_EQ(program->statements.size(), 1);
    auto& expr = program->statements[0];
    ASSERT_EQ(expr->getType(), CHTLJS::ASTNodeType::SelectorExpr);
    auto selector = static_cast<CHTLJS::SelectorExprNode*>(expr.get());
    EXPECT_EQ(selector->type, CHTLJS::SelectorType::Class);
    EXPECT_EQ(selector->baseName, "myClass");
}

TEST(CHTLJSParserTest, ParsesCompoundSelector) {
    std::string source = "{{.myClass myDescendant}}";
    CHTLJS::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    CHTLJS::Parser parser(tokens);
    auto program = parser.parse();

    ASSERT_NE(program, nullptr);
    ASSERT_EQ(program->statements.size(), 1);
    auto& expr = program->statements[0];
    ASSERT_EQ(expr->getType(), CHTLJS::ASTNodeType::SelectorExpr);
    auto selector = static_cast<CHTLJS::SelectorExprNode*>(expr.get());
    EXPECT_EQ(selector->type, CHTLJS::SelectorType::Compound);
    EXPECT_EQ(selector->baseName, "myClass");
    EXPECT_EQ(selector->descendant, "myDescendant");
}

TEST(CHTLJSParserTest, ParsesIndexedSelector) {
    std::string source = "{{myTag[0]}}";
    CHTLJS::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    CHTLJS::Parser parser(tokens);
    auto program = parser.parse();

    ASSERT_NE(program, nullptr);
    ASSERT_EQ(program->statements.size(), 1);
    auto& expr = program->statements[0];
    ASSERT_EQ(expr->getType(), CHTLJS::ASTNodeType::SelectorExpr);
    auto selector = static_cast<CHTLJS::SelectorExprNode*>(expr.get());
    EXPECT_EQ(selector->type, CHTLJS::SelectorType::Tag);
    EXPECT_EQ(selector->baseName, "myTag");
    ASSERT_TRUE(selector->index.has_value());
    EXPECT_EQ(selector->index.value(), 0);
}

TEST(CHTLJSParserTest, ParsesBinaryExpression) {
    std::string source = "1 + 2";
    CHTLJS::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    CHTLJS::Parser parser(tokens);
    auto program = parser.parse();

    ASSERT_NE(program, nullptr);
    ASSERT_EQ(program->statements.size(), 1);
    auto& expr = program->statements[0];
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
    auto program = parser.parse();

    ASSERT_NE(program, nullptr);
    ASSERT_EQ(program->statements.size(), 1);
    auto& expr = program->statements[0];
    ASSERT_EQ(expr->getType(), CHTLJS::ASTNodeType::BinaryExpr);
    auto topExpr = static_cast<CHTLJS::BinaryExprNode*>(expr.get());
    EXPECT_EQ(topExpr->op.type, CHTLJS::TokenType::Plus);

    ASSERT_EQ(topExpr->right->getType(), CHTLJS::ASTNodeType::BinaryExpr);
    auto rightExpr = static_cast<CHTLJS::BinaryExprNode*>(topExpr->right.get());
    EXPECT_EQ(rightExpr->op.type, CHTLJS::TokenType::Star);
}
