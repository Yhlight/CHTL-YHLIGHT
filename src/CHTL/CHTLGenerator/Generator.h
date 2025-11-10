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
#include "../CHTL/Configuration.h"
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
class IfNode;
class ElseNode;
class ImportNode;
class NamespaceNode;
class ConfigNode;
class UseNode;

class Generator {
public:
    Generator(const Configuration& config);
    std::string generate(const ProgramNode& program);

private:
    void visit(const BaseNode* node);
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
    void visit(const IfNode* node);
    void visit(const ElseNode* node);
    void visit(const ImportNode* node);
    void visit(const NamespaceNode* node);
    void visit(const ConfigNode* node);
    void visit(const UseNode* node);
    void collect_symbols(const BaseNode* node);
    bool evaluateCondition(const ValueNode* condition);
    void resolveStyleInheritance(const TemplateNode* node, std::map<std::string, const StylePropertyNode*>& properties, const std::set<std::string>& deletedInheritances);
    void resolveElementInheritance(const TemplateNode* node, std::vector<const BaseNode*>& body);

    Configuration config;
    std::stringstream html_output;
    std::stringstream css_output;
    std::map<std::string, std::map<std::string, const ValueNode*>> symbol_table;
    std::map<std::string, const TemplateNode*> style_templates;
    std::map<std::string, const TemplateNode*> element_templates;
    std::map<std::string, const TemplateNode*> var_templates;
    std::vector<std::string> namespace_stack;
    std::vector<std::string> inheritance_stack;
    std::vector<std::unique_ptr<StylePropertyNode>> owned_properties;
    std::vector<std::unique_ptr<BaseNode>> owned_nodes;
    std::vector<const TemplateUsageNode*> template_usage_context;
    std::map<std::string, int> element_indices;
    std::map<std::string, const OriginNode*> named_origins;
    std::map<std::string, const ConfigNode*> named_configs;
};

} // namespace CHTL
