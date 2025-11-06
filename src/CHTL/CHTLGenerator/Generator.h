#pragma once

#include "../CHTLNode/ProgramNode.h"
#include "../CHTLNode/ElementNode.h"
#include "../CHTLNode/TextNode.h"
#include "../CHTLNode/StyleNode.h"
#include "../CHTLNode/StylePropertyNode.h"
#include "../CHTLNode/StyleRuleNode.h"
#include "../CHTLNode/ScriptNode.h"
#include <string>
#include <sstream>

namespace CHTL {

class Generator {
public:
    std::string generate(const ProgramNode& program);

private:
    void visit(const ProgramNode* node);
    void visit(const ElementNode* node);
    void visit(const TextNode* node);
    void visit(const StyleNode* node, ElementNode* parent);
    void visit(const ScriptNode* node);
    void visit(const StylePropertyNode* node, std::stringstream& styleStream);

    std::stringstream html_output;
    std::stringstream css_output;
};

} // namespace CHTL
