#include <gtest/gtest.h>
#include "CHTL/CHTLLexer/Lexer.h"
#include "CHTL/CHTLParser/Parser.h"
#include "CHTL/CHTLAnalyser/Analyser.h"
#include "CHTL/CHTLNode/ProgramNode.h"
#include "CHTL/CHTLNode/ElementNode.h"
#include "CHTL/CHTLNode/StyleNode.h"
#include "CHTL/CHTLNode/StylePropertyNode.h"
#include "CHTL/CHTLNode/TemplateUsageNode.h"

TEST(AnalyserTest, ResolvesStyleTemplate) {
    std::string source =
        "[Template] @Style MyTheme {"
        "    color: red;"
        "}"
        "div {"
        "    style {"
        "        @Style MyTheme;"
        "    }"
        "}";
    CHTL::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    CHTL::Parser parser(tokens, source);
    auto ast = parser.parse();
    CHTL::Analyser analyser(*ast);
    analyser.analyse();

    CHTL::ProgramNode* programNode = static_cast<CHTL::ProgramNode*>(ast.get());
    ASSERT_EQ(programNode->children.size(), 1);

    CHTL::ElementNode* elementNode = static_cast<CHTL::ElementNode*>(programNode->children[0].get());
    ASSERT_EQ(elementNode->children.size(), 1);

    CHTL::StyleNode* styleNode = static_cast<CHTL::StyleNode*>(elementNode->children[0].get());
    ASSERT_EQ(styleNode->properties.size(), 1);

    CHTL::ASTNode* propBase = styleNode->properties[0].get();
    ASSERT_EQ(propBase->getType(), CHTL::ASTNodeType::StyleProperty);

    CHTL::StylePropertyNode* propNode = static_cast<CHTL::StylePropertyNode*>(propBase);
    EXPECT_EQ(propNode->name, "color");
    EXPECT_EQ(propNode->value, "red");
}

TEST(AnalyserTest, ResolvesCustomStyleTemplateWithSpecialization) {
    std::string source =
        "[Custom] @Style MyTheme {"
        "    color;"
        "    font-size: 16px;"
        "}"
        "div {"
        "    style {"
        "        @Style MyTheme {"
        "            color: blue;"
        "        }"
        "    }"
        "}";
    CHTL::Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    CHTL::Parser parser(tokens, source);
    auto ast = parser.parse();
    CHTL::Analyser analyser(*ast);
    analyser.analyse();

    CHTL::ProgramNode* programNode = static_cast<CHTL::ProgramNode*>(ast.get());
    ASSERT_EQ(programNode->children.size(), 1);

    CHTL::ElementNode* elementNode = static_cast<CHTL::ElementNode*>(programNode->children[0].get());
    ASSERT_EQ(elementNode->children.size(), 1);

    CHTL::StyleNode* styleNode = static_cast<CHTL::StyleNode*>(elementNode->children[0].get());
    ASSERT_EQ(styleNode->properties.size(), 2);

    CHTL::StylePropertyNode* prop1 = static_cast<CHTL::StylePropertyNode*>(styleNode->properties[0].get());
    EXPECT_EQ(prop1->name, "color");
    EXPECT_EQ(prop1->value, "blue");

    CHTL::StylePropertyNode* prop2 = static_cast<CHTL::StylePropertyNode*>(styleNode->properties[1].get());
    EXPECT_EQ(prop2->name, "font-size");
    EXPECT_EQ(prop2->value, "16px");
}
