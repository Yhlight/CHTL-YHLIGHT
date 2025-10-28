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

void testTokenizationOfComments() {
    std::string source =
        "// This is a single-line comment\n"
        "/* This is a multi-line comment */\n"
        "# This is a generator comment";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();

    assert(tokens.size() == 2);
    assert(tokens[0].type == TokenType::Comment);
    assert(tokens[0].value == " This is a generator comment");
    assert(tokens[1].type == TokenType::EndOfFile);
}

void testTokenizationOfElementsAndAttributes() {
    std::string source = "div { id: \"box\"; class = \"container\"; }";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();

    assert(tokens.size() == 12);
    assert(tokens[0].type == TokenType::Identifier);
    assert(tokens[0].value == "div");
    assert(tokens[1].type == TokenType::LBrace);
    assert(tokens[1].value == "{");
    assert(tokens[2].type == TokenType::Identifier);
    assert(tokens[2].value == "id");
    assert(tokens[3].type == TokenType::Colon);
    assert(tokens[3].value == ":");
    assert(tokens[4].type == TokenType::String);
    assert(tokens[4].value == "box");
    assert(tokens[5].type == TokenType::Semicolon);
    assert(tokens[5].value == ";");
    assert(tokens[6].type == TokenType::Identifier);
    assert(tokens[6].value == "class");
    assert(tokens[7].type == TokenType::Equal);
    assert(tokens[7].value == "=");
    assert(tokens[8].type == TokenType::String);
    assert(tokens[8].value == "container");
    assert(tokens[9].type == TokenType::Semicolon);
    assert(tokens[9].value == ";");
    assert(tokens[10].type == TokenType::RBrace);
    assert(tokens[10].value == "}");
    assert(tokens[11].type == TokenType::EndOfFile);
}

int main() {
    testTokenizationOfTextBlock();
    testTokenizationOfComments();
    testTokenizationOfElementsAndAttributes();
    std::cout << "All lexer tests passed!" << std::endl;
    return 0;
}
