#include "../include/Spreadsheet.hpp"
#include "../include/CellPosition.hpp"
#include "../include/AST.hpp"
#include "../include/ExpressionBuilder.hpp"

#include <variant>
#include <string>
#include <utility>

using namespace std::literals;

// rule of three ---------------------------------------------------------------

Spreadsheet::Spreadsheet(const Spreadsheet& other)
{
    for(const auto& cell : other.table)
    {
        setCell(cell.first, "="s + cell.second->toString());
    }
}

const Spreadsheet& Spreadsheet::operator=(const Spreadsheet& other)
{
    table.clear();

    for(const auto& cell : other.table)
    {
        setCell(cell.first, "="s + cell.second->toString());
    }

    return *this;
}

//------------------------------------------------------------------------------

bool Spreadsheet::setCell(const CellPosition& pos, const std::string& contents)
{
    ExpressionBuilder builder(&table);
    parseExpression(contents, builder);

    table[pos] = builder.waitingList.top();

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
