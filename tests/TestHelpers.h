#pragma once

#include "CHTL/CHTLLexer/Lexer.h"
#include "CHTL/CHTLParser/Parser.h"
#include "CHTL/CHTLGenerator/Generator.h"
#include "CHTL/CHTLAnalyser/Analyser.h"
#include <string>
#include <algorithm>

using namespace CHTL;

inline std::string removeWhitespace(std::string str) {
    str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
    str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
    str.erase(std::remove(str.begin(), str.end(), '\r'), str.end());
    str.erase(std::remove(str.begin(), str.end(), '\t'), str.end());
    return str;
}

inline std::string compile(const std::string& source, const std::string& filePath = "") {
    Lexer lexer(source);
    Parser parser(lexer.tokenize(), source);
    auto program = parser.parse();
    Analyser analyser(*program, filePath);
    analyser.analyse();
    Generator generator(*program);
    return generator.generate();
}

inline void expect_compile_error(const std::string& source, const std::string& expected_error) {
    try {
        compile(source);
        FAIL() << "Expected a compile error, but none was thrown.";
    } catch (const std::runtime_error& e) {
        EXPECT_STREQ(e.what(), expected_error.c_str());
    }
}
