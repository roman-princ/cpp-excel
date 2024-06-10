#pragma once
#include <string>
#include <variant>

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