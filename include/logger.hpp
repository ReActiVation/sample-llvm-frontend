#include <cstdio>
#include <memory>

#include "ast_tree.hpp"

namespace {

std::unique_ptr<TExprAST> LogError(const char* errorString) {
    fprintf(stderr, "Error: %s\n", errorString);
    return nullptr;
}

std::unique_ptr<TPrototypeAST> LogErrorP(const char* errorString) {
    LogError(errorString);
    return nullptr;
}

}