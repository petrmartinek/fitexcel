#ifndef SPREADSHEET_INCLUDED
#define SPREADSHEET_INCLUDED

#include "CellPosition.hpp"

#include <iostream>
#include <variant>
#include <string>

#ifndef __PROGTEST__

using CellValue = std::variant<std::monostate, double, std::string>;

constexpr unsigned SPREADSHEET_CYCLIC_DEPS = 0x01;
constexpr unsigned SPREADSHEET_FUNCTIONS = 0x02;
constexpr unsigned SPREADSHEET_FILE_IO = 0x04;
constexpr unsigned SPREADSHEET_SPEED = 0x08;
constexpr unsigned SPREADSHEET_PARSER = 0x10;

#endif /* __PROGTEST__ */

struct Spreadsheet
{
    static unsigned capabilities()
    {
        return SPREADSHEET_CYCLIC_DEPS | SPREADSHEET_FUNCTIONS | SPREADSHEET_FILE_IO | SPREADSHEET_SPEED | SPREADSHEET_PARSER;
    }

    Spreadsheet();

    bool load(std::istream &is);
    bool save(std::ostream &os) const;

    bool setCell(CellPosition pos, std::string contents);

    CellValue getValue(CellPosition pos);

    void copyRect(CellPosition dst, CellPosition src, int w = 1, int h = 1);

private:
    // todo
};

using CValue = CellValue; // for progtest
using CSpreadsheet = Spreadsheet; // for progtest
#endif
