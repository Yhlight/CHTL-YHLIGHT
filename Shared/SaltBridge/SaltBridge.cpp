#include "SaltBridge.h"
#include "CHTLJS/CHTLJSLexer/Lexer.h"
#include "CHTLJS/CHTLJSParser/Parser.h"
#include "CHTLJS/CHTLJSGenerator/Generator.h"

std::string SaltBridge::compileScript(const std::string& script) {
    if (script.find("{{") != std::string::npos || script.find("->") != std::string::npos) {
        CHTLJSLexer lexer(script);
        auto tokens = lexer.tokenize();
        CHTLJSParser parser(tokens);
        auto ast = parser.parse();
        CHTLJSGenerator generator;
        return generator.generate(ast.get());
    } else {
        return script;
    }
}
