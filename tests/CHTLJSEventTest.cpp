#include "gtest/gtest.h"
#include "CHTLJS/TestHelper.h"
#include <string>

using namespace CHTLJS;

TEST(CHTLJSEventTest, ListenExpression) {
    std::string source = "{{#myButton}} Listen { click: \"() => console.log('clicked')\" }";
    std::string result = compileCHTLJS(source);
    std::string expected = "document.getElementById(\"myButton\").addEventListener('click', () => console.log('clicked'));";
    ASSERT_EQ(result, expected);
}

TEST(CHTLJSEventTest, ListenExpressionMultiHandler) {
    std::string source = "{{#myButton}} Listen { click: \"() => console.log('clicked')\", mouseover: \"() => console.log('hovered')\" }";
    std::string result = compileCHTLJS(source);
    std::string expected = "document.getElementById(\"myButton\").addEventListener('click', () => console.log('clicked'));\ndocument.getElementById(\"myButton\").addEventListener('mouseover', () => console.log('hovered'));";
    ASSERT_EQ(result, expected);
}

TEST(CHTLJSEventTest, DelegateExpression) {
    std::string source = "{{#myList}} Delegate { target: {{.item}}, click: \"() => console.log('item clicked')\" }";
    std::string result = compileCHTLJS(source);
    std::string expected = "document.getElementById(\"myList\").addEventListener('click', event => {\n  if (event.target.matches('.item')) {\n    (() => console.log('item clicked'))(event);\n  }\n});";
    ASSERT_EQ(result, expected);
}
