#include "Test.h"
#include "../CHTLJS/CHTLJSLexer/Lexer.h"
#include "../CHTLJS/CHTLJSParser/Parser.h"
#include "../CHTLJS/CHTLJSNode/EnhancedSelectorNode.h"
#include "../CHTLJS/CHTLJSNode/ProgramNode.h"
#include <cassert>

Test CHTLJSParserTest = {
    "CHTLJSParser: Parse Enhanced Selector",
    []() {
        std::string source = "{{.box}}{{#button}}";
        CHTLJSLexer lexer(source);
        std::vector<CHTLJSToken> tokens = lexer.tokenize();
        CHTLJSParser parser(tokens);
        std::unique_ptr<ProgramNode> node = parser.parse();
        assert(node != nullptr);
        assert(node->children.size() == 2);
        EnhancedSelectorNode* selectorNode1 = dynamic_cast<EnhancedSelectorNode*>(node->children[0].get());
        assert(selectorNode1 != nullptr);
        assert(selectorNode1->selector == ".box");
        EnhancedSelectorNode* selectorNode2 = dynamic_cast<EnhancedSelectorNode*>(node->children[1].get());
        assert(selectorNode2 != nullptr);
        assert(selectorNode2->selector == "#button");
    }
};

// This is a bit of a hack, but it's how the other tests are registered.
struct CHTLJSParserTestRegister {
    CHTLJSParserTestRegister() {
        tests.push_back(CHTLJSParserTest);
    }
};

CHTLJSParserTestRegister CHTLJSParserTestRegister_instance;
