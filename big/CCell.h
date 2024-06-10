#pragma once
#include "AstNode.h"

class CCell {
private:
    CValue raw = CValue();
    CValue final = CValue();
    bool isExpression = false;
    int row_offset = 0;
    int col_offset = 0;
    std::shared_ptr<AstNode> m_astRoot;
public:
    CCell() = default;

    // Constructs a cell with the given content
    CCell(std::string content){
        try {
            double number = std::stod(content);
            raw = number;
            final = number;
            isExpression = false;
            col_offset = 0;
            row_offset = 0;
        } catch (std::invalid_argument &e) {
            raw = content;
            final = content;
            isExpression = content[0] == '=';
        }
    }

    // Returns the final value of the cell
    CValue getFinal() const{
        return final;
    }

    // Returns the raw value of the cell
    CValue getRaw() const{
        return raw;
    }

    // Sets the final value of the cell
    void setFinal(const CValue final){
        this->final = final;
    }

    // Returns whether the cell contains an expression
    bool isExp() const{
        return isExpression;
    }

    // Changes the cell content
    void changeCell(std::string content){
        try {
            double number = std::stod(content);
            raw = number;
            final = number;
            isExpression = false;
            row_offset = 0;
            col_offset = 0;
        } catch (std::invalid_argument &e) {
            raw = content;
            final = content;
            isExpression = content[0] == '=';
        }
    }

    // Changes the offset of the cell
    void changeOffset(const int row, const int col){
        row_offset = row;
        col_offset = col;
    }

    // Returns the offset of the cell
    std::pair<int, int> getOffset() const{
        return {row_offset, col_offset};
    }

    // Returns the root of the AST
    const AstNode *getAstRoot() const{
        return m_astRoot.get();
    }

    // Sets the root of the AST
    void setAstRoot(std::shared_ptr<AstNode> root){
        m_astRoot = root;
    }
};
