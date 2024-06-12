# 📊 FIT Excel

This project provides a C++ implementation for handling Excel-like spreadsheets. The repository includes source files, build scripts, and test cases to facilitate development and testing.

## 🛠️ Getting Started

### 📋 Prerequisites

- **C++ Compiler**: Ensure you have a C++ compiler that supports C++20.
- **CMake**: Version 3.10 or higher.

### 🚀 Installation

Clone the repository:
- `git clone https://github.com/roman-princ/cpp-excel.git`
- `cd cpp-excel`
### 🔨 Building the Project

1. **Using CMake**:
- `mkdir build`
- `cd build`
- `cmake ..`
- `make`
2. **Manually Compiling `combined.cpp`**:
  
If you prefer to compile the `combined.cpp` file directly, you can use the following commands:
- `g++ -std=c++20 -Wall -pedantic -g -o excel combined.cpp -L . -l expression_parser`
- `./excel`
This will compile and run the `combined.cpp` file.
- You can generate the `combined.cpp` file by running the `makeone.py` script❗

### 🧪 Running Tests

To run the tests included in the repository, you can execute the compiled `excel` binary:
- `./excel`

## 📁 Repository Structure

- **`big/`**: Contains the main source files for the project.
  - **`AstNode.h`**
  - **`BinaryOpNode.h`**
  - **`CCell.h`**
  - **`CExpressionBuilder.h`**
  - **`CPos.h`**
  - **`CSpreadsheet.h`**
  - **`CValue.h`**
  - **`ReferenceNode.h`**
  - **`UnaryOpNode.h`**
  - **`ValueNode.h`**
  - **`combined.cpp`**: Combined source file for ProgTest submission.
  - **`expression.h`**
  - **`libexpression_parser.a`**: Static library for expression parsing.
  - **`makeone.py`**: Python script to combine source files for ProgTest.
  - **`test.cpp`**: Test cases for the project.

- **`CMakeLists.txt`**: CMake script for building the project.
- **`LICENSE`**: License file.
- **`README.md`**: Project documentation (you are here).

## 📜 License

This project is licensed under the GNU General Public License v3.0. See the [LICENSE](LICENSE) file for details.
