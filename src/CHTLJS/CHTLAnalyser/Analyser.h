#pragma once

#include "../CHTLNode/ASTNode.h"
#include "../CHTLNode/VirNode.h"
#include "../CHTLNode/ProgramNode.h"
#include "../CHTLNode/VirUsageNode.h"
#include <string>
#include <unordered_map>
#include <memory>

namespace CHTLJS {

class Analyser {
public:
    Analyser(ASTNode& root);
    void analyse();

    const VirNode* getVir(const std::string& name) const;

private:
    void visit(ASTNode* node);
    void visitVirNode(VirNode* node);

    ASTNode& m_root;
    std::unordered_map<std::string, const VirNode*> m_virs;
};

} // namespace CHTLJS
