#include <gtest/gtest.h>
#include "CHTL/CHTLLexer/Lexer.h"
#include "CHTL/CHTLParser/Parser.h"
#include "CHTL/CHTLAnalyser/Analyser.h"
#include "SharedCore/SymbolTable.h"
#include "CHTL/CHTLNode/ProgramNode.h"
#include "CHTL/CHTLNode/ElementNode.h"
#include "CHTL/CHTLNode/StyleNode.h"
#include "CHTL/CHTLNode/StylePropertyNode.h"
#include "CHTL/CHTLNode/LiteralValueNode.h"
#include "CHTL/CHTLNode/TemplateNode.h"

TEST(AnalyserTest, TemplateCollection) {
    std::string source = "[Template] @Style MyTheme { color: red; } div {}";
    Lexer lexer(source);
    Parser parser(lexer);
    auto program = parser.parse();
    ASSERT_NE(program, nullptr);

    SymbolTable symbolTable;
    Analyser analyser(symbolTable);
    analyser.analyse(*program);

    // Template should be in the symbol table
    TemplateNode* templateNode = symbolTable.lookup("MyTheme");
    ASSERT_NE(templateNode, nullptr);
    EXPECT_EQ(templateNode->getName(), "MyTheme");

    // Template should be removed from the AST
    const auto& children = program->getChildren();
    ASSERT_EQ(children.size(), 1);
    EXPECT_EQ(children[0]->getType(), ASTNodeType::Element);
}

TEST(AnalyserTest, StyleTemplateUsageResolution) {
    std::string source = R"(
        [Template] @Style MyTheme {
            color: blue;
            font-size: 16px;
        }
        div {
            style {
                @Style MyTheme;
            }
        }
    )";
    Lexer lexer(source);
    Parser parser(lexer);
    auto program = parser.parse();
    ASSERT_NE(program, nullptr);

    SymbolTable symbolTable;
    Analyser analyser(symbolTable);
    analyser.analyse(*program);

    const auto& children = program->getChildren();
    ASSERT_EQ(children.size(), 1);
    auto* elementNode = static_cast<ElementNode*>(children[0].get());

    const auto& elementChildren = elementNode->getChildren();
    ASSERT_EQ(elementChildren.size(), 1);
    auto* styleNode = static_cast<StyleNode*>(elementChildren[0].get());

    const auto& styleChildren = styleNode->getChildren();
    ASSERT_EQ(styleChildren.size(), 2);

    auto* prop1 = static_cast<StylePropertyNode*>(styleChildren[0].get());
    EXPECT_EQ(prop1->getName(), "color");
    auto* value1 = static_cast<LiteralValueNode*>(const_cast<ValueNode*>(prop1->getValue()));
    EXPECT_EQ(value1->getValue(), "blue");

    auto* prop2 = static_cast<StylePropertyNode*>(styleChildren[1].get());
    EXPECT_EQ(prop2->getName(), "font-size");
    auto* value2 = static_cast<LiteralValueNode*>(const_cast<ValueNode*>(prop2->getValue()));
    EXPECT_EQ(value2->getValue(), "16px");
}
