#pragma once

#include "Token.h"
#include <vector>
#include <string>

class Lexer {
public:
    Lexer(const std::string& source);
    std::vector<Token> tokenize();

private:
    std::string source;
    int current_pos;
    int line;
    int column;
};
