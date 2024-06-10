cmake_minimum_required(VERSION 3.10)
project(big)

set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
link_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(big
        CValue.h
        CPos.h
        CExpressionBuilder.h
        CSpreadsheet.h
        CCell.h
        AstNode.h
        BinaryOpNode.h
        UnaryOpNode.h
        ValueNode.h
        ReferenceNode.h
        test.cpp
)

target_link_libraries(big PRIVATE libexpression_parser.a)