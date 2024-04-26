#include "../include/Spreadsheet.hpp"
#include "../include/CellPosition.hpp"
#include "../include/Expression.hpp"

#include <iostream>
#include <variant>
#include <string>

bool Spreadsheet::setCell(const CellPosition& pos, const std::string& contents)
{
    table[pos] = Cell{new Expression{contents}};

    return true;
}

CellValue Spreadsheet::getValue(const CellPosition& pos)
{
    if(!table.contains(pos))
    {
        return CellValue();
    }

    return table[pos]->evaluate();
}
