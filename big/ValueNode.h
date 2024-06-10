#pragma once
#include "AstNode.h"

class ValueNode : public AstNode {
private:
    CValue value;
public:

    // Constructs a value node with the given value
    ValueNode(CValue value) : AstNode(NodeType::Value), value(value) {}

    // Evaluates the value node
    CValue evaluate() const override{
        return value;
    }
};