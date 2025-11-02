#include "gtest/gtest.h"
#include "CHTL/CHTLLexer/Lexer.h"
#include "CHTL/CHTLParser/Parser.h"
#include "CHTL/CHTLAnalyser/Analyser.h"
#include "CHTL/CHTLNode/ProgramNode.h"
#include "SharedCore/CHTLContext.h"

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
    auto context = std::make_shared<CHTL::CHTLContext>();
    CHTL::Analyser analyser(*program, "", importStack, context);
    analyser.analyse();

    ASSERT_EQ(context->config.size(), 2);
    ASSERT_EQ(context->config["INDEX_INITIAL_COUNT"], "1");
    ASSERT_EQ(context->config["DEBUG_MODE"], "true");
}
