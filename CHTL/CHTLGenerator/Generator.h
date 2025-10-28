#pragma once

#include "CHTL/CHTLNode/BaseNode.h"
#include <string>
#include <memory>

namespace CHTL {

class Generator {
public:
    Generator(std::shared_ptr<BaseNode> root);

    std::string generate();

private:
    std::shared_ptr<BaseNode> m_root;

    void visit(std::shared_ptr<BaseNode> node, std::string& output);
};

} // namespace CHTL
