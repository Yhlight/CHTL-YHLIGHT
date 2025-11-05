#include "Lexer.h"
#include <cctype>

Lexer::Lexer(const std::string& source) : source(source), current_pos(0), line(1), column(1) {}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    while (current_pos < source.length()) {
        char current_char = source[current_pos];
        if (isspace(current_char)) {
            if (current_char == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            current_pos++;
            continue;
        }

        if (isalpha(current_char)) {
            std::string value;
            while (current_pos < source.length() && (isalnum(source[current_pos]) || source[current_pos] == '_')) {
                value += source[current_pos];
                current_pos++;
                column++;
            }
            tokens.push_back({TokenType::Identifier, value, line, column});
            continue;
        }

        if (current_char == '{') {
            tokens.push_back({TokenType::OpenBrace, "{", line, column});
            current_pos++;
            column++;
            continue;
        }

        if (current_char == '}') {
            tokens.push_back({TokenType::CloseBrace, "}", line, column});
            current_pos++;
            column++;
            continue;
        }

        if (current_char == ':') {
            tokens.push_back({TokenType::Colon, ":", line, column});
            current_pos++;
            column++;
            continue;
        }

        if (current_char == ';') {
            tokens.push_back({TokenType::Semicolon, ";", line, column});
            current_pos++;
            column++;
            continue;
        }

        if (current_char == '"') {
            std::string value;
            current_pos++; // Skip the opening quote
            while (current_pos < source.length() && source[current_pos] != '"') {
                value += source[current_pos];
                current_pos++;
                column++;
            }
            current_pos++; // Skip the closing quote
            tokens.push_back({TokenType::StringLiteral, value, line, column});
            continue;
        }

        // Unrecognized character
        current_pos++;
        column++;
    }

    tokens.push_back({TokenType::EndOfFile, "", line, column});
    return tokens;
}
