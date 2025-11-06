#pragma once

#include "ExprNode.h"
#include <string>
#include <vector>

namespace CHTLJS {

enum class SelectorComponentType {
    TAG,
    ID,
    CLASS,
    DESCENDANT,
    INDEX
};

struct SelectorComponent {
    SelectorComponentType type;
    std::string value;
};

class SelectorExprNode : public ExprNode {
public:
    explicit SelectorExprNode(std::vector<SelectorComponent> components);

    const std::vector<SelectorComponent>& getComponents() const;

private:
    std::vector<SelectorComponent> components_;
};

}
