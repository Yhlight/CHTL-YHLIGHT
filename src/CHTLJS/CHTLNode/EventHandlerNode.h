#pragma once

#include "ASTNode.h"
#include <string>

namespace CHTLJS {

class EventHandlerNode : public ASTNode {
public:
    std::string eventName;
    std::string handlerBody;

    EventHandlerNode(const std::string& eventName, const std::string& handlerBody)
        : eventName(eventName), handlerBody(handlerBody) {}

    ASTNodeType getType() const override { return ASTNodeType::EventHandler; }
};

} // namespace CHTLJS
