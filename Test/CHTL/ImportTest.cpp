#include "gtest/gtest.h"
#include <fstream>
#include "CHTL/Lexer.h"
#include "CHTL/CHTLParser/Parser.h"
#include "CHTL/CHTLGenerator/Generator.h"
#include "Util/FileUtil.h"

TEST(ImportTest, SimpleImport) {
    std::string imported_content = "[Template] @Style MyStyle { color: red; }";
    std::string main_content = "[Import] @Chtl from \"imported.chtl\"; div { style { @Style MyStyle; } }";

    // Create a dummy imported file
    std::ofstream("imported.chtl") << imported_content;

    CHTL::Lexer lexer(main_content);
    CHTL::Parser parser(lexer);
    auto program = parser.parse();
    CHTL::Generator generator(program, parser.getStyleTemplates(), parser.getElementTemplates(), parser.getVarTemplates());
    std::string result = generator.generate();

    ASSERT_EQ(result, "<div style=\"color:red;\"></div>");

    // Clean up the dummy file
    remove("imported.chtl");
}
