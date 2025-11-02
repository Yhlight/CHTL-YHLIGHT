#include "Test.h"
#include "../CHTLJS/CHTLJSLexer/Lexer.h"
#include "../CHTLJS/CHTLJSParser/Parser.h"
#include "../CHTLJS/CHTLJSGenerator/Generator.h"
#include <cassert>

Test CHTLJSGeneratorTest = {
    "CHTLJSGenerator: Generate Member Call Expression with Arguments",
    []() {
        std::string source = "{{box}}->setAttribute(\"id\", \"main-box\")";
        CHTLJSLexer lexer(source);
        std::vector<CHTLJSToken> tokens = lexer.tokenize();
        CHTLJSParser parser(tokens);
        std::unique_ptr<CHTLJSBaseNode> node = parser.parse();
        CHTLJSGenerator generator;
        std::string result = generator.generate(node.get());
        assert(result == "document.querySelector('box').setAttribute(\"id\", \"main-box\");\n");
    }
};

// This is a bit of a hack, but it's how the other tests are registered.
struct CHTLJSGeneratorTestRegister {
    CHTLJSGeneratorTestRegister() {
        tests.push_back(CHTLJSGeneratorTest);
    }
};

CHTLJSGeneratorTestRegister CHTLJSGeneratorTestRegister_instance;
