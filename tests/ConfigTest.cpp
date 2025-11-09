#include "gtest/gtest.h"
#include "CHTLLexer/Lexer.h"
#include "CHTLParser/Parser.h"
#include "CHTLGenerator/Generator.h"
#include "CHTL/Configuration.h"

TEST(ConfigTest, DebugMode) {
    std::string source = R"(
        [Configuration] {
            DEBUG_MODE: true;
        }
        div {}
    )";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto program = parser.parse();
    CHTL::Configuration config = parser.getConfiguration();
    CHTL::Generator generator(config);
    std::string result = generator.generate(*program);
    std::string expected = "<!-- Start Element: div --><div></div><!-- End Element: div -->";
    ASSERT_EQ(result, expected);
}
