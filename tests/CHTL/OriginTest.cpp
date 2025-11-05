#include <gtest/gtest.h>
#include "CHTL/CHTLParser/Parser.h"
#include "CHTL/CHTLGenerator/Generator.h"
#include "CHTL/CHTLNode/OriginNode.h"

TEST(OriginTest, ParseAndGenerateOriginBlock) {
    std::string source = R"(
        [Origin] @Html MyOrigin { <div><p>hello</p></div> }
    )";

    CHTL::Parser parser(source);
    auto program = parser.parse();

    ASSERT_NE(program, nullptr);
    ASSERT_EQ(program->children.size(), 1);

    auto origin = dynamic_cast<CHTL::OriginNode*>(program->children[0].get());
    ASSERT_NE(origin, nullptr);
    EXPECT_EQ(origin->type, "Html");
    EXPECT_EQ(origin->name.value(), "MyOrigin");
    EXPECT_EQ(origin->content, " <div><p>hello</p></div> ");

    CHTL::Generator generator;
    std::string result = generator.generate(program.get());
    EXPECT_EQ(result, " <div><p>hello</p></div> ");
}

TEST(OriginTest, ParseOriginBlockWithUnbalancedBraceInString) {
    std::string source = R"(
        [Origin] @JavaScript MyScript { let message = "Hello } World"; }
    )";

    CHTL::Parser parser(source);
    auto program = parser.parse();

    ASSERT_NE(program, nullptr);
    ASSERT_EQ(program->children.size(), 1);

    auto origin = dynamic_cast<CHTL::OriginNode*>(program->children[0].get());
    ASSERT_NE(origin, nullptr);
    EXPECT_EQ(origin->type, "JavaScript");
    EXPECT_EQ(origin->name.value(), "MyScript");
    EXPECT_EQ(origin->content, " let message = \"Hello } World\"; ");

    CHTL::Generator generator;
    std::string result = generator.generate(program.get());
    EXPECT_EQ(result, " let message = \"Hello } World\"; ");
}
