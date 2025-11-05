#pragma once

#include "BaseNode.h"

enum class StyleContentType {
    Property,
    Rule,
};

class StyleContentNode : public BaseNode {
public:
    virtual StyleContentType getStyleContentType() const = 0;
};
