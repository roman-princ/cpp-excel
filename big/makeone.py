import os

# Define the files to combine
files_to_combine = ["AstNode.h","CValue.h","CCell.h" ,"CPos.h", "UnaryOpNode.h", "BinaryOpNode.h", "ValueNode.h","ReferenceNode.h", "CExpressionBuilder.h",
                    "CSpreadsheet.h"]  # Add all the files you want to combine here
main_file = "test.cpp"
# Define the output file
output_file = "combined.cpp"

with open(output_file, 'w') as outfile:
    # Iterate over each file and write the content to the output file until the #include "CSpreadsheet.h" line is encountered
    with open(main_file) as mainfile:
        for line in mainfile:
            if line.strip() == '#include "CSpreadsheet.h"':
                break
            outfile.write(line)

    # Iterate over each file and write the content to the output file, ingore includes and pragma once
    for file in files_to_combine:
        with open(file) as infile:
            for line in infile:
                if line.strip() == '#pragma once' or line.startswith("#include"):
                    continue
                outfile.write(line)

    # Write the content of the main file from the line after #include "CSpreadsheet.h" is encountered
    with open(main_file) as mainfile:
        write = False
        for line in mainfile:
            if line.strip() == '#include "CSpreadsheet.h"':
                write = True
                continue
            if write:
                outfile.write(line)
