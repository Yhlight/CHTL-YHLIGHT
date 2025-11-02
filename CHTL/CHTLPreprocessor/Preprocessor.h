#ifndef CHTLPREPROCESSOR_H
#define CHTLPREPROCESSOR_H

#include <string>
#include "CHTLLexer/Token.h"
#include <vector>

class CHTLPreprocessor {
public:
    CHTLPreprocessor(const std::string& source);
    std::string preprocess();

private:
    std::string source;
};

#endif //CHTLPREPROCESSOR_H
