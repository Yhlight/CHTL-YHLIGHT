#include <gtest/gtest.h>
#include "CHTLLexer/Lexer.h"
#include "CHTLParser/Parser.h"
#include "CHTLNode/ProgramNode.h"
#include "CHTLNode/ElementNode.h"
#include "CHTLNode/TextNode.h"
#include "CHTLNode/StyleNode.h"
#include "CHTLNode/StylePropertyNode.h"
#include "CHTLNode/StyleRuleNode.h"
#include "CHTLNode/ScriptNode.h"
#include "CHTLNode/OriginNode.h"
#include "CHTLNode/TemplateNode.h"
#include "CHTLNode/ValueNode.h"
#include "CHTLNode/LiteralValueNode.h"
#include "CHTLNode/VariableUsageNode.h"
#include "CHTLNode/ImportNode.h"

TEST(ParserTest, ParsesEmptyElement) {
    std::string source = "div {}";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);

    std::unique_ptr<CHTL::ProgramNode> program = parser.parse();

    ASSERT_NE(program, nullptr);
    ASSERT_EQ(program->statements.size(), 1);

    CHTL::BaseNode* stmt = program->statements[0].get();
    ASSERT_EQ(stmt->getType(), CHTL::NodeType::Element);

    CHTL::ElementNode* element = static_cast<CHTL::ElementNode*>(stmt);
    ASSERT_EQ(element->tagName, "div");
    ASSERT_TRUE(element->children.empty());
}

TEST(ParserTest, ParsesNestedElements) {
    std::string source = "html { body {} }";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);

    std::unique_ptr<CHTL::ProgramNode> program = parser.parse();

    ASSERT_NE(program, nullptr);
    ASSERT_EQ(program->statements.size(), 1);

    CHTL::BaseNode* root_stmt = program->statements[0].get();
    ASSERT_EQ(root_stmt->getType(), CHTL::NodeType::Element);

    CHTL::ElementNode* html_element = static_cast<CHTL::ElementNode*>(root_stmt);
    ASSERT_EQ(html_element->tagName, "html");
    ASSERT_EQ(html_element->children.size(), 1);

    CHTL::BaseNode* child_stmt = html_element->children[0].get();
    ASSERT_EQ(child_stmt->getType(), CHTL::NodeType::Element);

    CHTL::ElementNode* body_element = static_cast<CHTL::ElementNode*>(child_stmt);
    ASSERT_EQ(body_element->tagName, "body");
    ASSERT_TRUE(body_element->children.empty());
}

TEST(ParserTest, ParsesAttributes) {
    std::string source = "div { id: \"main\"; class = container; }";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);

    std::unique_ptr<CHTL::ProgramNode> program = parser.parse();

    ASSERT_NE(program, nullptr);
    ASSERT_EQ(program->statements.size(), 1);

    CHTL::BaseNode* stmt = program->statements[0].get();
    ASSERT_EQ(stmt->getType(), CHTL::NodeType::Element);

    CHTL::ElementNode* element = static_cast<CHTL::ElementNode*>(stmt);
    ASSERT_EQ(element->tagName, "div");
    ASSERT_EQ(element->attributes.size(), 2);
    ASSERT_EQ(element->attributes["id"], "main");
    ASSERT_EQ(element->attributes["class"], "container");
}

TEST(ParserTest, ParsesTextNode) {
    std::string source = "p { text { \"Hello, World!\" } }";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);

    std::unique_ptr<CHTL::ProgramNode> program = parser.parse();

    ASSERT_NE(program, nullptr);
    ASSERT_EQ(program->statements.size(), 1);

    CHTL::BaseNode* stmt = program->statements[0].get();
    ASSERT_EQ(stmt->getType(), CHTL::NodeType::Element);

    CHTL::ElementNode* p_element = static_cast<CHTL::ElementNode*>(stmt);
    ASSERT_EQ(p_element->tagName, "p");
    ASSERT_EQ(p_element->children.size(), 1);

    CHTL::BaseNode* child_stmt = p_element->children[0].get();
    ASSERT_EQ(child_stmt->getType(), CHTL::NodeType::Text);

    CHTL::TextNode* text_node = static_cast<CHTL::TextNode*>(child_stmt);
    ASSERT_EQ(text_node->content, "Hello, World!");
}

TEST(ParserTest, ParsesImportStatement) {
    std::string source = "[Import] @Html from \"./path/to/file.html\" as myFile";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);

    std::unique_ptr<CHTL::ProgramNode> program = parser.parse();

    ASSERT_NE(program, nullptr);
    ASSERT_EQ(program->statements.size(), 1);

    CHTL::BaseNode* stmt = program->statements[0].get();
    ASSERT_EQ(stmt->getType(), CHTL::NodeType::Import);

    CHTL::ImportNode* import_node = static_cast<CHTL::ImportNode*>(stmt);
    ASSERT_EQ(import_node->type, "Html");
    ASSERT_EQ(import_node->path, "./path/to/file.html");
    ASSERT_EQ(import_node->name, "myFile");
}

TEST(ParserTest, ParsesStyleBlockWithProperties) {
    std::string source = "div { style { color: red; width: 100px; } }";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);

    std::unique_ptr<CHTL::ProgramNode> program = parser.parse();

    ASSERT_NE(program, nullptr);
    ASSERT_EQ(program->statements.size(), 1);

    CHTL::BaseNode* stmt = program->statements[0].get();
    ASSERT_EQ(stmt->getType(), CHTL::NodeType::Element);

    CHTL::ElementNode* div_element = static_cast<CHTL::ElementNode*>(stmt);
    ASSERT_EQ(div_element->tagName, "div");
    ASSERT_EQ(div_element->children.size(), 1);

    CHTL::BaseNode* child_stmt = div_element->children[0].get();
    ASSERT_EQ(child_stmt->getType(), CHTL::NodeType::Style);

    CHTL::StyleNode* style_node = static_cast<CHTL::StyleNode*>(child_stmt);
    ASSERT_EQ(style_node->children.size(), 2);

    CHTL::StyleContentNode* prop1_node = style_node->children[0].get();
    ASSERT_EQ(prop1_node->getType(), CHTL::NodeType::StyleProperty);
    CHTL::StylePropertyNode* prop1 = static_cast<CHTL::StylePropertyNode*>(prop1_node);
    ASSERT_EQ(prop1->key, "color");
    ASSERT_EQ(prop1->value.size(), 1);
    CHTL::LiteralValueNode* val1 = static_cast<CHTL::LiteralValueNode*>(prop1->value[0].get());
    ASSERT_EQ(val1->value, "red");

    CHTL::StyleContentNode* prop2_node = style_node->children[1].get();
    ASSERT_EQ(prop2_node->getType(), CHTL::NodeType::StyleProperty);
    CHTL::StylePropertyNode* prop2 = static_cast<CHTL::StylePropertyNode*>(prop2_node);
    ASSERT_EQ(prop2->key, "width");
    ASSERT_EQ(prop2->value.size(), 1);
    CHTL::LiteralValueNode* val2 = static_cast<CHTL::LiteralValueNode*>(prop2->value[0].get());
    ASSERT_EQ(val2->value, "100px");
}

TEST(ParserTest, ParsesStyleBlockWithRules) {
    std::string source = "div { style { .box { width: 100px; } } }";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);

    std::unique_ptr<CHTL::ProgramNode> program = parser.parse();

    ASSERT_NE(program, nullptr);
    ASSERT_EQ(program->statements.size(), 1);

    CHTL::BaseNode* stmt = program->statements[0].get();
    ASSERT_EQ(stmt->getType(), CHTL::NodeType::Element);

    CHTL::ElementNode* div_element = static_cast<CHTL::ElementNode*>(stmt);
    ASSERT_EQ(div_element->tagName, "div");
    ASSERT_EQ(div_element->children.size(), 1);

    CHTL::BaseNode* child_stmt = div_element->children[0].get();
    ASSERT_EQ(child_stmt->getType(), CHTL::NodeType::Style);

    CHTL::StyleNode* style_node = static_cast<CHTL::StyleNode*>(child_stmt);
    ASSERT_EQ(style_node->children.size(), 1);

    CHTL::StyleContentNode* rule_node = style_node->children[0].get();
    ASSERT_EQ(rule_node->getType(), CHTL::NodeType::StyleRule);
    CHTL::StyleRuleNode* rule = static_cast<CHTL::StyleRuleNode*>(rule_node);
    ASSERT_EQ(rule->selector, ".box");
    ASSERT_EQ(rule->properties.size(), 1);
    ASSERT_EQ(rule->properties[0]->key, "width");
    ASSERT_EQ(rule->properties[0]->value.size(), 1);
    CHTL::LiteralValueNode* val = static_cast<CHTL::LiteralValueNode*>(rule->properties[0]->value[0].get());
    ASSERT_EQ(val->value, "100px");
}

TEST(ParserTest, ParsesScriptBlock) {
    std::string source = R"(div { script { "console.log('hello');" } })";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);

    std::unique_ptr<CHTL::ProgramNode> program = parser.parse();

    ASSERT_NE(program, nullptr);
    ASSERT_EQ(program->statements.size(), 1);

    CHTL::BaseNode* stmt = program->statements[0].get();
    ASSERT_EQ(stmt->getType(), CHTL::NodeType::Element);

    CHTL::ElementNode* div_element = static_cast<CHTL::ElementNode*>(stmt);
    ASSERT_EQ(div_element->tagName, "div");
    ASSERT_EQ(div_element->children.size(), 1);

    CHTL::BaseNode* child_stmt = div_element->children[0].get();
    ASSERT_EQ(child_stmt->getType(), CHTL::NodeType::Script);

    CHTL::ScriptNode* script_node = static_cast<CHTL::ScriptNode*>(child_stmt);
    ASSERT_EQ(script_node->content, "console.log('hello');");
}

TEST(ParserTest, ParsesOriginBlock) {
    std::string source = R"([Origin] @Html { "<div></div>" })";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);

    std::unique_ptr<CHTL::ProgramNode> program = parser.parse();

    ASSERT_NE(program, nullptr);
    ASSERT_EQ(program->statements.size(), 1);

    CHTL::BaseNode* stmt = program->statements[0].get();
    ASSERT_EQ(stmt->getType(), CHTL::NodeType::Origin);

    CHTL::OriginNode* origin_node = static_cast<CHTL::OriginNode*>(stmt);
    ASSERT_EQ(origin_node->originType, "Html");
    ASSERT_EQ(origin_node->content, "<div></div>");
}

TEST(ParserTest, ParsesStyleGroupTemplate) {
    std::string source = R"(
        [Template] @Style DefaultText {
            color: "black";
            line-height: 1.6;
        }
    )";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);

    std::unique_ptr<CHTL::ProgramNode> program = parser.parse();

    ASSERT_NE(program, nullptr);
    ASSERT_EQ(program->statements.size(), 1);

    CHTL::BaseNode* stmt = program->statements[0].get();
    ASSERT_EQ(stmt->getType(), CHTL::NodeType::Template);

    CHTL::TemplateNode* template_node = static_cast<CHTL::TemplateNode*>(stmt);
    ASSERT_EQ(template_node->type, "Style");
    ASSERT_EQ(template_node->name, "DefaultText");
    ASSERT_EQ(template_node->body.size(), 2);

    CHTL::StylePropertyNode* prop1 = static_cast<CHTL::StylePropertyNode*>(template_node->body[0].get());
    ASSERT_EQ(prop1->key, "color");
    ASSERT_EQ(prop1->value.size(), 1);
    CHTL::LiteralValueNode* val1 = static_cast<CHTL::LiteralValueNode*>(prop1->value[0].get());
    ASSERT_EQ(val1->value, "black");

    CHTL::StylePropertyNode* prop2 = static_cast<CHTL::StylePropertyNode*>(template_node->body[1].get());
    ASSERT_EQ(prop2->key, "line-height");
    ASSERT_EQ(prop2->value.size(), 1);
    CHTL::LiteralValueNode* val2 = static_cast<CHTL::LiteralValueNode*>(prop2->value[0].get());
    ASSERT_EQ(val2->value, "1.6");
}

TEST(ParserTest, ParsesElementTemplate) {
    std::string source = R"(
        [Template] @Element Box {
            div {
                text { "Hello" }
            }
        }
    )";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);

    std::unique_ptr<CHTL::ProgramNode> program = parser.parse();

    ASSERT_NE(program, nullptr);
    ASSERT_EQ(program->statements.size(), 1);

    CHTL::BaseNode* stmt = program->statements[0].get();
    ASSERT_EQ(stmt->getType(), CHTL::NodeType::Template);

    CHTL::TemplateNode* template_node = static_cast<CHTL::TemplateNode*>(stmt);
    ASSERT_EQ(template_node->type, "Element");
    ASSERT_EQ(template_node->name, "Box");
    ASSERT_EQ(template_node->body.size(), 1);

    CHTL::ElementNode* div_element = static_cast<CHTL::ElementNode*>(template_node->body[0].get());
    ASSERT_EQ(div_element->tagName, "div");
    ASSERT_EQ(div_element->children.size(), 1);

    CHTL::TextNode* text_node = static_cast<CHTL::TextNode*>(div_element->children[0].get());
    ASSERT_EQ(text_node->content, "Hello");
}

TEST(ParserTest, ParsesVarGroupTemplate) {
    std::string source = R"(
        [Template] @Var Theme {
            textColor: "black";
            backgroundColor: "white";
        }
    )";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);

    std::unique_ptr<CHTL::ProgramNode> program = parser.parse();

    ASSERT_NE(program, nullptr);
    ASSERT_EQ(program->statements.size(), 1);

    CHTL::BaseNode* stmt = program->statements[0].get();
    ASSERT_EQ(stmt->getType(), CHTL::NodeType::Template);

    CHTL::TemplateNode* template_node = static_cast<CHTL::TemplateNode*>(stmt);
    ASSERT_EQ(template_node->type, "Var");
    ASSERT_EQ(template_node->name, "Theme");
    ASSERT_EQ(template_node->body.size(), 2);

    CHTL::StylePropertyNode* prop1 = static_cast<CHTL::StylePropertyNode*>(template_node->body[0].get());
    ASSERT_EQ(prop1->key, "textColor");
    ASSERT_EQ(prop1->value.size(), 1);
    CHTL::LiteralValueNode* val1 = static_cast<CHTL::LiteralValueNode*>(prop1->value[0].get());
    ASSERT_EQ(val1->value, "black");

    CHTL::StylePropertyNode* prop2 = static_cast<CHTL::StylePropertyNode*>(template_node->body[1].get());
    ASSERT_EQ(prop2->key, "backgroundColor");
    ASSERT_EQ(prop2->value.size(), 1);
    CHTL::LiteralValueNode* val2 = static_cast<CHTL::LiteralValueNode*>(prop2->value[0].get());
    ASSERT_EQ(val2->value, "white");
}

TEST(ParserTest, ParsesStyleWithVariableUsage) {
    std::string source = "div { style { color: Theme(textColor); } }";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);

    std::unique_ptr<CHTL::ProgramNode> program = parser.parse();

    ASSERT_NE(program, nullptr);
    ASSERT_EQ(program->statements.size(), 1);

    CHTL::ElementNode* div_element = static_cast<CHTL::ElementNode*>(program->statements[0].get());
    CHTL::StyleNode* style_node = static_cast<CHTL::StyleNode*>(div_element->children[0].get());
    ASSERT_EQ(style_node->children.size(), 1);

    CHTL::StylePropertyNode* prop = static_cast<CHTL::StylePropertyNode*>(style_node->children[0].get());
    ASSERT_EQ(prop->key, "color");
    ASSERT_EQ(prop->value.size(), 1);

    CHTL::VariableUsageNode* var_usage = static_cast<CHTL::VariableUsageNode*>(prop->value[0].get());
    ASSERT_EQ(var_usage->groupName, "Theme");
    ASSERT_EQ(var_usage->variableName, "textColor");
}

TEST(ParserTest, ParsesTemplateInheritance) {
    std::string source = R"(
        [Template] @Style Child {
            @Style Parent;
            color: "red";
        }
    )";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);

    std::unique_ptr<CHTL::ProgramNode> program = parser.parse();

    ASSERT_NE(program, nullptr);
    ASSERT_EQ(program->statements.size(), 1);

    CHTL::BaseNode* stmt = program->statements[0].get();
    ASSERT_EQ(stmt->getType(), CHTL::NodeType::Template);

    CHTL::TemplateNode* template_node = static_cast<CHTL::TemplateNode*>(stmt);
    ASSERT_EQ(template_node->type, "Style");
    ASSERT_EQ(template_node->name, "Child");
    ASSERT_EQ(template_node->body.size(), 1);
    ASSERT_EQ(template_node->inheritances.size(), 1);

    CHTL::TemplateUsageNode* inheritance = template_node->inheritances[0].get();
    ASSERT_EQ(inheritance->type, "Style");
    ASSERT_EQ(inheritance->name, "Parent");
}

TEST(ParserTest, ParsesValuelessStyleGroup) {
    std::string source = R"(
        [Custom] @Style TextSet {
            color,
            font-size;
        }
    )";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);

    std::unique_ptr<CHTL::ProgramNode> program = parser.parse();

    ASSERT_NE(program, nullptr);
    ASSERT_EQ(program->statements.size(), 1);

    CHTL::BaseNode* stmt = program->statements[0].get();
    ASSERT_EQ(stmt->getType(), CHTL::NodeType::Template);

    CHTL::TemplateNode* template_node = static_cast<CHTL::TemplateNode*>(stmt);
    ASSERT_EQ(template_node->type, "Style");
    ASSERT_EQ(template_node->name, "TextSet");
    ASSERT_TRUE(template_node->isCustom);
    ASSERT_EQ(template_node->body.size(), 2);

    CHTL::StylePropertyNode* prop1 = static_cast<CHTL::StylePropertyNode*>(template_node->body[0].get());
    ASSERT_EQ(prop1->key, "color");
    ASSERT_TRUE(prop1->value.empty());

    CHTL::StylePropertyNode* prop2 = static_cast<CHTL::StylePropertyNode*>(template_node->body[1].get());
    ASSERT_EQ(prop2->key, "font-size");
    ASSERT_TRUE(prop2->value.empty());
}

TEST(ParserTest, ParsesValuelessStyleGroupUsage) {
    std::string source = R"(
        div {
            style {
                @Style TextSet {
                    color: red;
                    font-size: 16px;
                }
            }
        }
    )";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);

    std::unique_ptr<CHTL::ProgramNode> program = parser.parse();

    ASSERT_NE(program, nullptr);
    ASSERT_EQ(program->statements.size(), 1);

    CHTL::ElementNode* div_element = static_cast<CHTL::ElementNode*>(program->statements[0].get());
    CHTL::StyleNode* style_node = static_cast<CHTL::StyleNode*>(div_element->children[0].get());
    ASSERT_EQ(style_node->children.size(), 1);

    CHTL::TemplateUsageNode* usage_node = static_cast<CHTL::TemplateUsageNode*>(style_node->children[0].get());
    ASSERT_EQ(usage_node->type, "Style");
    ASSERT_EQ(usage_node->name, "TextSet");
    ASSERT_EQ(usage_node->provided_properties.size(), 2);

    CHTL::StylePropertyNode* prop1 = usage_node->provided_properties[0].get();
    ASSERT_EQ(prop1->key, "color");
    ASSERT_EQ(prop1->value.size(), 1);
    CHTL::LiteralValueNode* val1 = static_cast<CHTL::LiteralValueNode*>(prop1->value[0].get());
    ASSERT_EQ(val1->value, "red");

    CHTL::StylePropertyNode* prop2 = usage_node->provided_properties[1].get();
    ASSERT_EQ(prop2->key, "font-size");
    ASSERT_EQ(prop2->value.size(), 1);
    CHTL::LiteralValueNode* val2 = static_cast<CHTL::LiteralValueNode*>(prop2->value[0].get());
    ASSERT_EQ(val2->value, "16px");
}

TEST(ParserTest, ParsesNamedOriginBlock) {
    std::string source = R"([Origin] @Html myFile { "<div></div>" })";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);

    std::unique_ptr<CHTL::ProgramNode> program = parser.parse();

    ASSERT_NE(program, nullptr);
    ASSERT_EQ(program->statements.size(), 1);

    CHTL::BaseNode* stmt = program->statements[0].get();
    ASSERT_EQ(stmt->getType(), CHTL::NodeType::Origin);

    CHTL::OriginNode* origin_node = static_cast<CHTL::OriginNode*>(stmt);
    ASSERT_EQ(origin_node->originType, "Html");
    ASSERT_EQ(origin_node->name, "myFile");
    ASSERT_EQ(origin_node->content, "<div></div>");
}

TEST(ParserTest, ParsesUnquotedTextNode) {
    std::string source = "p { text { Hello, World! } }";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);

    std::unique_ptr<CHTL::ProgramNode> program = parser.parse();

    ASSERT_NE(program, nullptr);
    ASSERT_EQ(program->statements.size(), 1);

    CHTL::BaseNode* stmt = program->statements[0].get();
    ASSERT_EQ(stmt->getType(), CHTL::NodeType::Element);

    CHTL::ElementNode* p_element = static_cast<CHTL::ElementNode*>(stmt);
    ASSERT_EQ(p_element->tagName, "p");
    ASSERT_EQ(p_element->children.size(), 1);

    CHTL::BaseNode* child_stmt = p_element->children[0].get();
    ASSERT_EQ(child_stmt->getType(), CHTL::NodeType::Text);

    CHTL::TextNode* text_node = static_cast<CHTL::TextNode*>(child_stmt);
    ASSERT_EQ(text_node->content, "Hello, World!");
}
