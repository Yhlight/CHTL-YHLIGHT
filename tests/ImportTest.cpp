#include "gtest/gtest.h"
#include "CHTLLexer/Lexer.h"
#include "CHTLParser/Parser.h"
#include "CHTLGenerator/Generator.h"
#include <fstream>

TEST(ImportTest, ImportHtml) {
    std::string source = R"([Import] @Html from "../../tests/test.html" as myFile;
div { @Html myFile; }
)";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto program = parser.parse();
    CHTL::Generator generator;
    std::string result = generator.generate(*program);
    ASSERT_EQ(result, "<div><h1>Hello from HTML</h1></div>");
}

TEST(ImportTest, ImportCss) {
    std::string source = R"([Import] @Style from "../../tests/test.css" as myStyles;
div { @Style myStyles; }
)";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto program = parser.parse();
    CHTL::Generator generator;
    std::string result = generator.generate(*program);
    ASSERT_EQ(result, "<style>body { color: red; }</style><div></div>");
}

TEST(ImportTest, ImportChtl) {
    std::string source = R"([Import] @Chtl from "../../tests/test.chtl" as myTemplates;
div {
    style {
        @Style MyTemplate;
    }
}
)";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto program = parser.parse();
    CHTL::Generator generator;
    std::string result = generator.generate(*program);
    ASSERT_EQ(result, R"(<div style="color:blue;"></div>)");
}

TEST(ImportTest, SelectiveImportChtl) {
    std::string source = R"([Import] [Template] @Style Template1 from "../../tests/selective.chtl";
div {
    style {
        @Style Template1;
    }
}
)";
    CHTL::Lexer lexer(source);
    CHTL::Parser parser(lexer);
    auto program = parser.parse();
    CHTL::Generator generator;
    std::string result = generator.generate(*program);
    ASSERT_EQ(result, R"(<div style="color:red;"></div>)");
}
