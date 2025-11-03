#pragma once

#include "CHTLJS/CHTLLexer/Lexer.h"
#include "CHTLJS/CHTLParser/Parser.h"
#include "CHTLJS/CHTLAnalyser/Analyser.h"
#include "CHTLJS/CHTLGenerator/Generator.h"
#include <string>
#include <memory>

namespace CHTLJS {

inline std::string compileCHTLJS(const std::string& source) {
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();
    Parser parser(tokens);
    auto program = parser.parse();
    if (!program) {
        return "";
    }
    Analyser analyser(*program);
    analyser.analyse();
    Generator generator(*program, analyser);
    return generator.generate();
}

} // namespace CHTLJS
