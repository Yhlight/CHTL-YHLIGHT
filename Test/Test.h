#pragma once

#include <string>
#include <vector>
#include <functional>

struct Test {
    std::string name;
    std::function<void()> test_case;
};

extern std::vector<Test> tests;
