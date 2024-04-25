#ifndef CELL_POSITION_INCLUDED
#define CELL_POSITION_INCLUDED

#include <string_view>
#include <string>
#include <iostream>

struct CellPosition
{
    explicit CellPosition(const std::string_view& str);
    
    bool operator<(const CellPosition& other) const;

    friend std::ostream& operator<<(std::ostream& os, const CellPosition& pos);

private:
    size_t rowIndex;
    std::string columnIndex; // maybe not a good idea? consider numbers if issues arise
};

using CPos = CellPosition; // for progtest
#endif
