#pragma once

#include "CHTL/CHTLNode/ASTNode.h"
#include <string>
#include <memory>

namespace CHTL {

class Importer {
public:
    Importer();
    std::unique_ptr<ASTNode> importFile(const std::string& filePath, const std::string& currentPath);
};

}
