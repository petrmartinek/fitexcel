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

    try
    {
        parseExpression(contents, builder);
    }
    catch(const std::exception& e)
    {
        return false;
    }
    
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

//------------------------------------------------------------------------------

void Spreadsheet::copyRect(CellPosition dst, CellPosition src, int w, int h)
{
    std::pair<int, int> positionShift = {dst.column() - src.column(), dst.row() - src.row()};

    for(size_t i = src.column(); i < src.column() + w; ++i)
    {
        for(size_t j = src.row(); j < src.row() + h; ++j)
        {
            if(!table.contains(CellPosition{i, j}))
            {
                continue;
            }

            size_t dstI = i + positionShift.first;
            size_t dstJ = j + positionShift.second;

            table[CellPosition{dstI, dstJ}] = table[CellPosition{i, j}]->clone();
            table[CellPosition{dstI, dstJ}]->updateReferences(positionShift);
        }
    }
}
