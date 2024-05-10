#pragma once

#include <memory>
#include <string>
#include <vector>

namespace {
class TExprAST {
public:
    virtual ~TExprAST() = default;
};

class TNumberExprAST : public TExprAST {
    double Value_;

public:
    TNumberExprAST(double value)
        : Value_(value) {}
};

class TVariableExprAST : public TExprAST {
    std::string VariableName_;

public:
    TVariableExprAST(std::string name) 
        : VariableName_(name) {}
};

class TBinaryExprAST : public TExprAST {
    char Op_;
    std::unique_ptr<TExprAST> LHS_, RHS_;

public:
    TBinaryExprAST(char op, std::unique_ptr<TExprAST> lhs, std::unique_ptr<TExprAST> rhs)
        : Op_(op),
        LHS_(std::move(lhs)),
        RHS_(std::move(rhs)) {}
};

class TCallExprAST : public TExprAST {
    std::string Callee_;
    std::vector<std::unique_ptr<TExprAST>> Args_;

public:
    TCallExprAST (const std::string &callee, std::vector<std::unique_ptr<TExprAST>> args)
        : Callee_(callee),
        Args_(std::move(args)) {}
};

class TPrototypeAST {
    std::string Name_;
    std::vector<std::string> Args_;

public:
    TPrototypeAST(const std::string &name, std::vector<std::string> args)
        : Name_(name),
        Args_(args) {}
};

class TFunctionAST {
    std::unique_ptr<TPrototypeAST> Proto_;
    std::unique_ptr<TExprAST> Body_;

public:
    TFunctionAST(std::unique_ptr<TPrototypeAST> proto, std::unique_ptr<TExprAST> body)
        : Proto_(std::move(proto)),
        Body_(std::move(body)) {}
};
}
