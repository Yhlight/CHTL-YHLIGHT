add_library(SaltBridge
    SaltBridge/SaltBridge.cpp
)

target_link_libraries(SaltBridge PRIVATE CHTLJSLexer CHTLJSParser CHTLJSGenerator CHTLJSNode)

target_include_directories(SaltBridge PUBLIC ../ ../CHTL)
