#include <gtest/gtest.h>
#include "CHTL/CHTLParser/Parser.h"
#include "CHTL/CHTLAnalyser/Analyser.h"
#include "CHTL/CHTLGenerator/Generator.h"
#include "SharedCore/SymbolTable.h"

std::string compile(const std::string& source) {
    CHTL::Parser parser(source);
    auto program = parser.parse();
    CHTL::SymbolTable symbolTable;
    CHTL::Analyser analyser(symbolTable);
    analyser.analyse(program.get());
    CHTL::Generator generator;
    return generator.generate(program.get());
}

TEST(TemplateTest, StyleTemplate) {
    std::string source =
        "[Template] @Style MyTheme {\n"
        "    color: red;\n"
        "    font-size: 16px;\n"
        "}\n"
        "div {\n"
        "    style {\n"
        "        @Style MyTheme;\n"
        "    }\n"
        "}";
    std::string expected = "<div style=\"color:red;font-size:16px;\"></div>";
    EXPECT_EQ(compile(source), expected);
}

TEST(TemplateTest, ElementTemplate) {
    std::string source =
        "[Template] @Element MyElement {\n"
        "    p { text { \"hello\" } }\n"
        "    div {}\n"
        "}\n"
        "body {\n"
        "    @Element MyElement;\n"
        "}";
    std::string expected = "<body><p>hello</p><div></div></body>";
    EXPECT_EQ(compile(source), expected);
}
