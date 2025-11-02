#pragma once

#include <string>
#include <vector>

enum class CHTLJSTokenType {
    EnhancedSelector,
    Identifier,
    // ... other token types
};

struct CHTLJSToken {
    CHTLJSTokenType type;
    std::string value;
};
