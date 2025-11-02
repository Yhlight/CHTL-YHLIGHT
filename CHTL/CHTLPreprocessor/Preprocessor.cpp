#include "Preprocessor.h"

CHTLPreprocessor::CHTLPreprocessor(const std::string& source) : source(source) {}

std::string CHTLPreprocessor::preprocess() {
    return source;
}
