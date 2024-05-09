#ifndef CELL_POSITION_INCLUDED
#define CELL_POSITION_INCLUDED

#include <string_view>
#include <string>
#include <iostream>
#include <sstream>

struct CellPosition
{
    CellPosition(std::string_view str)
        : identifier(str), columnString(), columnIndex(), rowIndex()
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

    CellPosition(size_t column, size_t row)
        : identifier(), columnString(), columnIndex(column), rowIndex(row)
    {}

    bool operator<(const CellPosition& other) const;

    const std::string& string() const { return getIdentifier(); }
    size_t row() const { return rowIndex; }
    size_t column() const { return columnIndex; }
    const std::string& columnAlpha() const { return getColumnString(); }

    friend std::ostream& operator<<(std::ostream& os, const CellPosition& pos);

private:
    void createAlphaValues() const;
    
    const std::string& getIdentifier() const;
    const std::string& getColumnString() const;

    static std::string decToBase26(size_t decimal);
    static size_t base26ToDec(const std::string& hexavigesimal);
                                    // base26 == hexavigesimal https://en.wiktionary.org/wiki/hexavigesimal

    // mutable because it should save time not having to create strings from numbers
    // for example in a additive loop
    // only create it when first needed
    mutable std::string identifier;
    mutable std::string columnString;

    size_t columnIndex;
    size_t rowIndex;
};

using CPos = CellPosition; // for progtest
#endif
