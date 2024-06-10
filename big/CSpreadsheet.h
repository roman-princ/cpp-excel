#pragma once
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <sstream>
#include "CPos.h"
#include "CCell.h"
#include "CExpressionBuilder.h"

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