#include <gtest/gtest.h>
#include "CHTL/CHTLParser/Parser.h"
#include "CHTL/CHTLAnalyser/Analyser.h"
#include "CHTL/CHTLGenerator/Generator.h"
#include "SharedCore/SymbolTable.h"

TEST(NamespaceTest, NamespaceResolution) {
    std::string source = R"(
        [Namespace] MyNamespace {
            [Template] @Style MyTemplate {
                color: red;
            }
        }

        p {
            style {
                @Style MyNamespace::MyTemplate;
            }
        }
    )";

    CHTL::Parser parser(source);
    auto program = parser.parse();

    CHTL::SymbolTable symbolTable;
    CHTL::Analyser analyser(symbolTable);
    analyser.analyse(program.get());

    CHTL::Generator generator;
    std::string result = generator.generate(program.get());

    std::string expected = R"(<p style="color:red;"></p>)";
    ASSERT_EQ(result, expected);
}
