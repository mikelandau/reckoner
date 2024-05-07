#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include <emscripten/emscripten.h>

#include "Operation.h"

std::vector<Operation> OPERATIONS;

int main() 
{
    OPERATIONS.push_back(Operation(std::regex("(.+)\\-(.+)"), [](double a, double b) -> double { return a - b; }));
    OPERATIONS.push_back(Operation(std::regex("(.+)\\+(.+)"), [](double a, double b) -> double { return a + b; }));
    OPERATIONS.push_back(Operation(std::regex("(.+)\\/(.+)"), [](double a, double b) -> double { return a / b; }));
    OPERATIONS.push_back(Operation(std::regex("(.+)\\*(.+)"), [](double a, double b) -> double { return a * b; }));
    std::cout << "Ready to Reckon!" << std::endl;
    return 0;
}

double calculateTerm(std::string term) {
    std::smatch match;

    for (auto &operation : OPERATIONS)
    {
        auto ex = operation.getExpression();
        std::regex_search (term, match, operation.getExpression());
        if (match.size() >= 3)
        {
            return operation.execute(calculateTerm(match[1]), calculateTerm(match[2]));
        }
    }

    // if no operations, assume it's a number literal
    return stod(term);
}

extern "C" EMSCRIPTEN_KEEPALIVE double reckon(const char* cstr_input) 
{
    return calculateTerm(cstr_input);
}
