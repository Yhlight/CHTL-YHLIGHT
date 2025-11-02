#include "gtest/gtest.h"
#include "CHTL/CHTLLexer/Lexer.h"
#include "CHTL/CHTLParser/Parser.h"
#include "CHTL/CHTLNode/ConfigurationNode.h"
#include "CHTL/CHTLNode/ProgramNode.h"

TEST(ParserTest, ConfigurationBlock) {
    std::string source = R"(
        [Configuration] @MyConfig {
            INDEX_INITIAL_COUNT = 1;
            DEBUG_MODE = true;

            [Name] {
                KEYWORD_TEXT = text2;
            }
        }
    )";

    CHTL::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    CHTL::Parser parser(tokens, source);
    auto program = parser.parse();

    ASSERT_NE(program, nullptr);
    auto programNode = dynamic_cast<CHTL::ProgramNode*>(program.get());
    ASSERT_NE(programNode, nullptr);
    ASSERT_EQ(programNode->children.size(), 1);

    auto configNode = dynamic_cast<CHTL::ConfigurationNode*>(programNode->children[0].get());
    ASSERT_NE(configNode, nullptr);
    ASSERT_TRUE(configNode->name.has_value());
    ASSERT_EQ(configNode->name.value(), "@MyConfig");
    ASSERT_EQ(configNode->settings.size(), 2);
    ASSERT_EQ(configNode->settings["INDEX_INITIAL_COUNT"], "1");
    ASSERT_EQ(configNode->settings["DEBUG_MODE"], "true");
    ASSERT_EQ(configNode->nameGroup.size(), 1);
    ASSERT_EQ(configNode->nameGroup["KEYWORD_TEXT"], "text2");
}
