#include "Test.h"
#include "../CHTLJS/CHTLJSLexer/Lexer.h"
#include <cassert>

Test CHTLJSLexerTest = {
    "CHTLJSLexer: Tokenize Enhanced Selector",
    []() {
        std::string source = "{{.box}}";
        CHTLJSLexer lexer(source);
        std::vector<CHTLJSToken> tokens = lexer.tokenize();
        assert(tokens.size() == 1);
        assert(tokens[0].type == CHTLJSTokenType::EnhancedSelector);
        assert(tokens[0].value == ".box");
    }
};

// This is a bit of a hack, but it's how the other tests are registered.
struct CHTLJSLexerTestRegister {
    CHTLJSLexerTestRegister() {
        tests.push_back(CHTLJSLexerTest);
    }
};

CHTLJSLexerTestRegister CHTLJSLexerTestRegister_instance;
