#include <gtest/gtest.h>
#include "CHTL/CHTLParser/Parser.h"
#include "CHTL/CHTLAnalyser/Analyser.h"
#include "CHTL/CHTLGenerator/Generator.h"
#include "SharedCore/SymbolTable.h"
#include <fstream>
#include <iostream>

static std::string compile(const std::string& source, CHTL::SymbolTable& symbolTable) {
    CHTL::Parser parser(source);
    auto program = parser.parse();

    CHTL::Analyser analyser(symbolTable);
    analyser.analyse(program.get());

    CHTL::Generator generator;
    return generator.generate(program.get());
}

TEST(ImportTest, ImportStyleTemplate) {
    std::ofstream templateFile("template.chtl");
    templateFile << R"(
        [Template] @Style MyStyle {
            color: red;
        }
    )";
    templateFile.close();

    std::string source = R"(
        [Import] @Chtl from "template.chtl";
        div {
            style {
                @Style MyStyle;
            }
        }
    )";

    CHTL::SymbolTable symbolTable;
    std::string expected = R"(<div style="color:red;"></div>)";
    ASSERT_EQ(compile(source, symbolTable), expected);

    std::remove("template.chtl");
}
