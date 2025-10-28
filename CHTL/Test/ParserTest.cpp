#include "../CHTLParser/Parser.h"
#include "../CHTLLexer/Lexer.h"
#include <iostream>
#include <cassert>

void testParsingOfTextNode() {
    std::string source = "text { \"Hello, CHTL!\" }";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();
    Parser parser(tokens);
    std::unique_ptr<ASTNode> node = parser.parse();

    assert(node != nullptr);
    TextNode* textNode = dynamic_cast<TextNode*>(node.get());
    assert(textNode != nullptr);
    assert(textNode->text == "Hello, CHTL!");
}

int main() {
    testParsingOfTextNode();
    std::cout << "All parser tests passed!" << std::endl;
    return 0;
}
