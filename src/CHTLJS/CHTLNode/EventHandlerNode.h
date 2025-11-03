#pragma once

#include "ASTNode.h"
#include <string>
#include <memory>

namespace CHTLJS {

class EventHandlerNode : public ASTNode {
public:
    std::string eventName;
    std::string handlerBody;

    EventHandlerNode(const std::string& eventName, const std::string& handlerBody)
        : eventName(eventName), handlerBody(handlerBody) {}

    EventHandlerNode(const EventHandlerNode& other)
        : eventName(other.eventName), handlerBody(other.handlerBody) {}

    ASTNodeType getType() const override { return ASTNodeType::EventHandler; }

    std::unique_ptr<ASTNode> clone() const override {
        return std::make_unique<EventHandlerNode>(*this);
    }
};

} // namespace CHTLJS
