#pragma once

#include "BaseNode.h"
#include "StyleContentNode.h"
#include <string>
#include <vector>
#include <memory>

class StyleNode : public BaseNode {
public:
    std::vector<std::unique_ptr<StyleContentNode>> content;
};
