#include "Test.h"
#include "../CHTLJS/CHTLJSLexer/Lexer.h"
#include "../CHTLJS/CHTLJSParser/Parser.h"
#include "../CHTLJS/CHTLJSNode/EnhancedSelectorNode.h"
#include "../CHTLJS/CHTLJSNode/ProgramNode.h"
#include "../CHTLJS/CHTLJSNode/CallExpressionNode.h"
#include "../CHTLJS/CHTLJSNode/MemberExpressionNode.h"
#include "../CHTLJS/CHTLJSNode/IdentifierNode.h"
#include "../CHTLJS/CHTLJSNode/StringLiteralNode.h"
#include <cassert>

Test CHTLJSParserTest = {
    "CHTLJSParser: Parse Member Call Expression with Arguments",
    []() {
        std::string source = "{{box}}->setAttribute(\"id\", \"main-box\")";
        CHTLJSLexer lexer(source);
        std::vector<CHTLJSToken> tokens = lexer.tokenize();
        CHTLJSParser parser(tokens);
        std::unique_ptr<ProgramNode> program = parser.parse();
        assert(program != nullptr);
        assert(program->children.size() == 1);

        CallExpressionNode* callExpr = dynamic_cast<CallExpressionNode*>(program->children[0].get());
        assert(callExpr != nullptr);
        assert(callExpr->arguments.size() == 2);

        StringLiteralNode* arg1 = dynamic_cast<StringLiteralNode*>(callExpr->arguments[0].get());
        assert(arg1 != nullptr);
        assert(arg1->value == "id");

        StringLiteralNode* arg2 = dynamic_cast<StringLiteralNode*>(callExpr->arguments[1].get());
        assert(arg2 != nullptr);
        assert(arg2->value == "main-box");
    }
};

// This is a bit of a hack, but it's how the other tests are registered.
struct CHTLJSParserTestRegister {
    CHTLJSParserTestRegister() {
        tests.push_back(CHTLJSParserTest);
    }
};

CHTLJSParserTestRegister CHTLJSParserTestRegister_instance;
