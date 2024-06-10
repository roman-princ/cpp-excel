#ifndef __PROGTEST__

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <climits>
#include <cfloat>
#include <cassert>
#include <cmath>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <string>
#include <array>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <stack>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <functional>
#include <iterator>
#include <stdexcept>
#include <variant>
#include <optional>
#include <compare>
#include <charconv>
#include <span>
#include <utility>
#include <future>
#include "expression.h"

using namespace std::literals;
using CValue = std::variant<std::monostate, double, std::string>;
constexpr unsigned SPREADSHEET_CYCLIC_DEPS = 0x01;
constexpr unsigned SPREADSHEET_FUNCTIONS = 0x02;
constexpr unsigned SPREADSHEET_FILE_IO = 0x04;
constexpr unsigned SPREADSHEET_SPEED = 0x08;
constexpr unsigned SPREADSHEET_PARSER = 0x10;
#endif /* __PROGTEST__ */


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

using CValue = std::variant<std::monostate, double, std::string>;

// Addition operator for CValue
CValue operator+(const CValue &lhs, const CValue &rhs){
    if (std::holds_alternative<double>(lhs) && std::holds_alternative<double>(rhs)) {
        return std::get<double>(lhs) + std::get<double>(rhs);
    } else if (std::holds_alternative<std::string>(lhs) && std::holds_alternative<std::string>(rhs)) {
        return std::get<std::string>(lhs) + std::get<std::string>(rhs);
    } else if (std::holds_alternative<double>(lhs) && std::holds_alternative<std::string>(rhs)) {
        return std::to_string(std::get<double>(lhs)) + std::get<std::string>(rhs);
    } else if (std::holds_alternative<std::string>(lhs) && std::holds_alternative<double>(rhs)) {
        return std::get<std::string>(lhs) + std::to_string(std::get<double>(rhs));
    } else {
        return std::monostate();
    }
}

// Subtraction operator for CValue
CValue operator-(const CValue &lhs, const CValue &rhs) {
    if (std::holds_alternative<double>(lhs) && std::holds_alternative<double>(rhs)) {
        return std::get<double>(lhs) - std::get<double>(rhs);
    } else {
        return std::monostate();
    }
}

// Multiplication operator for CValue
CValue operator*(const CValue &lhs, const CValue &rhs) {
    if (std::holds_alternative<double>(lhs) && std::holds_alternative<double>(rhs)) {
        return std::get<double>(lhs) * std::get<double>(rhs);
    } else {
        return std::monostate();
    }
}

// Division operator for CValue
CValue operator/(const CValue &lhs, const CValue &rhs) {
    if (std::holds_alternative<double>(lhs) && std::holds_alternative<double>(rhs)) {
        if (std::get<double>(rhs) == 0) {
            return std::monostate();
        }
        return std::get<double>(lhs) / std::get<double>(rhs);
    } else {
        return std::monostate();
    }
}

// Negation operator for CValue
CValue operator!(const CValue &value) {
    if (std::holds_alternative<double>(value)) {
        return -std::get<double>(value);
    } else {
        return std::monostate();
    }
}

// Equality operator for CValue
CValue operator==(const CValue &lhs, const CValue &rhs) {
    if (std::holds_alternative<double>(lhs) && std::holds_alternative<double>(rhs)) {
        return static_cast<double>(std::get<double>(lhs) == std::get<double>(rhs));
    } else if (std::holds_alternative<std::string>(lhs) && std::holds_alternative<std::string>(rhs)) {
        return static_cast<double>(std::get<std::string>(lhs) == std::get<std::string>(rhs));
    } else {
        return std::monostate();
    }
}

// Not equal operator for CValue
CValue operator!=(const CValue &lhs, const CValue &rhs) {
    if (std::holds_alternative<double>(lhs) && std::holds_alternative<double>(rhs)) {
        return static_cast<double>(std::get<double>(lhs) != std::get<double>(rhs));
    } else if (std::holds_alternative<std::string>(lhs) && std::holds_alternative<std::string>(rhs)) {
        return static_cast<double>(std::get<std::string>(lhs) != std::get<std::string>(rhs));
    } else {
        return std::monostate();
    }
}

// Less than operator for CValue
CValue operator<(const CValue &lhs, const CValue &rhs) {
    if (std::holds_alternative<double>(lhs) && std::holds_alternative<double>(rhs)) {
        return static_cast<double>(std::get<double>(lhs) < std::get<double>(rhs));
    } else if (std::holds_alternative<std::string>(lhs) && std::holds_alternative<std::string>(rhs)) {
        return static_cast<double>(std::get<std::string>(lhs) < std::get<std::string>(rhs));
    } else {
        return std::monostate();
    }
}

// Less than or equal operator for CValue
CValue operator<=(const CValue &lhs, const CValue &rhs) {
    if (std::holds_alternative<double>(lhs) && std::holds_alternative<double>(rhs)) {
        return static_cast<double>(std::get<double>(lhs) <= std::get<double>(rhs));
    } else if (std::holds_alternative<std::string>(lhs) && std::holds_alternative<std::string>(rhs)) {
        return static_cast<double>(std::get<std::string>(lhs) <= std::get<std::string>(rhs));
    } else {
        return std::monostate();
    }
}

// Greater than operator for CValue
CValue operator>(const CValue &lhs, const CValue &rhs) {
    if (std::holds_alternative<double>(lhs) && std::holds_alternative<double>(rhs)) {
        return static_cast<double>(std::get<double>(lhs) > std::get<double>(rhs));
    } else if (std::holds_alternative<std::string>(lhs) && std::holds_alternative<std::string>(rhs)) {
        return static_cast<double>(std::get<std::string>(lhs) > std::get<std::string>(rhs));
    } else {
        return std::monostate();
    }
}

// Greater than or equal operator for CValue
CValue operator>=(const CValue &lhs, const CValue &rhs) {
    if (std::holds_alternative<double>(lhs) && std::holds_alternative<double>(rhs)) {
        return static_cast<double>(std::get<double>(lhs) >= std::get<double>(rhs));
    } else if (std::holds_alternative<std::string>(lhs) && std::holds_alternative<std::string>(rhs)) {
        return static_cast<double>(std::get<std::string>(lhs) >= std::get<std::string>(rhs));
    } else {
        return std::monostate();
    }
}
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

// Represents a position in a spreadsheet
class CPos {
public:
    CPos() = default;

    // Constructs a position with the given string
    CPos(std::string_view str){
        try {
            parsePosition(str);
        }
        catch (std::exception &e) {
            throw std::invalid_argument("Invalid position format");
        }
    }

    // Constructs a position with the given row and column and whether they are absolute
    CPos(int row, int column, bool absRow, bool absCol) : m_row(row), m_column(column), m_absoluteRow(absRow),
    m_absoluteColumn(absCol) {}

    // Compares two positions
    bool operator<(const CPos &other) const{
        return m_row < other.m_row || (m_row == other.m_row && m_column < other.m_column);
    }

    // Compares two positions
    bool operator==(const CPos &other) const{
        return m_row == other.m_row && m_column == other.m_column;
    }

    // Returns the position
    std::pair<int, int> getPosition() const{
        return {m_row, m_column};
    }

    // Parses the given string view into a position
    void parsePosition(std::string_view str){
        m_absoluteColumn = false;
        m_absoluteRow = false;
        size_t pos = 0;
        std::string columnStr;
        str = convertToUpperCase(str.data());
        while (pos < str.size()) {
            if (str[pos] == '$' && columnStr.empty()) {
                m_absoluteColumn = true;
                pos++;
            } else if (std::isalpha(str[pos])) {
                columnStr.push_back(std::toupper(str[pos]));
                pos++;
            } else break;
        }
        if (columnStr.empty()) {
            throw std::invalid_argument("Invalid position format");
        }
        m_column = encodeColumn(columnStr);
        str = str.substr(pos, str.size() - pos).data();
        pos = 0;
        std::string rowStr;
        while (pos < str.size()) {
            if (rowStr.empty() && str[pos] == '$') {
                m_absoluteRow = true;
                pos++;
            } else if (std::isdigit(str[pos])) {
                rowStr.push_back(str[pos]);
                pos++;
            } else {
                break;
            }
        }
        if (rowStr.empty()) {
            throw std::invalid_argument("Invalid position format");
        }
        m_row = std::stoi(rowStr);
        if (pos != str.size()) {
            throw std::invalid_argument("Invalid position format");
        }
    }
    // Returns whether the row and column are absolute
    std::pair<bool, bool> isAbsolute() const{
        return {m_absoluteRow, m_absoluteColumn};

    }

private:
    int m_row;
    int m_column;
    bool m_absoluteRow;
    bool m_absoluteColumn;

    // Converts the given string to uppercase
    std::string convertToUpperCase(const std::string &str) const{
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(),
                       [](unsigned char c) { return std::toupper(c); });
        return result;
    }
    // Encodes the given column string into a number
    int encodeColumn(const std::string &colStr) const{
        int columnNumber = 0;
        for (char c: colStr) {
            columnNumber = columnNumber * 26 + (c - 'A' + 1);
        }
        return columnNumber;
    }
};
// Hash function for CPos
namespace std {
    template<>
    struct hash<CPos> {
        std::size_t operator()(const CPos &pos) const {
            std::size_t hashValue = 0;
            hashValue ^= std::hash<unsigned int>{}(pos.getPosition().first);
            hashValue ^= std::hash<unsigned int>{}(pos.getPosition().second);
            return hashValue;
        }
    };
}


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


class CSpreadsheet {
private:
    std::map<CPos, CCell> m_sheet;

public:

    // idk how to access the exprs from test.cpp
    static unsigned capabilities(){
        return 0x01 | 0x04;
    }

    CSpreadsheet() = default;

    // Copy constructor
    CSpreadsheet &operator=(const CSpreadsheet &other){
        if (this != &other) {
            m_sheet = other.m_sheet;
            for (auto &cell: m_sheet) {
                if (cell.second.isExp()) {
                    CExpressionBuilder builder(*this, cell.second);
                    parseExpression(std::get<std::string>(cell.second.getRaw()), builder);
                    cell.second.setAstRoot(builder.getRoot());
                }

            }
            return *this;
        }
        return *this;
    }

    // Load the spreadsheet from the given input stream
    bool load(std::istream &is){
        if (!is) {
            return false;
        }
        CExpressionBuilder builder;
        std::string data((std::istreambuf_iterator<char>(is)), {});
        std::string checksum;
        std::string content;
        data.find("HASH:");
        if (data.find("HASH:") != std::string::npos) {
            checksum = data.substr(data.find("HASH:") + 5, data.size());
            content = data.substr(0, data.find("HASH:"));
        } else {
            return false;
        }
        std::stringstream ss(content);
        std::string hash = std::to_string(std::hash<std::string>{}(ss.str()));
        if (hash != checksum) {
            return false;
        }
        std::string line;
        while (std::getline(ss, line)) {
            size_t pos = line.find(':');
            if (pos == std::string::npos) {
                continue;
            }

            std::string posStr = line.substr(0, pos);
            std::string content = line.substr(pos + 1, line.find('|') - pos - 1);
            std::string offset = line.substr(line.find('|') + 1, line.find('\n') - line.find('|') - 1);

            pos = posStr.find(',');
            if (pos == std::string::npos) {
                continue;
            }
            int row = static_cast<int>(std::stoul(posStr.substr(0, pos)));
            int col = static_cast<int>(std::stoul(posStr.substr(pos + 1)));

            pos = offset.find(',');
            if (pos == std::string::npos) {
                continue;
            }

            int row_offset = static_cast<int>(std::stoul(offset.substr(0, pos)));
            int col_offset = static_cast<int>(std::stoul(offset.substr(pos + 1)));

            std::string decodedContent;
            bool escapeNext = false;
            for (char c: content) {
                if (escapeNext) {
                    if (c == '"' || c == '\\') {
                        decodedContent += c;
                    } else {
                        decodedContent += '\\';
                        decodedContent += c;
                    }
                    escapeNext = false;
                } else {
                    if (c == '\\') {
                        escapeNext = true;
                    } else {
                        decodedContent += c;
                    }
                }
            }

            CPos cellPos(decodeColumn(col) + std::to_string(row));
            setCell(cellPos, decodedContent, row_offset, col_offset);
        }
        return true;
    }

    // Save the spreadsheet to the given output stream
    bool save(std::ostream &os) const{
        if (!os)
            return false;
        std::stringstream ss;
        for (const auto &cellPair: m_sheet) {
            const CPos &pos = cellPair.first;
            const CCell &cell = cellPair.second;

            // Handle string content (escape quotes if needed)
            std::string content;
            if (std::holds_alternative<std::string>(cell.getRaw())) {
                content = std::get<std::string>(cell.getRaw());
            } else if (std::holds_alternative<double>(cell.getRaw())) {
                content = std::to_string(std::get<double>(cell.getRaw()));
            } else {
                continue;
            }
            ss << pos.getPosition().first << "," << pos.getPosition().second << ":";
            for (char c: content) {
                if (c == '"' || c == '\\') {
                    ss << '\\';
                } else if (c == '\n') {
                    ss << "\\n";
                    continue;
                }
                ss << c;
            }
            ss << "|" << cell.getOffset().first << "," << cell.getOffset().second << "\n";
        }

        std::string hash = std::to_string(std::hash<std::string>{}(ss.str()));
        ss << "HASH:" << hash;
        os << ss.str();
        return true;
    }

    // Set the contents of the cell at the given position
    bool setCell(CPos pos,
                 std::string contents){
        if (pos.getPosition().first < 0 || pos.getPosition().second < 0)
            return false;
        auto it = m_sheet.find(pos);
        if (it != m_sheet.end()) {
            it->second.changeOffset(0, 0);
            it->second.changeCell(contents);
            if (it->second.isExp()) {
                it->second.setAstRoot(nullptr);
                CExpressionBuilder builder(*this, it->second);
                parseExpression(contents, builder);
                it->second.setAstRoot(builder.getRoot());
            }
        } else {
            m_sheet.insert({pos, CCell(contents)});
            if (m_sheet[pos].isExp()) {
                CExpressionBuilder builder(*this, m_sheet[pos]);
                try {
                    parseExpression(contents, builder);
                }
                catch (std::exception &e) {
                    m_sheet.erase(pos);
                    return false;
                }
                m_sheet[pos].setAstRoot(builder.getRoot());
            }
        }
        return true;
    }

    // Get the contents of the cell at the given position
    CValue getValue(CPos pos){
        auto it = m_sheet.find(pos);
        if (it == m_sheet.end()) {
            return CValue();
        }
        if (it->second.isExp()) {
            try {
                it->second.setFinal(it->second.getAstRoot()->evaluate());
            }
            catch (std::exception &e) {
                visited.clear();
                return CValue();
            }
            visited.clear();
        }
        CValue finalValue = it->second.getFinal();
        return finalValue;
    }

    // Copies content from src to dst with h and w dimensions
    void copyRect(CPos dst,
                  CPos src,
                  int w = 1,
                  int h = 1){
        if (w <= 0 || h <= 0) {
            return;
        }
        if (dst.getPosition().first < 0 || dst.getPosition().second < 0 || src.getPosition().first < 0 ||
            src.getPosition().second < 0)
            return;
        std::unordered_map<CPos, CCell> tempSrcPos;
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                CPos srcPos(decodeColumn(src.getPosition().second + j) + std::to_string(src.getPosition().first + i));
                if (m_sheet.find(srcPos) != m_sheet.end()) {
                    tempSrcPos.insert({srcPos, m_sheet[srcPos]});
                }
            }
        }

        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                CPos srcPos(src.getPosition().first + i, src.getPosition().second + j, false, false);
                CPos dstPos = CPos(dst.getPosition().first + i, dst.getPosition().second + j, false, false);
                auto it = tempSrcPos.find(srcPos);
                if (it == tempSrcPos.end()) {
                    m_sheet.erase(dstPos);
                    continue;
                }
                CValue value = it->second.getRaw();
                if (!it->second.isExp()) {
                    if (std::holds_alternative<std::string>(value))
                        setCell(dstPos, std::get<std::string>(value));
                    else if (std::holds_alternative<double>(value))
                        setCell(dstPos, std::to_string(std::get<double>(value)));
                    else {
                        m_sheet[dstPos] = CCell();
                    }
                } else {
                    int row = dstPos.getPosition().first - srcPos.getPosition().first + it->second.getOffset().first;
                    int col = dstPos.getPosition().second - srcPos.getPosition().second + it->second.getOffset().second;
                    setCell(dstPos, std::get<std::string>(value), row, col);
                }
            }
        }
    }

    // Decode column number to string
    std::string decodeColumn(const int &col) const{
        std::string columnStr;
        int column = col;
        while (column > 0) {
            int rem = column % 26;
            if (rem == 0) {
                columnStr.push_back('Z');
                column = (column / 26) - 1;
            } else {
                columnStr.push_back((rem - 1) + 'A');
                column = column / 26;
            }
        }
        std::reverse(columnStr.begin(), columnStr.end());
        return columnStr;
    }

    // set cell with offset
    bool setCell(CPos pos, std::string contents, int row_offset, int col_offset){
        if (pos.getPosition().first < 0 || pos.getPosition().second < 0)
            return false;
        auto it = m_sheet.find(pos);
        if (it != m_sheet.end()) {
            it->second.changeCell(contents);
            if (it->second.isExp()) {
                it->second.setAstRoot(nullptr);
                it->second.changeOffset(row_offset, col_offset);
                CExpressionBuilder builder(*this, it->second);
                try {
                    parseExpression(contents, builder);
                }
                catch (std::exception &e) {
                    m_sheet.erase(pos);
                    return false;
                }
                it->second.setAstRoot(builder.getRoot());
            }
        } else {
            m_sheet.insert({pos, CCell(contents)});
            if (m_sheet[pos].isExp()) {
                m_sheet[pos].changeOffset(row_offset, col_offset);
                CExpressionBuilder builder(*this, m_sheet[pos]);
                try {
                    parseExpression(contents, builder);
                }
                catch (std::exception &e) {
                    m_sheet.erase(pos);
                    return false;
                }
                m_sheet[pos].setAstRoot(builder.getRoot());
            }
        }
        return true;
    }

    // Proccesses the AST and returns the result
    CValue getValueOfReferenceNode(CPos pos){
        if (visited.count(pos) > 0) {
            throw std::runtime_error("Cyclic dependency");
        }
        visited.insert(pos);
        auto it = m_sheet.find(pos);
        if (it == m_sheet.end()) {
            return CValue();
        }
        if (it->second.isExp()) {
            it->second.setFinal(it->second.getAstRoot()->evaluate());
        }
        CValue finalValue = it->second.getFinal();
        visited.erase(pos);
        return finalValue;
    }

    std::unordered_set<CPos> visited;
};

CValue ReferenceNode::evaluate() const{
    return spreadsheet->getValueOfReferenceNode(pos);
}
#ifndef __PROGTEST__

bool valueMatch(const CValue &r,
                const CValue &s) {
    if (r.index() != s.index())
        return false;
    if (r.index() == 0)
        return true;
    if (r.index() == 2)
        return std::get<std::string>(r) == std::get<std::string>(s);
    if (std::isnan(std::get<double>(r)) && std::isnan(std::get<double>(s)))
        return true;
    if (std::isinf(std::get<double>(r)) && std::isinf(std::get<double>(s)))
        return (std::get<double>(r) < 0 && std::get<double>(s) < 0)
               || (std::get<double>(r) > 0 && std::get<double>(s) > 0);
    return fabs(std::get<double>(r) - std::get<double>(s)) <= 1e8 * DBL_EPSILON * fabs(std::get<double>(r));
}

int main() {
    CSpreadsheet x0, x1;
    std::ostringstream oss;
    std::istringstream iss;
    std::string data;
    assert(x0.setCell(CPos("A1"), "10"));
    assert(x0.setCell(CPos("A2"), "20.5"));
    assert(x0.setCell(CPos("A3"), "3e1"));
    assert(x0.setCell(CPos("A4"), "=40"));
    assert(x0.setCell(CPos("A5"), "=5e+1"));
    assert(x0.setCell(CPos("A6"), "raw text with any characters, including a quote \" or a newline\n"));
    assert(x0.setCell(CPos("A7"),
                      "=\"quoted string, quotes must be doubled: \"\". Moreover, backslashes are needed for C++.\""));
    assert(valueMatch(x0.getValue(CPos("A1")), CValue(10.0)));
    assert(valueMatch(x0.getValue(CPos("A2")), CValue(20.5)));
    assert(valueMatch(x0.getValue(CPos("A3")), CValue(30.0)));
    assert(valueMatch(x0.getValue(CPos("A4")), CValue(40.0)));
    assert(valueMatch(x0.getValue(CPos("A5")), CValue(50.0)));
    assert(valueMatch(x0.getValue(CPos("A6")),
                      CValue("raw text with any characters, including a quote \" or a newline\n")));
    assert(valueMatch(x0.getValue(CPos("A7")),
                      CValue("quoted string, quotes must be doubled: \". Moreover, backslashes are needed for C++.")));
    assert(valueMatch(x0.getValue(CPos("A8")), CValue()));
    assert(valueMatch(x0.getValue(CPos("AAAA9999")), CValue()));
    assert(x0.setCell(CPos("B1"), "=A1+A2*A3"));
    assert(x0.setCell(CPos("B2"), "= -A1 ^ 2 - A2 / 2   "));
    assert(x0.setCell(CPos("B3"), "= 2 ^ $A$1"));
    assert(x0.setCell(CPos("B4"), "=($A1+A$2)^2"));
    assert(x0.setCell(CPos("B5"), "=B1+B2+B3+B4"));
    assert(x0.setCell(CPos("B6"), "=B1+B2+B3+B4+B5"));
    assert(valueMatch(x0.getValue(CPos("B1")), CValue(625.0)));
    assert(valueMatch(x0.getValue(CPos("B2")), CValue(-110.25)));
    assert(valueMatch(x0.getValue(CPos("B3")), CValue(1024.0)));
    assert(valueMatch(x0.getValue(CPos("B4")), CValue(930.25)));
    assert(valueMatch(x0.getValue(CPos("B5")), CValue(2469.0)));
    assert(valueMatch(x0.getValue(CPos("B6")), CValue(4938.0)));
    assert(x0.setCell(CPos("A1"), "12"));
    assert(valueMatch(x0.getValue(CPos("B1")), CValue(627.0)));
    assert(valueMatch(x0.getValue(CPos("B2")), CValue(-154.25)));
    assert(valueMatch(x0.getValue(CPos("B3")), CValue(4096.0)));
    assert(valueMatch(x0.getValue(CPos("B4")), CValue(1056.25)));
    assert(valueMatch(x0.getValue(CPos("B5")), CValue(5625.0)));
    assert(valueMatch(x0.getValue(CPos("B6")), CValue(11250.0)));
    x1 = x0;
    assert(x0.setCell(CPos("A2"), "100"));
    assert(x1.setCell(CPos("A2"), "=A3+A5+A4"));
    assert(valueMatch(x0.getValue(CPos("B1")), CValue(3012.0)));
    assert(valueMatch(x0.getValue(CPos("B2")), CValue(-194.0)));
    assert(valueMatch(x0.getValue(CPos("B3")), CValue(4096.0)));
    assert(valueMatch(x0.getValue(CPos("B4")), CValue(12544.0)));
    assert(valueMatch(x0.getValue(CPos("B5")), CValue(19458.0)));
    assert(valueMatch(x0.getValue(CPos("B6")), CValue(38916.0)));
    assert(valueMatch(x1.getValue(CPos("B1")), CValue(3612.0)));
    assert(valueMatch(x1.getValue(CPos("B2")), CValue(-204.0)));
    assert(valueMatch(x1.getValue(CPos("B3")), CValue(4096.0)));
    assert(valueMatch(x1.getValue(CPos("B4")), CValue(17424.0)));
    assert(valueMatch(x1.getValue(CPos("B5")), CValue(24928.0)));
    assert(valueMatch(x1.getValue(CPos("B6")), CValue(49856.0)));
    oss.clear();
    oss.str("");
    assert(x0.save(oss));
    data = oss.str();
    iss.clear();
    iss.str(data);
    assert(x1.load(iss));
    assert(valueMatch(x1.getValue(CPos("B1")), CValue(3012.0)));
    assert(valueMatch(x1.getValue(CPos("B2")), CValue(-194.0)));
    assert(valueMatch(x1.getValue(CPos("B3")), CValue(4096.0)));
    assert(valueMatch(x1.getValue(CPos("B4")), CValue(12544.0)));
    assert(valueMatch(x1.getValue(CPos("B5")), CValue(19458.0)));
    assert(valueMatch(x1.getValue(CPos("B6")), CValue(38916.0)));
    assert(x0.setCell(CPos("A3"), "4e1"));
    assert(valueMatch(x1.getValue(CPos("B1")), CValue(3012.0)));
    assert(valueMatch(x1.getValue(CPos("B2")), CValue(-194.0)));
    assert(valueMatch(x1.getValue(CPos("B3")), CValue(4096.0)));
    assert(valueMatch(x1.getValue(CPos("B4")), CValue(12544.0)));
    assert(valueMatch(x1.getValue(CPos("B5")), CValue(19458.0)));
    assert(valueMatch(x1.getValue(CPos("B6")), CValue(38916.0)));
    oss.clear();
    oss.str("");
    assert(x0.save(oss));
    data = oss.str();
    for (size_t i = 0; i < std::min<size_t>(data.length(), 10); i++)
        data[i] ^= 0x5a;
    iss.clear();
    iss.str(data);
    assert(!x1.load(iss));
    assert(x0.setCell(CPos("D0"), "10"));
    assert(x0.setCell(CPos("D1"), "20"));
    assert(x0.setCell(CPos("D2"), "30"));
    assert(x0.setCell(CPos("D3"), "40"));
    assert(x0.setCell(CPos("D4"), "50"));
    assert(x0.setCell(CPos("E0"), "60"));
    assert(x0.setCell(CPos("E1"), "70"));
    assert(x0.setCell(CPos("E2"), "80"));
    assert(x0.setCell(CPos("E3"), "90"));
    assert(x0.setCell(CPos("E4"), "100"));
    assert(x0.setCell(CPos("F10"), "=D0+5"));
    assert(x0.setCell(CPos("F11"), "=$D0+5"));
    assert(x0.setCell(CPos("F12"), "=D$0+5"));
    assert(x0.setCell(CPos("F13"), "=$D$0+5"));
    x0.copyRect(CPos("G11"), CPos("F10"), 1, 4);
    assert(valueMatch(x0.getValue(CPos("F10")), CValue(15.0)));
    assert(valueMatch(x0.getValue(CPos("F11")), CValue(15.0)));
    assert(valueMatch(x0.getValue(CPos("F12")), CValue(15.0)));
    assert(valueMatch(x0.getValue(CPos("F13")), CValue(15.0)));
    assert(valueMatch(x0.getValue(CPos("F14")), CValue()));
    assert(valueMatch(x0.getValue(CPos("G10")), CValue()));
    assert(valueMatch(x0.getValue(CPos("G11")), CValue(75.0)));
    assert(valueMatch(x0.getValue(CPos("G12")), CValue(25.0)));
    assert(valueMatch(x0.getValue(CPos("G13")), CValue(65.0)));
    assert(valueMatch(x0.getValue(CPos("G14")), CValue(15.0)));
    x0.copyRect(CPos("G11"), CPos("F10"), 2, 4);
    assert(valueMatch(x0.getValue(CPos("F10")), CValue(15.0)));
    assert(valueMatch(x0.getValue(CPos("F11")), CValue(15.0)));
    assert(valueMatch(x0.getValue(CPos("F12")), CValue(15.0)));
    assert(valueMatch(x0.getValue(CPos("F13")), CValue(15.0)));
    assert(valueMatch(x0.getValue(CPos("F14")), CValue()));
    assert(valueMatch(x0.getValue(CPos("G10")), CValue()));
    assert(valueMatch(x0.getValue(CPos("G11")), CValue(75.0)));
    assert(valueMatch(x0.getValue(CPos("G12")), CValue(25.0)));
    assert(valueMatch(x0.getValue(CPos("G13")), CValue(65.0)));
    assert(valueMatch(x0.getValue(CPos("G14")), CValue(15.0)));
    assert(valueMatch(x0.getValue(CPos("H10")), CValue()));
    assert(valueMatch(x0.getValue(CPos("H11")), CValue()));
    assert(valueMatch(x0.getValue(CPos("H12")), CValue()));
    assert(valueMatch(x0.getValue(CPos("H13")), CValue(35.0)));
    assert(valueMatch(x0.getValue(CPos("H14")), CValue()));
    assert(x0.setCell(CPos("F0"), "-27"));
    assert(valueMatch(x0.getValue(CPos("H14")), CValue(-22.0)));
    x0.copyRect(CPos("H12"), CPos("H13"), 1, 2);
    assert(valueMatch(x0.getValue(CPos("H12")), CValue(25.0)));
    assert(valueMatch(x0.getValue(CPos("H13")), CValue(-22.0)));
    assert(valueMatch(x0.getValue(CPos("H14")), CValue(-22.0)));

    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
