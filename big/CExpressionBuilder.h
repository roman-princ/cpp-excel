#pragma once
#include "expression.h"
#include "CSpreadsheet.h"
#include "UnaryOpNode.h"
#include "BinaryOpNode.h"
#include "ReferenceNode.h"
#include "ValueNode.h"
#include <stack>

// Pre-declare CSpreadsheet
class CSpreadsheet;

// Represents a builder for constructing an expression tree
class CExpressionBuilder : public CExprBuilder {
private:
    std::stack<std::shared_ptr<AstNode>> m_expressionStack;
    CSpreadsheet *m_spreadsheet;
    CCell *currentCell;
public:
    CExpressionBuilder() = default;

    // Constructs an expression builder with the given spreadsheet and cell
    CExpressionBuilder(CSpreadsheet &sheet, CCell &cell){
        m_spreadsheet = &sheet;
        currentCell = &cell;
    }

    // Additive operator
    void opAdd() override{
        addNodeBinary("+");
    }

    // Subtraction operator
    void opSub() override{
        addNodeBinary("-");
    }

    // Multiplication operator
    void opMul() override{
        addNodeBinary("*");
    }

    // Division operator
    void opDiv() override{
        addNodeBinary("/");
    }

    // Exponentiation operator
    void opPow() override{
        addNodeBinary("^");
    }

    // Unary negation operator
    void opNeg() override{
        addNodeUnary('-');
    }

    // Equality operator
    void opEq() override{
        addNodeBinary("=");
    }

    // Inequality operator
    void opNe() override{
        addNodeBinary("!=");
    }

    // Less than operator
    void opLt() override{
        addNodeBinary("<");
    }

    // Less than or equal operator
    void opLe() override{
        addNodeBinary("<=");
    }

    // Greater than operator
    void opGt() override{
        addNodeBinary(">");
    }

    // Greater than or equal operator
    void opGe() override{
        addNodeBinary(">=");
    }

    // Add a number value
    void valNumber(double val) override{
        addValueNode(val);
    }

    // Add a string value
    void valString(std::string val) override{
        addValueNode(val);
    }

    // Add a reference value
    void valReference(std::string val) override{
        addReferenceNode(CPos(val));
    }

    void valRange(std::string val) override{
        throw std::runtime_error("Ranges are not supported");
    }

    void funcCall(std::string fnName,
                  int paramCount) override{
        throw std::runtime_error("Function calls are not supported");
    }

    // Returns the root of the expression tree
    std::shared_ptr<AstNode> getRoot() {
        return m_expressionStack.top();
    }

    // Adds a binary node to the expression stack
    void addNodeBinary(std::string op){
        auto rhs = m_expressionStack.top();
        m_expressionStack.pop();
        auto lhs = m_expressionStack.top();
        m_expressionStack.pop();
        auto node = BinaryOpNode(op, lhs, rhs);
        m_expressionStack.push(std::make_shared<BinaryOpNode>(node));
    }

    // Adds a unary node to the expression stack
    void addNodeUnary(char op){
        auto child = m_expressionStack.top();
        m_expressionStack.pop();
        auto node = UnaryOpNode(op, child);
        m_expressionStack.push(std::make_shared<UnaryOpNode>(node));
    }

    // Adds a reference node to the expression stack
    void addReferenceNode(CPos pos){
        auto offset = currentCell->getOffset();
        int row = pos.isAbsolute().first ? pos.getPosition().first : pos.getPosition().first + offset.first;
        int col = pos.isAbsolute().second ? pos.getPosition().second : pos.getPosition().second + offset.second;
        CPos shiftedPos = CPos(row, col, pos.isAbsolute().first, pos.isAbsolute().second);
        auto node = std::make_shared<ReferenceNode>(ReferenceNode(m_spreadsheet, shiftedPos));
        m_expressionStack.push(node);
    }

    // Adds a value node to the expression stack
    void addValueNode(double val){
        auto node = std::make_shared<ValueNode>(ValueNode(val));
        m_expressionStack.push(node);
    }

    // Adds a value node to the expression stack
    void addValueNode(std::string val){
        auto node = std::make_shared<ValueNode>(ValueNode(val));
        m_expressionStack.push(node);
    }
};

