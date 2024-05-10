#!/bin/bash
grep -vh '^#include' \
    include/CellValue.hpp include/CellPosition.hpp include/AST.hpp include/ExpressionBuilder.hpp include/Spreadsheet.hpp \
    src/CellPosition.cpp src/AST.cpp src/ExpressionBuilder.cpp src/Spreadsheet.cpp \
    > almost
grep -vh '^using namespace' almost > all_in_one.cpp; rm almost
