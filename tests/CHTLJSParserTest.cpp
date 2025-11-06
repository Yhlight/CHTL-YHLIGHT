#include <gtest/gtest.h>
#include "CHTLJS/CHTLJSLexer/Lexer.h"
#include "CHTLJS/CHTLJSParser/Parser.h"
#include "CHTLJS/CHTLJSNode/ProgramNode.h"
#include "CHTLJS/CHTLJSNode/LiteralExprNode.h"
#include "CHTLJS/CHTLJSNode/SelectorExprNode.h"
#include "CHTLJS/CHTLJSNode/BinaryExprNode.h"
#include "CHTLJS/CHTLJSNode/CallExprNode.h"
#include "CHTLJS/CHTLJSNode/MemberAccessExprNode.h"

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
    const auto& components = selectorExpr->getComponents();
    ASSERT_EQ(components.size(), 1);
    EXPECT_EQ(components[0].type, SelectorComponentType::ID);
    EXPECT_EQ(components[0].value, "my-id");
}

TEST(CHTLJSParserTest, ParseTagSelector) {
    std::string source = "{{my-tag}};";
    Lexer lexer(source);
    Parser parser(lexer);

    auto program = parser.parse();
    ASSERT_NE(program, nullptr);

    const auto& expressions = program->getExpressions();
    ASSERT_EQ(expressions.size(), 1);

    const auto& expr = expressions[0];
    ASSERT_EQ(expr->getType(), ExprNodeType::Selector);

    auto* selectorExpr = static_cast<SelectorExprNode*>(expr.get());
    const auto& components = selectorExpr->getComponents();
    ASSERT_EQ(components.size(), 1);
    EXPECT_EQ(components[0].type, SelectorComponentType::TAG);
    EXPECT_EQ(components[0].value, "my-tag");
}

TEST(CHTLJSParserTest, ParseComplexSelector) {
    std::string source = "{{.box button[0]}};";
    Lexer lexer(source);
    Parser parser(lexer);

    auto program = parser.parse();
    ASSERT_NE(program, nullptr);

    const auto& expressions = program->getExpressions();
    ASSERT_EQ(expressions.size(), 1);

    const auto& expr = expressions[0];
    ASSERT_EQ(expr->getType(), ExprNodeType::Selector);

    auto* selectorExpr = static_cast<SelectorExprNode*>(expr.get());
    const auto& components = selectorExpr->getComponents();
    ASSERT_EQ(components.size(), 4);
    EXPECT_EQ(components[0].type, SelectorComponentType::CLASS);
    EXPECT_EQ(components[0].value, "box");
    EXPECT_EQ(components[1].type, SelectorComponentType::DESCENDANT);
    EXPECT_EQ(components[1].value, " ");
    EXPECT_EQ(components[2].type, SelectorComponentType::TAG);
    EXPECT_EQ(components[2].value, "button");
    EXPECT_EQ(components[3].type, SelectorComponentType::INDEX);
    EXPECT_EQ(components[3].value, "0");
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

TEST(CHTLJSParserTest, ParseCallExpression) {
    std::string source = "{{myObject}}->myMethod();";
    Lexer lexer(source);
    Parser parser(lexer);

    auto program = parser.parse();
    ASSERT_NE(program, nullptr);

    const auto& expressions = program->getExpressions();
    ASSERT_EQ(expressions.size(), 1);

    const auto& expr = expressions[0];
    ASSERT_EQ(expr->getType(), ExprNodeType::Call);

    auto* callExpr = static_cast<CallExprNode*>(expr.get());
    const auto& callee = callExpr->getCallee();
    ASSERT_EQ(callee.getType(), ExprNodeType::MemberAccess);

    auto* memberAccessExpr = static_cast<const MemberAccessExprNode*>(&callee);
    const auto& object = memberAccessExpr->getObject();
    ASSERT_EQ(object.getType(), ExprNodeType::Selector);

    auto* selectorExpr = static_cast<const SelectorExprNode*>(&object);
    const auto& components = selectorExpr->getComponents();
    ASSERT_EQ(components.size(), 1);
    EXPECT_EQ(components[0].type, SelectorComponentType::TAG);
    EXPECT_EQ(components[0].value, "myObject");

    EXPECT_EQ(memberAccessExpr->getMember().lexeme, "myMethod");

    const auto& args = callExpr->getArguments();
    EXPECT_EQ(args.size(), 0);
}
