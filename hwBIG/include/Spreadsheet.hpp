#ifndef SPREADSHEET_INCLUDED
#define SPREADSHEET_INCLUDED

#include "Expression.hpp"
#include "CellPosition.hpp"

#include <iostream>
#include <variant>
#include <string>
#include <map>

using namespace std::literals;
using CellValue = std::variant<std::monostate, double, std::string>;
using Cell = ExpressionBuilder;

constexpr unsigned SPREADSHEET_CYCLIC_DEPS = 0/* x01 */;
constexpr unsigned SPREADSHEET_FUNCTIONS = 0/* x02 */;
constexpr unsigned SPREADSHEET_FILE_IO = 0/* x04 */;
constexpr unsigned SPREADSHEET_SPEED = 0/* x08 */;
constexpr unsigned SPREADSHEET_PARSER = 0/* x10 */;

struct Spreadsheet
{
    static unsigned capabilities()
    {
        return SPREADSHEET_CYCLIC_DEPS | SPREADSHEET_FUNCTIONS | SPREADSHEET_FILE_IO | SPREADSHEET_SPEED | SPREADSHEET_PARSER;
    }

    Spreadsheet() = default;

    bool load(std::istream &is) { return true; }
    bool save(std::ostream &os) const { return true; }

    bool setCell(CellPosition pos, std::string contents);

    CellValue getValue(CellPosition pos);

    void copyRect(CellPosition dst, CellPosition src, int w = 1, int h = 1);

private:
    std::map<CellPosition, Cell> table;
};

using CValue = CellValue; // for progtest
using CSpreadsheet = Spreadsheet; // for progtest
#endif
