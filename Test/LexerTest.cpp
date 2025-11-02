#include "Test.h"
#include <stdexcept>
#include "CHTLLexer/Lexer.h"

#define ASSERT(condition) \
    if (!(condition)) { \
        throw std::runtime_error("Assertion failed at " + std::string(__FILE__) + ":" + std::to_string(__LINE__)); \
    }

void LexerTests() {
    tests.push_back({"Test Basic Tokenization", []() {
        std::string source = R"(
            // This is a comment
            # This is a generator comment
            div {
                id: "main";
            }
        )";

        Lexer lexer(source);
        std::vector<Token> tokens = lexer.tokenize();

        ASSERT(tokens.size() == 10);
        ASSERT(tokens[0].type == TokenType::LineComment);
        ASSERT(tokens[1].type == TokenType::GeneratorComment);
        ASSERT(tokens[2].type == TokenType::Identifier);
        ASSERT(tokens[2].value == "div");
        ASSERT(tokens[3].type == TokenType::OpenBrace);
        ASSERT(tokens[4].type == TokenType::Identifier);
        ASSERT(tokens[4].value == "id");
        ASSERT(tokens[5].type == TokenType::Colon);
        ASSERT(tokens[6].type == TokenType::StringLiteral);
        ASSERT(tokens[6].value == "main");
        ASSERT(tokens[7].type == TokenType::Semicolon);
        ASSERT(tokens[8].type == TokenType::CloseBrace);
        ASSERT(tokens[9].type == TokenType::EndOfFile);
    }});
}
