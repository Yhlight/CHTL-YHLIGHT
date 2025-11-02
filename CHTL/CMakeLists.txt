add_library(CHTLLexer
    CHTLLexer/Lexer.cpp
)

target_include_directories(CHTLLexer PUBLIC ./)

add_library(CHTLParser
    CHTLParser/Parser.cpp
    CHTLNode/BaseNode.h
    CHTLNode/ElementNode.h
    CHTLNode/TextNode.h
    CHTLNode/StyleNode.h
    CHTLNode/ScriptNode.h
    CHTLNode/CommentNode.h
    CHTLNode/TemplateNode.h
    CHTLNode/CustomNode.h
    CHTLNode/OriginNode.h
    CHTLNode/ImportNode.h
    CHTLNode/NamespaceNode.h
)

target_link_libraries(CHTLParser PRIVATE CHTLLexer)
target_include_directories(CHTLParser PUBLIC ./)
