#pragma once
#include "AstNode.h"

class BinaryOpNode : public AstNode {
private:
    std::string op;
    std::shared_ptr<AstNode> left;
    std::shared_ptr<AstNode> right;
public:
    // Constructs a binary operator node with the given operator and child nodes
    BinaryOpNode(std::string op, std::shared_ptr<AstNode> left, std::shared_ptr<AstNode> right) : AstNode(NodeType::BinaryOp), op(op), left(std::move(left)), right(std::move(right)) {};

    // Evaluates the binary operator node
    CValue evaluate() const override{
        CValue l = left->evaluate();
        CValue r = right->evaluate();
        if (op == "+") return l + r;
        if (op == "-") return l - r;
        if (op == "*") return l * r;
        if (op == "/") return l / r;
        if (op == "=") return l == r;
        if (op == "!=") return l != r;
        if (op == "<") return l < r;
        if (op == "<=") return l <= r;
        if (op == ">") return l > r;
        if (op == ">=") return l >= r;
        if (op == "^") {
            if (std::holds_alternative<double>(l) && std::holds_alternative<double>(r))
                return std::pow(std::get<double>(l), std::get<double>(r));
            else return std::monostate();
        }
        return std::monostate();
    }
};