#include "gtest/gtest.h"
#include "CHTLJS/TestHelper.h"
#include <string>

using namespace CHTLJS;

TEST(CHTLJSVirTest, VirDeclarationAndUsage) {
    std::string source = "Vir myListener = Listen { click: \"() => console.log('hello')\" }; {{#myButton}} myListener;";
    std::string result = compileCHTLJS(source);
    std::string expected = "document.getElementById(\"myButton\").addEventListener('click', () => console.log('hello'));\n";
    ASSERT_EQ(result, expected);
}

TEST(CHTLJSVirTest, VirUsageWithDelegate) {
    std::string source = "Vir myDelegate = Delegate { target: {{.item}}, click: \"() => console.log('item clicked')\" }; {{#myList}} myDelegate;";
    std::string result = compileCHTLJS(source);
    std::string expected = "document.getElementById(\"myList\").addEventListener('click', event => {\n  if (event.target.matches('.item')) {\n    (() => console.log('item clicked'))(event);\n  }\n});\n";
    ASSERT_EQ(result, expected);
}

TEST(CHTLJSVirTest, VirUsageWithAnimate) {
    std::string source = "Vir myAnimation = Animate { duration: 1000, begin: { opacity: \\\"0\\\" }, end: { opacity: \\\"1\\\" } }; {{#myDiv}} myAnimation;";
    std::string result = compileCHTLJS(source);
    std::string expected = "document.getElementById(\"myDiv\").animate([{\"opacity\":\"0\"},{\"opacity\":\"1\"}],{\"duration\":1000});\n";
    ASSERT_EQ(result, expected);
}
