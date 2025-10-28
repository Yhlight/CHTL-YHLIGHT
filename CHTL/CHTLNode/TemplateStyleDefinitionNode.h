#pragma once

#include "BaseNode.h"
#include "StyleBlockNode.h"
#include <string>
#include <memory>

namespace CHTL {

class TemplateStyleDefinitionNode : public BaseNode {
public:
    TemplateStyleDefinitionNode(const std::string& name, std::shared_ptr<StyleBlockNode> styleBlock)
        : m_name(name), m_styleBlock(styleBlock) {}

    NodeType getType() const override { return NodeType::TemplateStyleDefinition; }

    const std::string& getName() const { return m_name; }
    std::shared_ptr<StyleBlockNode> getStyleBlock() const { return m_styleBlock; }

private:
    std::string m_name;
    std::shared_ptr<StyleBlockNode> m_styleBlock;
};

} // namespace CHTL
