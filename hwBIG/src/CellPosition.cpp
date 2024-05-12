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
    os << "Position: " << pos.string() << "| Column: " << pos.columnIndex << "| Row: " << pos.rowIndex << '\n';
    return os;
}

//------------------------------------------------------------------------------

// ♥♥♥ all love goes to this guy https://www.mathsisfun.com/base-conversion-method.html ♥♥♥
// this helped me how to make base-10 => base-26 and change the other way round
// however here was a issue, so i love this guy 3000 https://stackoverflow.com/a/48984697

// base-26:
// A = 1, B = 2, ... , Z = 26

constexpr int RADIX = 26;
constexpr int UPPER_ALPHABET_INDEX_MOD = 'A' - 1;
constexpr int LOWER_ALPHABET_INDEX_MOD = 'a' - 1;

std::string CellPosition::decToBase26(size_t decimal)
{
    std::string hexavigesimal;

    while(decimal > 0)
    {
        int remainder = decimal % RADIX;

        // if the remainder is zero, take one RADIX from decimal and make it the remainder
        if(!remainder)
        {
            remainder = RADIX;
            --decimal;
        }

        hexavigesimal.push_back(UPPER_ALPHABET_INDEX_MOD + remainder);

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
        // when calling from constructor theres no issue, but just in case
        if(!std::isalpha(*it))
        {
            throw std::invalid_argument("input not a base-26 number!");
        }

        // makes the coefficient in range of mod 26 case-insensitive
        char coefficient = *it % (std::isupper(*it) ? UPPER_ALPHABET_INDEX_MOD : LOWER_ALPHABET_INDEX_MOD);

        decimal += coefficient * std::pow(RADIX, index);

        ++index;
    }

    return decimal;
}

//------------------------------------------------------------------------------

void CellPosition::createAlphaValues() const
{
    columnString = decToBase26(columnIndex);
    identifier = columnString + std::to_string(rowIndex);
}

const std::string& CellPosition::getIdentifier() const
{
    // empty only if constructed from numbers instead from string
    if(!identifier.empty())
    {
        return identifier;
    }

    createAlphaValues();
    
    return identifier;
}

const std::string& CellPosition::getColumnString() const
{
    // empty only if constructed from numbers instead from string
    if(!columnString.empty())
    {
        return columnString;
    }

    createAlphaValues();

    return columnString;
}
