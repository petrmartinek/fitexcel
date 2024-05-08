#ifndef CELL_POSITION_INCLUDED
#define CELL_POSITION_INCLUDED

#include <string_view>
#include <string>
#include <iostream>

struct CellPosition
{
    explicit CellPosition(const std::string_view& str)
        : identifier(str), columnString(), columnIndex(), rowIndex()
    {}
    CellPosition(size_t column, size_t row)
        : identifier(), columnString(), columnIndex(column), rowIndex(row)
    {}

    bool operator<(const CellPosition& other) const;

    const std::string& string() { return createIdentifier(); }
    size_t row() const { return rowIndex; }
    size_t column() const { return columnIndex; }

    friend std::ostream& operator<<(std::ostream& os, const CellPosition& pos);

private:
    const std::string& createIdentifier();

    static std::string decToBase26(size_t decimal);
    static size_t base26ToDec(const std::string& hexavigesimal);
                                    // base26 == hexavigesimal https://en.wiktionary.org/wiki/hexavigesimal

    std::string identifier;
    std::string columnString;

    size_t columnIndex;
    size_t rowIndex;
};

using CPos = CellPosition; // for progtest
#endif
