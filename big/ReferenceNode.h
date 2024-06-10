#pragma once
#include "AstNode.h"
#include "CSpreadsheet.h"

// Pre-declare CSpreadsheet
class CSpreadsheet;

// Represents a reference to a cell in a spreadsheet
class ReferenceNode : public AstNode {
private:
    CPos pos;
    CSpreadsheet *spreadsheet;
public:
    // Constructs a reference node with the given position and spreadsheet
    ReferenceNode(CSpreadsheet *spreadsheet, CPos pos) : AstNode(NodeType::RefNode), pos(pos), spreadsheet(spreadsheet) {}

    // Evaluates the reference node
    CValue evaluate() const override;
};