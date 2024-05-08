#include "../include/CellPosition.hpp"

#include <string_view>
#include <string>
#include <cstring>
#include <iostream>
#include <sstream>
#include <cctype>
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include <cctype>

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

    columnString = str.substr(0, position);

    columnIndex = base26ToDec(columnString);

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
        return columnIndex < other.columnIndex;
    }

    return rowIndex < other.rowIndex;
}

std::ostream& operator<<(std::ostream& os, const CellPosition& pos)
{
    os << "Column: " << pos.columnIndex << " Row: " << pos.rowIndex << '\n';
    return os;
}

//------------------------------------------------------------------------------

// ♥♥♥ all love goes to this guy https://www.mathsisfun.com/base-conversion-method.html ♥♥♥
// this helped me how to make base-10 => base-26 and change the other way round

// base-26:
// Z = 0, A = 1, B = 2, ... , Y = 25

constexpr int RADIX = 26;
constexpr int UPPER_ALPHABET_INDEX_MOD = 'A' - 1;
constexpr int LOWER_ALPHABET_INDEX_MOD = 'a' - 1;

std::string CellPosition::decToBase26(size_t decimal)
{
    std::string hexavigesimal;

    while(decimal)
    {
        int remainder = decimal % RADIX;

        hexavigesimal.push_back(remainder != 0 ? UPPER_ALPHABET_INDEX_MOD + remainder : 'Z');

        decimal /= RADIX;
    }

    std::reverse(hexavigesimal.begin(), hexavigesimal.end());

    return hexavigesimal;
}

size_t CellPosition::base26ToDec(const std::string& hexavigesimal)
{
    size_t decimal = 0;
    size_t index = 0;

    for(auto it = hexavigesimal.rbegin(); it != hexavigesimal.rend(); ++it)
    {
        char coefficient = *it % (std::isupper(*it) ? UPPER_ALPHABET_INDEX_MOD : LOWER_ALPHABET_INDEX_MOD) % RADIX;
        
        decimal += coefficient * std::pow(RADIX, index);

        ++index;
    }

    return decimal;
}

//------------------------------------------------------------------------------

const std::string& CellPosition::createIdentifier()
{
    if(!identifier.empty())
    {
        return identifier;
    }

    if(columnString.empty())
    {
        columnString = decToBase26(columnIndex);
    }

    identifier = columnString + std::to_string(rowIndex);

    return identifier;
}
