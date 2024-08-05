#include "dns_wildcard.h"
#include <iostream>
#include <vector>
#include <string>
#include <exception>

void runTests() {
    DNSWildcard dnsWildcard;
    dnsWildcard.addRecord("*.bbb.cc.d");
    dnsWildcard.addRecord("aaaa.bbb.cc.*");
    dnsWildcard.addRecord("test.*.example.com");
    dnsWildcard.addRecord("*.sub.example.com");
    dnsWildcard.addRecord("*.xyz.com");
    dnsWildcard.addRecord("*.multi.level.example.com");
    dnsWildcard.addRecord("sub.*.multi.level.example.com");
    dnsWildcard.addRecord("*.*.complex.example.com");

    std::vector<std::pair<std::string, bool>> testCases = {
        {"q.w.e.r.t.y.bbb.cc.d", true},
        {"aaaa.bbb.cc.d", true},
        {"any.sub.example.com", true},
        {"test.123.example.com", true},
        {"example.com", false},
        {"xyz", false},
        {"a.xyz.com", true},
        {"b.sub.example.com", true},
        {"random.test.sub.example.com", true},
        {"random.multi.level.example.com", true},
        {"sub.random.multi.level.example.com", true},
        {"any.any.complex.example.com", true},
        {"not.matching.com", false},
        {"sub.level.example.com", false}  
    };

    int passed = 0;
    for (const auto& testCase : testCases) {
        try {
            bool result = dnsWildcard.isValid(testCase.first);
            if (result == testCase.second) {
                passed++;
                std::cout << "Test passed for query \"" << testCase.first << "\"" << std::endl;
            } else {
                std::cout << "Test failed for query \"" << testCase.first << "\"" << std::endl;
            }
        } catch (const std::exception& e) {
            std::cout << "Test failed for query \"" << testCase.first << "\" with exception: " << e.what() << std::endl;
        }
    }

    std::cout << "Passed " << passed << " out of " << testCases.size() << " tests." << std::endl;

    std::vector<std::string> invalidInputs = {
        "",
        ".",
        "com.",
        "example..com",
        "*"
    };

    for (const auto& input : invalidInputs) {
        try {
            dnsWildcard.addRecord(input);
            std::cout << "Test failed for invalid input \"" << input << "\"" << std::endl;
        } catch (const std::exception& e) {
            std::cout << "Correctly caught exception for input \"" << input << "\": " << e.what() << std::endl;
        }
    }
}

int main() {
    runTests();
    return 0;
}
