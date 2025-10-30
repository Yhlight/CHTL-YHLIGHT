#include "gtest/gtest.h"
#include "CHTL/Lexer.h"
#include "CHTL/CHTLParser/Parser.h"
#include "CHTL/CHTLGenerator/Generator.h"
#include <string>

TEST(StyleBlockTest, GeneratesInlineStyles) {
    std::string source = "div { style { color: red; font-size: 16px; } }";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto ast = parser.parse();
    CHTL::Generator generator(ast, parser.getStyleTemplates(), parser.getElementTemplates(), parser.getVarTemplates());
    std::string result = generator.generate();
    std::string expected = "<html><head></head><body><div style=\"color:red;font-size:16px;\"></div></body></html>";
    ASSERT_EQ(result, expected);
}

TEST(StyleBlockTest, DISABLED_AutoAddsClassAttribute) {
    std::string source = "div { style { .my-class { color: blue; } } }";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto ast = parser.parse();
    CHTL::Generator generator(ast, parser.getStyleTemplates(), parser.getElementTemplates(), parser.getVarTemplates());
    std::string result = generator.generate();
    std::string expected = "<html><head><style>.my-class{color:blue;}</style></head><body><div class=\"my-class\"></div></body></html>";
    ASSERT_EQ(result, expected);
}

TEST(StyleBlockTest, DISABLED_AutoAddsIdAttribute) {
    std::string source = "div { style { #my-id { text-align: center; } } }";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto ast = parser.parse();
    CHTL::Generator generator(ast, parser.getStyleTemplates(), parser.getElementTemplates(), parser.getVarTemplates());
    std::string result = generator.generate();
    std::string expected = "<html><head><style>#my-id{text-align:center;}</style></head><body><div id=\"my-id\"></div></body></html>";
    ASSERT_EQ(result, expected);
}

TEST(StyleBlockTest, DISABLED_ContextDeductionForHover) {
    std::string source = "button { style { .btn { background: #fff; } &:hover { background: #000; } } }";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto ast = parser.parse();
    CHTL::Generator generator(ast, parser.getStyleTemplates(), parser.getElementTemplates(), parser.getVarTemplates());
    std::string result = generator.generate();
    std::string expected = "<html><head><style>.btn{background:#fff;}.btn:hover{background:#000;}</style></head><body><button class=\"btn\"></button></body></html>";
    ASSERT_EQ(result, expected);
}

TEST(StyleBlockTest, DISABLED_MixOfInlineAndHoistedStyles) {
    std::string source = "div { style { width: 100px; .container { display: flex; } } }";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto ast = parser.parse();
    CHTL::Generator generator(ast, parser.getStyleTemplates(), parser.getElementTemplates(), parser.getVarTemplates());
    std::string result = generator.generate();
    std::string expected = "<html><head><style>.container{display:flex;}</style></head><body><div style=\"width:100px;\" class=\"container\"></div></body></html>";
    ASSERT_EQ(result, expected);
}
