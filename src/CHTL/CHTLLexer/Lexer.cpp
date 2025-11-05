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

        int start_pos = current_pos;

        if (source.substr(current_pos, 10) == "[Template]") {
            tokens.push_back({TokenType::TemplateKeyword, "[Template]", line, column, start_pos});
            current_pos += 10;
            column += 10;
            continue;
        }

        if (source.substr(current_pos, 8) == "[Custom]") {
            tokens.push_back({TokenType::CustomKeyword, "[Custom]", line, column, start_pos});
            current_pos += 8;
            column += 8;
            continue;
        }

        if (source.substr(current_pos, 8) == "[Origin]") {
            tokens.push_back({TokenType::OriginKeyword, "[Origin]", line, column, start_pos});
            current_pos += 8;
            column += 8;
            continue;
        }

        if (source.substr(current_pos, 8) == "[Import]") {
            tokens.push_back({TokenType::ImportKeyword, "[Import]", line, column, start_pos});
            current_pos += 8;
            column += 8;
            continue;
        }

        if (source.substr(current_pos, 11) == "[Namespace]") {
            tokens.push_back({TokenType::NamespaceKeyword, "[Namespace]", line, column, start_pos});
            current_pos += 11;
            column += 11;
            continue;
        }

        if (source.substr(current_pos, 15) == "[Configuration]") {
            tokens.push_back({TokenType::ConfigurationKeyword, "[Configuration]", line, column, start_pos});
            current_pos += 15;
            column += 15;
            continue;
        }

        if (isalpha(current_char) || current_char == '-' || isdigit(current_char)) {
            std::string value;
            while (current_pos < source.length() && (isalnum(source[current_pos]) || source[current_pos] == '_' || source[current_pos] == '-' || source[current_pos] == '%')) {
                value += source[current_pos];
                current_pos++;
                column++;
            }
            if (value == "from") {
                tokens.push_back({TokenType::FromKeyword, value, line, column, start_pos});
            } else if (value == "as") {
                tokens.push_back({TokenType::AsKeyword, value, line, column, start_pos});
            } else {
                tokens.push_back({TokenType::Identifier, value, line, column, start_pos});
            }
            continue;
        }

        if (current_char == '{') {
            tokens.push_back({TokenType::OpenBrace, "{", line, column, start_pos});
            current_pos++;
            column++;
            continue;
        }

        if (current_char == '}') {
            tokens.push_back({TokenType::CloseBrace, "}", line, column, start_pos});
            current_pos++;
            column++;
            continue;
        }

        if (current_char == ':') {
            tokens.push_back({TokenType::Colon, ":", line, column, start_pos});
            current_pos++;
            column++;
            continue;
        }

        if (current_char == '=') {
            tokens.push_back({TokenType::Equal, "=", line, column, start_pos});
            current_pos++;
            column++;
            continue;
        }

        if (current_char == ';') {
            tokens.push_back({TokenType::Semicolon, ";", line, column, start_pos});
            current_pos++;
            column++;
            continue;
        }

        if (current_char == '.') {
            tokens.push_back({TokenType::Dot, ".", line, column, start_pos});
            current_pos++;
            column++;
            continue;
        }

        if (current_char == '@') {
            tokens.push_back({TokenType::At, "@", line, column, start_pos});
            current_pos++;
            column++;
            continue;
        }

        if (current_char == '(') {
            tokens.push_back({TokenType::OpenParen, "(", line, column, start_pos});
            current_pos++;
            column++;
            continue;
        }

        if (current_char == ')') {
            tokens.push_back({TokenType::CloseParen, ")", line, column, start_pos});
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
            tokens.push_back({TokenType::StringLiteral, value, line, column, start_pos});
            continue;
        }

        // Unrecognized character
        current_pos++;
        column++;
    }

    tokens.push_back({TokenType::EndOfFile, "", line, column, (int)source.length()});
    return tokens;
}
