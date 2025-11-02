#include "Test.h"
#include "../CHTLJS/CHTLJSLexer/Lexer.h"
#include "../CHTLJS/CHTLJSParser/Parser.h"
#include "../CHTLJS/CHTLJSGenerator/Generator.h"
#include <cassert>

Test CHTLJSGeneratorTest = {
    "CHTLJSGenerator: Generate Enhanced Selector",
    []() {
        std::string source = "{{.box}}{{#button}}";
        CHTLJSLexer lexer(source);
        std::vector<CHTLJSToken> tokens = lexer.tokenize();
        CHTLJSParser parser(tokens);
        std::unique_ptr<CHTLJSBaseNode> node = parser.parse();
        CHTLJSGenerator generator;
        std::string result = generator.generate(node);
        assert(result == "document.querySelector('.box');\ndocument.querySelector('#button');\n");
    }
};

// This is a bit of a hack, but it's how the other tests are registered.
struct CHTLJSGeneratorTestRegister {
    CHTLJSGeneratorTestRegister() {
        tests.push_back(CHTLJSGeneratorTest);
    }
};

CHTLJSGeneratorTestRegister CHTLJSGeneratorTestRegister_instance;
