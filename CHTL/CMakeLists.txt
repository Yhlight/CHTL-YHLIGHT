add_library(CHTLLexer
    CHTLLexer/Lexer.cpp
)

target_include_directories(CHTLLexer PUBLIC ./)

add_library(CHTLParser
    CHTLParser/Parser.cpp
    CHTLParser/StyleParser.cpp
    CHTLNode/BaseNode.h
    CHTLNode/ElementNode.h
    CHTLNode/TextNode.h
    CHTLNode/StyleNode.h
    CHTLNode/ScriptNode.h
    CHTLNode/CommentNode.h
    CHTLNode/TemplateNode.h
    CHTLNode/StyleContentNode.h
    CHTLParser/StyleParser.h
    CHTLNode/ElementDirectiveNode.h
    CHTLNode/CustomNode.h
    CHTLNode/OriginNode.h
    CHTLNode/ImportNode.h
    CHTLNode/NamespaceNode.h
    CHTLNode/OriginDirectiveNode.h
    ../Util/FileUtil.h
)

target_link_libraries(CHTLParser PRIVATE CHTLLexer)
target_include_directories(CHTLParser PUBLIC ./)

add_library(CHTLGenerator
    CHTLGenerator/Generator.cpp
)

target_link_libraries(CHTLGenerator PRIVATE CHTLParser)
target_include_directories(CHTLGenerator PUBLIC ./ ../)
