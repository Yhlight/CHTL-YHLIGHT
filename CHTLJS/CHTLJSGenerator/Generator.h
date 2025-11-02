#pragma once

#include "../CHTLJSNode/BaseNode.h"
#include <string>
#include <memory>

class CHTLJSGenerator {
public:
    CHTLJSGenerator();
    std::string generate(CHTLJSBaseNode* node);
};
