#pragma once
#include "CValue.h"
#include <memory>

enum NodeType {
    BinaryOp, UnaryOp, RefNode, Value
};

class AstNode {
private:
    NodeType type;
public:
    // Constructs an AST node with the given type
    AstNode(NodeType type) : type(type) {}

    // Virtual destructor
    virtual ~AstNode() = default;

    // Evaluates the AST node
    virtual CValue evaluate() const = 0;
};
