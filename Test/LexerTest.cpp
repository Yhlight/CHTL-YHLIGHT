#include "Test.h"
#include <stdexcept>
#include <iostream>
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

    tests.push_back({"Test All Keywords", []() {
        std::string source = R"(
            [Template] @Style inherit delete insert after before replace from as except use html5 text style script at top at bottom 5 ** 2
        )";

        Lexer lexer(source);
        std::vector<Token> tokens = lexer.tokenize();

        ASSERT(tokens.size() == 22);
        ASSERT(tokens[0].type == TokenType::TemplateKeyword);
        ASSERT(tokens[1].type == TokenType::StyleSpecifier);
        ASSERT(tokens[2].type == TokenType::InheritKeyword);
        ASSERT(tokens[3].type == TokenType::DeleteKeyword);
        ASSERT(tokens[4].type == TokenType::InsertKeyword);
        ASSERT(tokens[5].type == TokenType::AfterKeyword);
        ASSERT(tokens[6].type == TokenType::BeforeKeyword);
        ASSERT(tokens[7].type == TokenType::ReplaceKeyword);
        ASSERT(tokens[8].type == TokenType::FromKeyword);
        ASSERT(tokens[9].type == TokenType::AsKeyword);
        ASSERT(tokens[10].type == TokenType::ExceptKeyword);
        ASSERT(tokens[11].type == TokenType::UseKeyword);
        ASSERT(tokens[12].type == TokenType::Html5Keyword);
        ASSERT(tokens[13].type == TokenType::TextKeyword);
        ASSERT(tokens[14].type == TokenType::StyleKeyword);
        ASSERT(tokens[15].type == TokenType::ScriptKeyword);
        ASSERT(tokens[16].type == TokenType::AtTopKeyword);
        ASSERT(tokens[17].type == TokenType::AtBottomKeyword);
        ASSERT(tokens[18].type == TokenType::NumericLiteral);
        ASSERT(tokens[19].type == TokenType::DoubleStar);
        ASSERT(tokens[20].type == TokenType::NumericLiteral);
        ASSERT(tokens[21].type == TokenType::EndOfFile);
    }});
}
