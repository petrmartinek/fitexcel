#include "../include/Spreadsheet.hpp"
#include "../include/CellPosition.hpp"
#include "../include/AST.hpp"
#include "../include/ExpressionBuilder.hpp"

#include <iostream>
#include <variant>
#include <string>

bool Spreadsheet::setCell(const CellPosition& pos, const std::string& contents)
{
    ExpressionBuilder builder(&table);
    parseExpression(contents, builder);

    table[pos] = builder.waitingList.top();
    referencesInTable[pos] = builder.cellReferences;

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
