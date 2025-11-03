#include "gtest/gtest.h"
#include "CHTLJS/TestHelper.h"
#include <string>

using namespace CHTLJS;

TEST(CHTLJSAnimateTest, AnimateExpression) {
    std::string source = "Animate { "
                         "target: {{#myDiv}}, "
                         "duration: 1000, "
                         "easing: \"ease-in-out\", "
                         "begin: { opacity: \"0\" },"
                         "when: [ { at: 0.5, opacity: \"0.5\" }, { at: 0.8, opacity: \"0.8\" } ],"
                         "end: { opacity: \"1\" },"
                         "loop: 3,"
                         "direction: \"alternate\","
                         "delay: 500,"
                         "callback: onAnimationEnd"
                         "}";
    std::string result = compileCHTLJS(source);
    std::string expected = "const anim = document.getElementById(\"myDiv\").animate([\n"
                         "  {\n"
                         "      opacity: \"0\"\n"
                         "  },\n"
                         "  {\n"
                         "      opacity: \"0.5\",\n"
                         "      offset: 0.5\n"
                         "  },\n"
                         "  {\n"
                         "      opacity: \"0.8\",\n"
                         "      offset: 0.8\n"
                         "  },\n"
                         "  {\n"
                         "      opacity: \"1\"\n"
                         "  }\n"
                         "], {\n"
                         "  duration: 1000,\n"
                         "  easing: \"ease-in-out\",\n"
                         "  iterations: 3,\n"
                         "  direction: \"alternate\",\n"
                         "  delay: 500\n"
                         "});\n"
                         "anim.onfinish = onAnimationEnd;";
    ASSERT_EQ(result, expected);
}
