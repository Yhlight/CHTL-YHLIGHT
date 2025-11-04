#include "gtest/gtest.h"
#include "CHTLJS/CHTLLexer/Lexer.h"
#include "CHTLJS/CHTLParser/Parser.h"
#include "CHTLJS/CHTLGenerator/Generator.h"
#include "CHTLJS/CHTLAnalyser/Analyser.h"

TEST(CHTLJSMemberTest, MemberAccess) {
    std::string source = "{{obj}}->property;";
    CHTLJS::Lexer lexer(source);
    std::vector<CHTLJS::Token> tokens = lexer.scanTokens();
    CHTLJS::Parser parser(tokens);
    std::unique_ptr<CHTLJS::ProgramNode> ast = parser.parse();
    CHTLJS::Analyser analyser(*ast);
    analyser.analyse();
    CHTLJS::Generator generator(*ast, analyser);
    std::string result = generator.generate();
    ASSERT_EQ(result, "document.querySelectorAll(\"obj\")[0].property;\n");
}
