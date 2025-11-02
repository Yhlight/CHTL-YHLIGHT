#pragma once

#include "SymbolTable.h"
#include <map>
#include <string>

namespace CHTL {

class CHTLContext {
public:
    SymbolTable symbolTable;
    std::map<std::string, std::string> config;
};

} // namespace CHTL
