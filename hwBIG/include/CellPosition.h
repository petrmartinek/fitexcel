#ifndef CellPositionIncluded
#define CellPositionIncluded

#include <string_view>

struct CellPosition
{
    CellPosition(std::string_view str);
    
private:
    // todo
};

using CPos = CellPosition; // for progtest
#endif
