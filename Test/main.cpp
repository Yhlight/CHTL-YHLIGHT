#include "Test.h"
#include <iostream>

std::vector<Test> tests;

void run_tests() {
    int passed = 0;
    for (const auto& test : tests) {
        try {
            test.test_case();
            std::cout << "[PASSED] " << test.name << std::endl;
            passed++;
        } catch (const std::exception& e) {
            std::cout << "[FAILED] " << test.name << " - " << e.what() << std::endl;
        }
    }
    std::cout << "\n" << passed << "/" << tests.size() << " tests passed." << std::endl;
    if (passed != tests.size()) {
        exit(1);
    }
}

void LexerTests();
void ParserTests();

int main() {
    LexerTests();
    ParserTests();
    run_tests();
    return 0;
}
