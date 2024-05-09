#include "../include/Spreadsheet.hpp"
#include "../include/CellPosition.hpp"
#include "../include/AST.hpp"
#include "../include/ExpressionBuilder.hpp"
#include "../include/CellValue.hpp"

#include <variant>
#include <string>
#include <utility>
#include <iostream>

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

constexpr char COLUMN_DELIMETER = '\0'; 

bool Spreadsheet::save(std::ostream &os) const
{
    if(os.fail())
    {
        return false;
    }

    for(const auto& cell : table)
    {
        os << cell.first.string() << COLUMN_DELIMETER << ("="s + cell.second->toString()) << COLUMN_DELIMETER;
        
        CellValue value = cell.second->evaluate();

        if(std::holds_alternative<double>(value))
        {
            os << std::get<double>(value);
        } 
        else if(std::holds_alternative<std::string>(value))
        {
            os << std::get<std::string>(value);
        }
        os << COLUMN_DELIMETER;
    }

    return true;
}

bool Spreadsheet::load(std::istream &is)
{
    if(is.fail())
    {
        return false;
    }

    Spreadsheet loadedSpreadsheet;

    std::vector<CellValue> controlValues;
    std::vector<CellPosition> loadedCellsInOrder;

    while(is)
    {
        std::string loadPosition;
        std::string expression;
        CellValue value;

        std::getline(is, loadPosition, COLUMN_DELIMETER);

        if(loadPosition.empty())
        {
            if(is.eof())
            {
                break;
            }

            return false;
        }

        std::unique_ptr<CellPosition> position = nullptr;

        try
        {
            position = std::make_unique<CellPosition>(loadPosition);
        }
        catch(const std::exception& e)
        {
            // if position identifier is invalid

            return false;
        }
        

        std::getline(is, expression, COLUMN_DELIMETER);

        if(expression.empty() || expression.front() != '=')
        {
            return false;
        }

        std::string valueString;
        std::getline(is, valueString, COLUMN_DELIMETER);

        if(!valueString.empty()) // if not monostate
        {
            std::istringstream valueStream(valueString);
            
            double valueDouble;
            
            if(valueStream >> valueDouble)
            {
                value = valueDouble;
            }
            else // only string remains
            {
                value = valueString;
            }
        }

        if(!loadedSpreadsheet.setCell(*position, expression))
        {
            return false;
        }

        loadedCellsInOrder.emplace_back(*position);
        controlValues.emplace_back(value);
    }

    if(!is.eof())
    {
        return false;
    }

    if
    (
        !std::equal(loadedCellsInOrder.begin(), loadedCellsInOrder.end(), controlValues.begin(),
            [&](const CellPosition& position, const CellValue& value)
            {
                return valueMatchTop(loadedSpreadsheet.getValue(position), value);
            }
        )
    )
    {
        return false;
    }

    *this = loadedSpreadsheet;  

    return true; 
}

//------------------------------------------------------------------------------

bool Spreadsheet::setCell(CellPosition pos, std::string contents)
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

CellValue Spreadsheet::getValue(CellPosition pos)
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
    std::pair<long long, long long> positionShift = { (long long)dst.column() -  (long long)src.column(), (long long)dst.row() -  (long long)src.row()};

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
