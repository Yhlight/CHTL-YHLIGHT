#pragma once

#include <string>

enum class TokenType {
    // Keywords
    HtmlKeyword,
    HeadKeyword,
    BodyKeyword,
    TitleKeyword,
    DivKeyword,
    SpanKeyword,
    TextKeyword,
    StyleKeyword,
    ScriptKeyword,
    TemplateKeyword,
    CustomKeyword,
    OriginKeyword,
    ImportKeyword,
    NamespaceKeyword,

    // Punctuation
    OpenBrace,
    CloseBrace,
    OpenParen,
    CloseParen,
    OpenBracket,
    CloseBracket,
    Semicolon,
    Colon,
    Equals,
    Plus,
    Minus,
    Star,
    Slash,
    Percent,
    DoubleStar,
    Ampersand,
    Pipe,
    Comma,
    DoubleAsterisk,
    Asterisk,
    At,

    // Literals
    Identifier,
    StringLiteral,
    NumericLiteral,

    // Comments
    LineComment,
    BlockComment,
    GeneratorComment,

    // Special
    StyleSpecifier,
    ElementSpecifier,
    VarSpecifier,
    HtmlSpecifier,
    JavaScriptSpecifier,
    ChtlSpecifier,
    CjmodSpecifier,
    FromKeyword,
    AsKeyword,
    InheritKeyword,
    DeleteKeyword,
    InsertKeyword,
    AfterKeyword,
    BeforeKeyword,
    ReplaceKeyword,
    AtTopKeyword,
    AtBottomKeyword,
    ExceptKeyword,
    UseKeyword,
    Html5Keyword,
    ConfigSpecifier,

    // End of File
    EndOfFile,
};

struct Token {
    TokenType type;
    std::string value;
    size_t line;
    size_t col;
    size_t pos;
};
