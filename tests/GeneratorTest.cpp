#include "gtest/gtest.h"
#include "Generator.h"
#include "ProgramNode.h"
#include "ElementNode.h"
#include "TextNode.h"
#include "StyleNode.h"
#include "StylePropertyNode.h"
#include "StyleRuleNode.h"
#include "ScriptNode.h"
#include "TemplateNode.h"
#include "CustomNode.h"
#include "ElementDirectiveNode.h"
#include "StyleDirectiveNode.h"
#include "OriginNode.h"
#include "OriginDirectiveNode.h"
#include "ImportNode.h"

// TEST(GeneratorTest, ImportStatement) {
//     auto root = std::make_unique<ProgramNode>();
//     root->children.push_back(std::make_unique<ImportNode>("Html", "../test.html", "myHtml"));
//     root->children.push_back(std::make_unique<ImportNode>("Style", "../test.css", "myCss"));
//     root->children.push_back(std::make_unique<ImportNode>("JavaScript", "../test.js", "myJs"));
//     root->children.push_back(std::make_unique<OriginDirectiveNode>("Html", "myHtml"));
//     root->children.push_back(std::make_unique<OriginDirectiveNode>("Style", "myCss"));
//     root->children.push_back(std::make_unique<OriginDirectiveNode>("JavaScript", "myJs"));
//     Generator generator(*root);
//     auto html = generator.generate();
//     EXPECT_EQ(html, "<html><head><style>.test { color: red; }\n</style></head><body><p>This is a test HTML file.</p>\n<script>console.log(\"This is a test JS file.\");\n</script></body></html>");
// }

TEST(GeneratorTest, OriginBlock) {
    auto root = std::make_unique<ProgramNode>();
    root->children.push_back(std::make_unique<OriginNode>("Html", "", "<p>hello</p>"));
    root->children.push_back(std::make_unique<OriginNode>("Style", "", ".red { color: red; }"));
    root->children.push_back(std::make_unique<OriginNode>("JavaScript", "", "let x = 1;"));
    Generator generator(*root);
    auto html = generator.generate();
    EXPECT_EQ(html, "<html><head><style>.red { color: red; }</style></head><body><p>hello</p><script>let x = 1;</script></body></html>");
}

TEST(GeneratorTest, NamedOriginBlock) {
    auto root = std::make_unique<ProgramNode>();
    root->children.push_back(std::make_unique<OriginNode>("Html", "myHtml", "<p>hello</p>"));
    root->children.push_back(std::make_unique<OriginDirectiveNode>("Html", "myHtml"));
    Generator generator(*root);
    auto html = generator.generate();
    EXPECT_EQ(html, "<html><head><style></style></head><body><p>hello</p><script></script></body></html>");
}

TEST(GeneratorTest, SimpleElement) {
    auto root = std::make_unique<ProgramNode>();
    auto element = std::make_unique<ElementNode>("div");
    element->children.push_back(std::make_unique<TextNode>("hello"));
    root->children.push_back(std::move(element));
    Generator generator(*root);
    auto html = generator.generate();
    EXPECT_EQ(html, "<html><head><style></style></head><body><div>hello</div><script></script></body></html>");
}

TEST(GeneratorTest, SimpleElementRaw) {
    auto root = std::make_unique<ProgramNode>();
    auto element = std::make_unique<ElementNode>("div");
    element->children.push_back(std::make_unique<TextNode>("hello"));
    root->children.push_back(std::move(element));
    Generator generator(*root);
    auto html = generator.generate(false);
    EXPECT_EQ(html, "<div>hello</div>");
}

TEST(GeneratorTest, ElementWithAttributes) {
    auto root = std::make_unique<ProgramNode>();
    auto element = std::make_unique<ElementNode>("div");
    element->attributes["id"] = "main";
    element->attributes["class"] = "container";
    root->children.push_back(std::move(element));
    Generator generator(*root);
    auto html = generator.generate();
    EXPECT_NE(html.find("id=\"main\""), std::string::npos);
    EXPECT_NE(html.find("class=\"container\""), std::string::npos);
}

TEST(GeneratorTest, ElementWithInlineStyle) {
    auto root = std::make_unique<ProgramNode>();
    auto element = std::make_unique<ElementNode>("div");
    auto style_node = std::make_unique<StyleNode>();
    style_node->children.push_back(std::make_unique<StylePropertyNode>("color", "red"));
    style_node->children.push_back(std::make_unique<StylePropertyNode>("font-size", "16px"));
    element->children.push_back(std::move(style_node));
    root->children.push_back(std::move(element));
    Generator generator(*root);
    auto html = generator.generate();
    EXPECT_EQ(html, "<html><head><style></style></head><body><div style=\"color:red;font-size:16px;\"></div><script></script></body></html>");
}

TEST(GeneratorTest, ElementWithGlobalStyle) {
    auto root = std::make_unique<ProgramNode>();
    auto element = std::make_unique<ElementNode>("div");
    auto style_node = std::make_unique<StyleNode>();
    auto rule_node = std::make_unique<StyleRuleNode>("my-class");
    rule_node->children.push_back(std::make_unique<StylePropertyNode>("color", "red"));
    style_node->children.push_back(std::move(rule_node));
    element->children.push_back(std::move(style_node));
    root->children.push_back(std::move(element));
    Generator generator(*root);
    auto html = generator.generate();
    EXPECT_EQ(html, "<html><head><style>.my-class{color:red;}</style></head><body><div class=\"my-class\"></div><script></script></body></html>");
}

TEST(GeneratorTest, ElementWithScript) {
    auto root = std::make_unique<ProgramNode>();
    auto element = std::make_unique<ElementNode>("div");
    element->children.push_back(std::make_unique<ScriptNode>("let x = 1;"));
    root->children.push_back(std::move(element));
    Generator generator(*root);
    auto html = generator.generate();
    EXPECT_EQ(html, "<html><head><style></style></head><body><div></div><script>let x = 1;</script></body></html>");
}

TEST(GeneratorTest, ElementTemplate) {
    auto root = std::make_unique<ProgramNode>();
    auto template_node = std::make_unique<TemplateNode>("MyElement", TemplateType::Element);
    auto template_element = std::make_unique<ElementNode>("div");
    template_element->children.push_back(std::make_unique<TextNode>("hello"));
    template_node->children.push_back(std::move(template_element));
    root->children.push_back(std::move(template_node));
    auto element_directive = std::make_unique<ElementDirectiveNode>("MyElement");
    root->children.push_back(std::move(element_directive));
    Generator generator(*root);
    auto html = generator.generate();
    EXPECT_EQ(html, "<html><head><style></style></head><body><div>hello</div><script></script></body></html>");
}

TEST(GeneratorTest, StyleTemplate) {
    auto root = std::make_unique<ProgramNode>();
    auto template_node = std::make_unique<TemplateNode>("MyStyle", TemplateType::Style);
    template_node->children.push_back(std::make_unique<StylePropertyNode>("color", "red"));
    root->children.push_back(std::move(template_node));
    auto element = std::make_unique<ElementNode>("div");
    auto style_node = std::make_unique<StyleNode>();
    style_node->children.push_back(std::make_unique<StyleDirectiveNode>("MyStyle"));
    element->children.push_back(std::move(style_node));
    root->children.push_back(std::move(element));
    Generator generator(*root);
    auto html = generator.generate();
    EXPECT_EQ(html, "<html><head><style></style></head><body><div style=\"color:red;\"></div><script></script></body></html>");
}

TEST(GeneratorTest, VarTemplate) {
    auto root = std::make_unique<ProgramNode>();
    auto template_node = std::make_unique<TemplateNode>("MyVars", TemplateType::Var);
    template_node->variables["color"] = "red";
    root->children.push_back(std::move(template_node));
    auto element = std::make_unique<ElementNode>("div");
    auto style_node = std::make_unique<StyleNode>();
    style_node->children.push_back(std::make_unique<StylePropertyNode>("color", "MyVars(color)"));
    element->children.push_back(std::move(style_node));
    root->children.push_back(std::move(element));
    Generator generator(*root);
    auto html = generator.generate();
    EXPECT_EQ(html, "<html><head><style></style></head><body><div style=\"color:red;\"></div><script></script></body></html>");
}

TEST(GeneratorTest, CustomStyleTemplate) {
    auto root = std::make_unique<ProgramNode>();
    auto custom_node = std::make_unique<CustomNode>("MyStyle", CustomType::Style);
    custom_node->valueless_properties.push_back("color");
    custom_node->valueless_properties.push_back("font-size");
    root->children.push_back(std::move(custom_node));
    auto element = std::make_unique<ElementNode>("div");
    auto style_node = std::make_unique<StyleNode>();
    auto directive = std::make_unique<StyleDirectiveNode>("MyStyle");
    directive->properties["color"] = "red";
    directive->properties["font-size"] = "16px";
    style_node->children.push_back(std::move(directive));
    element->children.push_back(std::move(style_node));
    root->children.push_back(std::move(element));
    Generator generator(*root);
    auto html = generator.generate();
    EXPECT_EQ(html, "<html><head><style></style></head><body><div style=\"color:red;font-size:16px;\"></div><script></script></body></html>");
}
