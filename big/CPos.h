#pragma once
#include <string_view>

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

