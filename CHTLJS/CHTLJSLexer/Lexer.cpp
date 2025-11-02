#include "Lexer.h"

CHTLJSLexer::CHTLJSLexer(const std::string& source) : source(source) {}

std::vector<CHTLJSToken> CHTLJSLexer::tokenize() {
    std::vector<CHTLJSToken> tokens;
    while (pos < source.length()) {
        if (source.substr(pos, 2) == "{{") {
            pos += 2;
            size_t start = pos;
            while (pos < source.length() && source.substr(pos, 2) != "}}") {
                pos++;
            }
            if (pos < source.length()) {
                tokens.push_back({CHTLJSTokenType::EnhancedSelector, source.substr(start, pos - start)});
                pos += 2;
            }
        } else {
            pos++;
        }
    }
    return tokens;
}
