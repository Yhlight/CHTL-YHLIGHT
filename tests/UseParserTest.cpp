#include <gtest/gtest.h>
#include "CHTLLexer/Lexer.h"
#include "CHTLParser/Parser.h"
#include "CHTLNode/ProgramNode.h"
#include "CHTLNode/UseNode.h"

TEST(UseParserTest, ParsesUseHtml5) {
    std::string source = "use html5;";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);

    std::unique_ptr<CHTL::ProgramNode> program = parser.parse();

    ASSERT_NE(program, nullptr);
    ASSERT_EQ(program->statements.size(), 1);

    CHTL::BaseNode* stmt = program->statements[0].get();
    ASSERT_EQ(stmt->getType(), CHTL::NodeType::Use);

    CHTL::UseNode* useNode = static_cast<CHTL::UseNode*>(stmt);
    ASSERT_EQ(useNode->type, "html5");
}

TEST(UseParserTest, ParsesUseConfig) {
    std::string source = "use @Config Basic;";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);

    std::unique_ptr<CHTL::ProgramNode> program = parser.parse();

    ASSERT_NE(program, nullptr);
    ASSERT_EQ(program->statements.size(), 1);

    CHTL::BaseNode* stmt = program->statements[0].get();
    ASSERT_EQ(stmt->getType(), CHTL::NodeType::Use);

    CHTL::UseNode* useNode = static_cast<CHTL::UseNode*>(stmt);
    ASSERT_EQ(useNode->type, "@Config");
    ASSERT_EQ(useNode->configName, "Basic");
}
