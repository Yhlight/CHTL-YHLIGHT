#include "../CHTLLexer/Lexer.h"
#include <iostream>
#include <vector>
#include <cassert>

void testTokenizationOfTextBlock() {
    std::string source = "text { \"This is a text block\" }";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();

    assert(tokens.size() == 5);
    assert(tokens[0].type == TokenType::Text);
    assert(tokens[0].value == "text");
    assert(tokens[1].type == TokenType::LBrace);
    assert(tokens[1].value == "{");
    assert(tokens[2].type == TokenType::String);
    assert(tokens[2].value == "This is a text block");
    assert(tokens[3].type == TokenType::RBrace);
    assert(tokens[3].value == "}");
    assert(tokens[4].type == TokenType::EndOfFile);
}

int main() {
    testTokenizationOfTextBlock();
    std::cout << "All lexer tests passed!" << std::endl;
    return 0;
}
