#include "gtest/gtest.h"
#include "CHTL/CHTLLexer/Lexer.h"
#include "CHTL/CHTLParser/Parser.h"
#include "CHTL/CHTLAnalyser/Analyser.h"
#include "CHTL/CHTLNode/ProgramNode.h"

TEST(AnalyserTest, ProcessesConfigurationBlock) {
    std::string source = R"(
        [Configuration] {
            INDEX_INITIAL_COUNT = 1;
            DEBUG_MODE = true;
        }
    )";
    std::vector<std::string> importStack;

    CHTL::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    CHTL::Parser parser(tokens, source);
    auto program = parser.parse();
    CHTL::Analyser analyser(*program, "", importStack);
    analyser.analyse();

    auto config = analyser.getConfig();
    ASSERT_EQ(config.size(), 2);
    ASSERT_EQ(config["INDEX_INITIAL_COUNT"], "1");
    ASSERT_EQ(config["DEBUG_MODE"], "true");
}
