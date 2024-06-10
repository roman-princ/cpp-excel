#pragma once
#include "AstNode.h"

class UnaryOpNode : public AstNode {
private:
    char op;
    std::shared_ptr<AstNode> child;
public:

    // Constructs a unary operator node with the given operator and child node
    UnaryOpNode(char op, std::shared_ptr<AstNode> child) : AstNode(NodeType::UnaryOp), op(op),
                                                           child(std::move(child)) {}
    // Evaluates the unary operator node
    CValue evaluate() const override {
        CValue c = child->evaluate();
        if (std::holds_alternative<double>(c)) {
            switch (op) {
                case '-':
                    return -std::get<double>(c);
                default:
                    return std::monostate();
            }
        } else {
            return std::monostate();
        }
    }
};