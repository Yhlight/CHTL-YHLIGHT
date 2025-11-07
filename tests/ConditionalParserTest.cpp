#include "gtest/gtest.h"
#include "CHTLLexer/Lexer.h"
#include "CHTLParser/Parser.h"
#include "CHTLNode/ConditionalNode.h"
#include "CHTLNode/LiteralValueNode.h"
#include "CHTLNode/ComparisonNode.h"
#include "CHTLNode/StyleNode.h"
#include "CHTLNode/ElementNode.h"
#include "CHTLNode/StylePropertyNode.h"

TEST(ConditionalParserTest, SimpleConditional) {
    std::string source = "div { style { color: 1 > 2 ? 'red' : 'blue'; } }";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);

    auto program = parser.parse();
    auto element = static_cast<CHTL::ElementNode*>(program->statements[0].get());
    auto style = static_cast<CHTL::StyleNode*>(element->children[0].get());
    auto prop = static_cast<CHTL::StylePropertyNode*>(style->children[0].get());
    auto cond = static_cast<CHTL::ConditionalNode*>(prop->value[0].get());

    ASSERT_NE(cond, nullptr);
    ASSERT_EQ(cond->condition->getType(), CHTL::NodeType::Comparison);
    ASSERT_EQ(cond->true_branch->getType(), CHTL::NodeType::LiteralValue);
    ASSERT_EQ(cond->false_branch->getType(), CHTL::NodeType::LiteralValue);
}
