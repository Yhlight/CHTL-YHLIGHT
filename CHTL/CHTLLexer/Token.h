#pragma once

#include <string>
#include <utility>

enum class TokenType {
    // Keywords
    TemplateKeyword, // [Template]
    CustomKeyword, // [Custom]
    OriginKeyword, // [Origin]
    ImportKeyword, // [Import]
    NamespaceKeyword, // [Namespace]
    ConfigurationKeyword, // [Configuration]
    StyleSpecifier, // @Style
    ElementSpecifier, // @Element
    VarSpecifier, // @Var
    HtmlSpecifier, // @Html
    JavaScriptSpecifier, // @JavaScript
    ChtlSpecifier, // @Chtl
    CJmodSpecifier, // @CJmod
    InheritKeyword,
    DeleteKeyword,
    InsertKeyword,
    AfterKeyword,
    BeforeKeyword,
    ReplaceKeyword,
    AtTopKeyword,
    AtBottomKeyword,
    FromKeyword,
    AsKeyword,
    ExceptKeyword,
    UseKeyword,
    Html5Keyword,
    TextKeyword,
    StyleKeyword,
    ScriptKeyword,

    // Identifiers and literals
    Identifier,
    StringLiteral,
    NumericLiteral,

    // Punctuation
    OpenBrace,
    CloseBrace,
    OpenParen,
    CloseParen,
    Semicolon,
    Colon,
    Equals,
    Comma,

    // Comments
    LineComment,
    BlockComment,
    GeneratorComment,

    // Special Symbols
    At,
    Hash,
    Ampersand,

    // Operators
    Plus,
    Minus,
    Asterisk,
    Slash,
    Percent,
    DoubleAsterisk,

    // End-of-file
    EndOfFile,

    // Unknown
    Unknown,
};

struct Token {
    TokenType type;
    std::string value;
    size_t line;
    size_t column;
};
