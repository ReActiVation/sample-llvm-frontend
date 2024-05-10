
#include <memory>

#include "globals.hpp"
#include "ast_tree.hpp"
#include "processing_utils.hpp"

namespace {
static std::unique_ptr<TExprAST> ParseNumberExpr() {
    auto result = std::make_unique<TNumberExprAST>(g_NumericValue);
    GetNextToken();
    return std::move(result);
}
}