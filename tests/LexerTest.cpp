#include <gtest/gtest.h>
#include "CHTLLexer/Lexer.h"

TEST(LexerTest, HandlesSingleQuotedStrings) {
    CHTL::Lexer lexer("'hello'");
    CHTL::Token token = lexer.getNextToken();
    ASSERT_EQ(token.type, CHTL::TokenType::String);
    ASSERT_EQ(token.value, "hello");
}
