#include "Test.h"
#include "../CHTLJS/CHTLJSLexer/Lexer.h"
#include <cassert>

Test CHTLJSLexerTest = {
    "CHTLJSLexer: Tokenize Expression with Arguments",
    []() {
        std::string source = "{{box}}->setAttribute(\"id\", \"main-box\")";
        CHTLJSLexer lexer(source);
        std::vector<CHTLJSToken> tokens = lexer.tokenize();
        assert(tokens.size() == 8);
        assert(tokens[0].type == CHTLJSTokenType::EnhancedSelector);
        assert(tokens[0].value == "box");
        assert(tokens[1].type == CHTLJSTokenType::ArrowOperator);
        assert(tokens[1].value == "->");
        assert(tokens[2].type == CHTLJSTokenType::Identifier);
        assert(tokens[2].value == "setAttribute");
        assert(tokens[3].type == CHTLJSTokenType::OpenParen);
        assert(tokens[3].value == "(");
        assert(tokens[4].type == CHTLJSTokenType::StringLiteral);
        assert(tokens[4].value == "id");
        assert(tokens[5].type == CHTLJSTokenType::Comma);
        assert(tokens[5].value == ",");
        assert(tokens[6].type == CHTLJSTokenType::StringLiteral);
        assert(tokens[6].value == "main-box");
        assert(tokens[7].type == CHTLJSTokenType::CloseParen);
        assert(tokens[7].value == ")");
    }
};

// This is a bit of a hack, but it's how the other tests are registered.
struct CHTLJSLexerTestRegister {
    CHTLJSLexerTestRegister() {
        tests.push_back(CHTLJSLexerTest);
    }
};

CHTLJSLexerTestRegister CHTLJSLexerTestRegister_instance;
