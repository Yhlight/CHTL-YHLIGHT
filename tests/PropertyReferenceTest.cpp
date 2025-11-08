#include "gtest/gtest.h"
#include "CHTLLexer/Lexer.h"
#include "CHTLParser/Parser.h"
#include "CHTLGenerator/Generator.h"
#include "CHTLNode/PropertyReferenceNode.h"
#include "CHTLNode/StyleNode.h"
#include "CHTLNode/ElementNode.h"
#include "CHTLNode/StylePropertyNode.h"

TEST(PropertyReferenceTest, SimpleReference) {
    std::string source = R"(
        div {
            id: "box";
            style {
                width: 100px;
            }
        }
        p {
            style {
                width: #box.width;
            }
        }
    )";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto program = parser.parse();

    CHTL::Generator generator;
    std::string result = generator.generate(*program);

    ASSERT_EQ(result, R"(<div id="box" style="width:100px;"></div><p style="width:100px;"></p>)");
}
