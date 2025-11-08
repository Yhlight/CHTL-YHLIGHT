#pragma once

#include "../CHTLNode/ProgramNode.h"
#include "../CHTLNode/ElementNode.h"
#include "../CHTLNode/TextNode.h"
#include "../CHTLNode/StyleNode.h"
#include "../CHTLNode/StylePropertyNode.h"
#include "../CHTLNode/StyleRuleNode.h"
#include "../CHTLNode/ScriptNode.h"
#include "../CHTLNode/OriginNode.h"
#include "../CHTLNode/TemplateNode.h"
#include "../CHTLNode/TemplateUsageNode.h"
#include "../CHTLNode/DeleteNode.h"
#include <string>
#include <sstream>
#include <map>
#include <set>

namespace CHTL {

class TemplateNode;
class TemplateUsageNode;
class BinaryOperationNode;
class PropertyReferenceNode;
class ConditionalNode;

class Generator {
public:
    std::string generate(const ProgramNode& program);

private:
    void visit(const ProgramNode* node);
    void visit(const ElementNode* node);
    void visit(const TextNode* node);
    void visit(const StyleNode* node, ElementNode* parent);
    void visit(const ScriptNode* node);
    void visit(const OriginNode* node);
    void visit(const TemplateNode* node);
    void visit(const TemplateUsageNode* node, ElementNode* parent);
    void visit(const StylePropertyNode* node, std::stringstream& styleStream);
    void visit(const BinaryOperationNode* node, std::stringstream& styleStream);
    void visit(const PropertyReferenceNode* node, std::stringstream& styleStream);
    void visit(const ConditionalNode* node, std::stringstream& styleStream);
    void collect_symbols(const BaseNode* node);
    void resolveStyleInheritance(const TemplateNode* node, std::map<std::string, const StylePropertyNode*>& properties, const std::set<std::string>& deletedInheritances);
    void resolveElementInheritance(const TemplateNode* node, std::vector<const BaseNode*>& body);

    std::stringstream html_output;
    std::stringstream css_output;
    std::map<std::string, std::map<std::string, const ValueNode*>> symbol_table;
    std::map<std::string, const TemplateNode*> style_templates;
    std::map<std::string, const TemplateNode*> element_templates;
    std::map<std::string, const TemplateNode*> var_templates;
    std::vector<std::string> inheritance_stack;
    std::vector<std::unique_ptr<StylePropertyNode>> owned_properties;
    std::vector<const TemplateUsageNode*> template_usage_context;
    std::map<std::string, int> element_indices;
};

} // namespace CHTL
