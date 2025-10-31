#include <gtest/gtest.h>
#include "CHTL/CHTLGenerator/Generator.h"
#include "CHTL/CHTLNode/ElementNode.h"
#include "CHTL/CHTLNode/TextNode.h"
#include "CHTL/CHTLNode/AttributeNode.h"
#include "CHTL/Lexer.h"
#include "CHTL/CHTLParser/Parser.h"

TEST(GeneratorTest, EndToEndElementTemplate) {
    std::string source = R"(
        [Template] @Element MyElement { span { text: "hello"; } }
        div {
            @Element MyElement;
        }
    )";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto root = parser.parse();

    CHTL::Generator generator(root, parser.getStyleTemplates(), parser.getElementTemplates(), parser.getVarTemplates());
    std::string result = generator.generate();

    std::string expected = "<html><head></head><body><div><span>hello</span></div></body></html>";
    EXPECT_EQ(result, expected);
}

TEST(GeneratorTest, EndToEndStyleTemplate) {
    std::string source = R"(
        [Template] @Style MyStyles { color: red; }
        div {
            style {
                @Style MyStyles;
                font-size: 16px;
            }
        }
    )";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto root = parser.parse();

    CHTL::Generator generator(root, parser.getStyleTemplates(), parser.getElementTemplates(), parser.getVarTemplates());
    std::string result = generator.generate();

    std::string expected = "<html><head></head><body><div style=\"color:red;font-size:16px;\"></div></body></html>";
    EXPECT_EQ(result, expected);
}

TEST(GeneratorTest, EndToEndVarTemplate) {
    std::string source = R"(
        [Template] @Var MyVars {
            brandColor: "#ff6347";
        }
        div {
            style {
                color: MyVars(brandColor);
            }
        }
    )";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto root = parser.parse();

    CHTL::Generator generator(root, parser.getStyleTemplates(), parser.getElementTemplates(), parser.getVarTemplates());
    std::string result = generator.generate();

    std::string expected = "<html><head></head><body><div style=\"color:#ff6347;\"></div></body></html>";
    EXPECT_EQ(result, expected);
}

TEST(GeneratorTest, EndToEndStyleTemplateInheritance) {
    std::string source = R"(
        [Template] @Style BaseStyle {
            color: red;
            font-size: 16px;
        }

        [Template] @Style ChildStyle {
            @Style BaseStyle;
            color: blue; // Override
            border: "1px solid black"; // Add new
        }

        div {
            style {
                @Style ChildStyle;
            }
        }
    )";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto root = parser.parse();

    CHTL::Generator generator(root, parser.getStyleTemplates(), parser.getElementTemplates(), parser.getVarTemplates());
    std::string result = generator.generate();

    // A more robust test checks for the presence of correct properties, ignoring order.
    EXPECT_NE(result.find("border:1px solid black;"), std::string::npos);
    EXPECT_NE(result.find("color:blue;"), std::string::npos);
    EXPECT_NE(result.find("font-size:16px;"), std::string::npos);
    EXPECT_EQ(result.find("color:red;"), std::string::npos); // Ensure override happened
    EXPECT_NE(result.find("<html><head></head><body>"), std::string::npos);
}

TEST(GeneratorTest, EndToEndStyleTemplateExplicitInheritance) {
    std::string source = R"(
        [Template] @Style BaseStyle {
            color: red;
            font-size: 16px;
        }

        [Template] @Style ChildStyle {
            inherit @Style BaseStyle;
            color: blue; // Override
            border: "1px solid black"; // Add new
        }

        div {
            style {
                @Style ChildStyle;
            }
        }
    )";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto root = parser.parse();

    CHTL::Generator generator(root, parser.getStyleTemplates(), parser.getElementTemplates(), parser.getVarTemplates());
    std::string result = generator.generate();

    // Assertions are the same as the implicit inheritance test
    EXPECT_NE(result.find("border:1px solid black;"), std::string::npos);
    EXPECT_NE(result.find("color:blue;"), std::string::npos);
    EXPECT_NE(result.find("font-size:16px;"), std::string::npos);
    EXPECT_EQ(result.find("color:red;"), std::string::npos);
    EXPECT_NE(result.find("<html><head></head><body>"), std::string::npos);
}

TEST(GeneratorTest, EndToEndElementTemplateInheritance) {
    std::string source = R"(
        [Template] @Element BaseElement {
            p { text: "from base"; }
        }

        [Template] @Element ChildElement {
            @Element BaseElement;
            span { text: "from child"; }
        }

        div {
            @Element ChildElement;
        }
    )";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto root = parser.parse();

    CHTL::Generator generator(root, parser.getStyleTemplates(), parser.getElementTemplates(), parser.getVarTemplates());
    std::string result = generator.generate();

    std::string expected = "<html><head></head><body><div><p>from base</p><span>from child</span></div></body></html>";
    EXPECT_EQ(result, expected);
}

TEST(GeneratorTest, EndToEndOriginBlock) {
    std::string rawHtml = "<script>alert('raw html');</script>";
    std::string source = "[Origin] @Html {" + rawHtml + "}";

    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto root = parser.parse();

    CHTL::Generator generator(root, parser.getStyleTemplates(), parser.getElementTemplates(), parser.getVarTemplates());
    std::string result = generator.generate();

    std::string expected = "<html><head></head><body><script>alert('raw html');</script></body></html>";
    EXPECT_EQ(result, expected);
}


TEST(GeneratorTest, GenerateHTMLWithAttribute) {
    // Manually construct an AST: div { id: "box" }
    auto attribute = std::make_shared<CHTL::AttributeNode>("id", "box");
    auto divNode = std::make_shared<CHTL::ElementNode>("div");
    divNode->addAttribute(attribute);

    std::map<std::string, std::shared_ptr<CHTL::TemplateStyleDefinitionNode>> emptyStyleMap;
    std::map<std::string, std::shared_ptr<CHTL::TemplateElementDefinitionNode>> emptyElementMap;
    std::map<std::string, std::shared_ptr<CHTL::TemplateVarDefinitionNode>> emptyVarMap;
    CHTL::Generator generator(divNode, emptyStyleMap, emptyElementMap, emptyVarMap);
    std::string result = generator.generate();

    std::string expected = "<html><head></head><body><div id=\"box\"></div></body></html>";
    EXPECT_EQ(result, expected);
}

TEST(GeneratorTest, GenerateSimpleHTML) {
    // Manually construct an AST: div { span { text { "hello" } } }
    auto textNode = std::make_shared<CHTL::TextNode>("hello");
    auto spanNode = std::make_shared<CHTL::ElementNode>("span");
    spanNode->addChild(textNode);
    auto divNode = std::make_shared<CHTL::ElementNode>("div");
    divNode->addChild(spanNode);

    std::map<std::string, std::shared_ptr<CHTL::TemplateStyleDefinitionNode>> emptyStyleMap;
    std::map<std::string, std::shared_ptr<CHTL::TemplateElementDefinitionNode>> emptyElementMap;
    std::map<std::string, std::shared_ptr<CHTL::TemplateVarDefinitionNode>> emptyVarMap;
    CHTL::Generator generator(divNode, emptyStyleMap, emptyElementMap, emptyVarMap);
    std::string result = generator.generate();

    std::string expected = "<html><head></head><body><div><span>hello</span></div></body></html>";
    EXPECT_EQ(result, expected);
}

TEST(GeneratorTest, EndToEndStyleRule) {
    std::string source = R"(
        div {
            style {
                .my-class {
                    color: red;
                }
            }
        }
    )";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto root = parser.parse();

    CHTL::Generator generator(root, parser.getStyleTemplates(), parser.getElementTemplates(), parser.getVarTemplates());
    std::string result = generator.generate();

    std::string expected = "<html><head><style>.my-class{color:red;}</style></head><body><div class=\"my-class\"></div></body></html>";
    EXPECT_EQ(result, expected);
}
