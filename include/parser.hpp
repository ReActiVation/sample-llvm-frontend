#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "logger.hpp"
#include "globals.hpp"
#include "ast_tree.hpp"
#include "processing_utils.hpp"

namespace {
static std::unique_ptr<TExprAST> ParseExpression();

static std::unique_ptr<TExprAST> ParseNumberExpr() {
    auto result = std::make_unique<TNumberExprAST>(g_NumericValue);
    GetNextToken();
    return std::move(result);
}

static std::unique_ptr<TExprAST> ParseParenthesisExpr() {
    GetNextToken();
    auto v = ParseExpression();

    if (!v)
        return nullptr;

    if (g_CurrentToken)
        return LogError("expected ')'");

    GetNextToken();
    return v;
}

static std::unique_ptr<TExprAST> ParseIdentifierExpr() {
    std::string idName = g_IdentifierString;

    GetNextToken();

    if (g_CurrentToken != '(')
        return std::make_unique<TVariableExprAST>(idName);

    GetNextToken();
    std::vector<std::unique_ptr<TExprAST>> args;
    if (g_CurrentToken != ')') {
        while (true) {
            if (auto arg = ParseExpression())
                args.push_back(std::move(arg));
            else
                return nullptr;

            if (g_CurrentToken == ')')
                break;

            if (g_CurrentToken != ',')
                return LogError("Expected ')' or ',' in argument list");
            GetNextToken();
        }
    }

    GetNextToken();

    return std::make_unique<TCallExprAST>(idName, std::move(args));
}

static std::unique_ptr<TExprAST> ParsePrimary() {
    switch (g_CurrentToken) {
        default:
            return LogError("Unknown token when expecting an expression");
        case T_IDENTIFIER:
            return ParseIdentifierExpr();
        case T_NUMBER:
            return ParseNumberExpr();
        case '(':
            return ParseParenthesisExpr();
    }
}

static std::unique_ptr<TExprAST> ParseBinaryOpRHS(int exprPrecedence, std::unique_ptr<TExprAST> lhs) {
    while (true) {
        int tokenPrecedence = GetTokenPrecedence();

        if (tokenPrecedence < exprPrecedence)
            return lhs;

        int binaryOp = g_CurrentToken;
        GetNextToken();

        auto rhs = ParsePrimary();

        if (!rhs)
            return nullptr;

        int nextPrecedence = GetTokenPrecedence();

        if (tokenPrecedence < nextPrecedence) {
            rhs = ParseBinaryOpRHS(tokenPrecedence + 1, std::move(rhs));

            if (!rhs)
                return nullptr;
        }

        lhs = std::make_unique<TBinaryExprAST>(binaryOp, std::move(lhs), std::move(rhs));
    }
}

static std::unique_ptr<TExprAST> ParseExpression() {
    auto lhs = ParsePrimary();

    if (!lhs)
        return nullptr;

    return ParseBinaryOpRHS(0, std::move(lhs));
}

static std::unique_ptr<TPrototypeAST> ParsePrototype() {
    if (g_CurrentToken != T_IDENTIFIER)
        return LogErrorP("Expected function name in prototype");

    std::string functionName = g_IdentifierString;
    GetNextToken();

    if (g_CurrentToken != '(')
        return LogErrorP("Expected '(' in prototype");

    std::vector<std::string> argNames;
    while (GetNextToken() == T_IDENTIFIER)
        argNames.push_back(g_IdentifierString);

    if (g_CurrentToken != ')')
        return LogErrorP("Expected ')' in prototype");

    GetNextToken();

    return std::make_unique<TPrototypeAST>(functionName, std::move(argNames));
}

static std::unique_ptr<TFunctionAST> ParseDefinition() {
    GetNextToken();

    auto prototype = ParsePrototype();

    if (!prototype)
        return nullptr;

    if (auto expr = ParseExpression())
        return std::make_unique<TFunctionAST>(std::move(prototype), std::move(expr));

    return nullptr;
}

static std::unique_ptr<TPrototypeAST> ParseExtern() {
    GetNextToken();
    return ParsePrototype();
}

static std::unique_ptr<TFunctionAST> ParseTopLevelExpression() {
    if (auto expr = ParseExpression()) {
        auto prototype = std::make_unique<TPrototypeAST>("", std::vector<std::string>());
        return std::make_unique<TFunctionAST>(std::move(prototype), std::move(expr));
    }

    return nullptr;
}
}