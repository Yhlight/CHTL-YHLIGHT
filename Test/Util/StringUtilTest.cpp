#include <gtest/gtest.h>
#include "Util/StringUtil.h"

TEST(StringUtilTest, Trim) {
    EXPECT_EQ(CHTL::Util::Trim("  hello  "), "hello");
    EXPECT_EQ(CHTL::Util::Trim("hello  "), "hello");
    EXPECT_EQ(CHTL::Util::Trim("  hello"), "hello");
    EXPECT_EQ(CHTL::Util::Trim("hello"), "hello");
    EXPECT_EQ(CHTL::Util::Trim(""), "");
    EXPECT_EQ(CHTL::Util::Trim("   "), "");
}

TEST(StringUtilTest, Split) {
    std::vector<std::string> expected1 = {"a", "b", "c"};
    EXPECT_EQ(CHTL::Util::Split("a,b,c", ','), expected1);

    std::vector<std::string> expected2 = {"a", "", "c"};
    EXPECT_EQ(CHTL::Util::Split("a,,c", ','), expected2);

    std::vector<std::string> expected3 = {"hello"};
    EXPECT_EQ(CHTL::Util::Split("hello", ','), expected3);

    std::vector<std::string> expected4 = {""};
    EXPECT_EQ(CHTL::Util::Split("", ','), expected4);
}
