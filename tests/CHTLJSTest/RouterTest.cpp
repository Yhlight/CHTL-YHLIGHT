#include "gtest/gtest.h"
#include "CHTLJS/TestHelper.h"
#include <string>

using namespace CHTLJS;

TEST(CHTLJSRouterTest, RouterDeclaration) {
    std::string source = "Router { url: \"/\", page: {{#home}} };";
    std::string result = compileCHTLJS(source);
    std::string expected = "const routes = {\n"
                         "  '/': document.getElementById(\"home\"),\n"
                         "};\n"
                         "const router = () => {\n"
                         "  const path = window.location.hash.slice(1) || '/';\n"
                         "  const page = routes[path];\n"
                         "  if (page) {\n"
                         "    Object.values(routes).forEach(p => p.style.display = 'none');\n"
                         "    page.style.display = 'block';\n"
                         "  }\n"
                         "};\n"
                         "window.addEventListener('hashchange', router);\n"
                         "window.addEventListener('load', router);\n";
    ASSERT_EQ(result, expected);
}
