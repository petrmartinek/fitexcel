#include "../include/CellPosition.hpp"

#include <string_view>
#include <string>
#include <cstring>
#include <iostream>
#include <sstream>
#include <cctype>
#include <stdexcept>
#include <algorithm>

CellPosition::CellPosition(const std::string_view& str)
{
    if(str.size() < 2 || !std::isalpha(str.front()) || !std::isdigit(str.back()))
    {
        throw std::invalid_argument("Invalid cell identifier");
    }

    size_t position = 0;

    while(std::isalpha(str[position]))
    {
        ++position;
    }

    columnIndex = str.substr(0, position);

    std::istringstream iss(std::string{str.substr(position)});

    if(!(iss >> rowIndex))
    {
        throw std::invalid_argument("Invalid row index");
    }
}

//------------------------------------------------------------------------------

bool CellPosition::operator<(const CellPosition& other) const
{
    if(rowIndex == other.rowIndex)
    {
        return strcasecmp(columnIndex.c_str(), other.columnIndex.c_str()) < 0;
    }

    return rowIndex < other.rowIndex;
}

std::ostream& operator<<(std::ostream& os, const CellPosition& pos)
{
    os << "Column: " << pos.columnIndex << " Row: " << pos.rowIndex << '\n';
    return os;
}

//------------------------------------------------------------------------------
