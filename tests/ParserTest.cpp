#include <gtest/gtest.h>
#include "CHTLLexer/Lexer.h"
#include "CHTLParser/Parser.h"
#include "CHTLNode/ProgramNode.h"
#include "CHTLNode/ElementNode.h"

TEST(ParserTest, ParsesEmptyElement) {
    std::string source = "div {}";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);

    std::unique_ptr<CHTL::ProgramNode> program = parser.parse();

    ASSERT_NE(program, nullptr);
    ASSERT_EQ(program->statements.size(), 1);

    CHTL::BaseNode* stmt = program->statements[0].get();
    ASSERT_EQ(stmt->getType(), CHTL::NodeType::Element);

    CHTL::ElementNode* element = static_cast<CHTL::ElementNode*>(stmt);
    ASSERT_EQ(element->tagName, "div");
    ASSERT_TRUE(element->children.empty());
}

TEST(ParserTest, ParsesNestedElements) {
    std::string source = "html { body {} }";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);

    std::unique_ptr<CHTL::ProgramNode> program = parser.parse();

    ASSERT_NE(program, nullptr);
    ASSERT_EQ(program->statements.size(), 1);

    CHTL::BaseNode* root_stmt = program->statements[0].get();
    ASSERT_EQ(root_stmt->getType(), CHTL::NodeType::Element);

    CHTL::ElementNode* html_element = static_cast<CHTL::ElementNode*>(root_stmt);
    ASSERT_EQ(html_element->tagName, "html");
    ASSERT_EQ(html_element->children.size(), 1);

    CHTL::BaseNode* child_stmt = html_element->children[0].get();
    ASSERT_EQ(child_stmt->getType(), CHTL::NodeType::Element);

    CHTL::ElementNode* body_element = static_cast<CHTL::ElementNode*>(child_stmt);
    ASSERT_EQ(body_element->tagName, "body");
    ASSERT_TRUE(body_element->children.empty());
}
