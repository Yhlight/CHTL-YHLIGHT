#pragma once

#include <string>
#include <vector>

enum class CHTLJSTokenType {
    EnhancedSelector,
    Identifier,
    ArrowOperator,
    OpenParen,
    CloseParen,
    Comma,
    StringLiteral,
    // ... other token types
};

struct CHTLJSToken {
    CHTLJSTokenType type;
    std::string value;
};
