add_library(CHTLJSNode
    EnhancedSelectorNode.cpp
    ProgramNode.cpp
    IdentifierNode.cpp
    MemberExpressionNode.cpp
    CallExpressionNode.cpp
    StringLiteralNode.cpp
)

target_include_directories(CHTLJSNode PUBLIC .)
