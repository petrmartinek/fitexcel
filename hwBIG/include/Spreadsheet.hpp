#ifndef SPREADSHEET_INCLUDED
#define SPREADSHEET_INCLUDED

#include "AST.hpp"
#include "CellPosition.hpp"
#include "ExpressionBuilder.hpp"
#include "CellValue.hpp"

#include <iostream>
#include <variant>
#include <string>
#include <map>
#include <vector>

using namespace std::literals;
using Cell = std::shared_ptr<Node>;

constexpr unsigned SPREADSHEET_CYCLIC_DEPS = 0/* x01 */;
constexpr unsigned SPREADSHEET_FUNCTIONS = 0/* x02 */;
constexpr unsigned SPREADSHEET_FILE_IO = 0/* x04 */;
constexpr unsigned SPREADSHEET_SPEED = 0/* x08 */;
constexpr unsigned SPREADSHEET_PARSER = 0/* x10 */;

struct Spreadsheet
{
    static unsigned capabilities()
    {
        return 0;
        // return SPREADSHEET_CYCLIC_DEPS | SPREADSHEET_FUNCTIONS | SPREADSHEET_FILE_IO | SPREADSHEET_SPEED | SPREADSHEET_PARSER;
    }

    Spreadsheet() = default;

    // rule of two three

    Spreadsheet(const Spreadsheet& other);
    const Spreadsheet& operator=(const Spreadsheet& other);
    ~Spreadsheet() = default;

    bool save(std::ostream &os) const;
    bool load(std::istream &is);

    bool setCell(CellPosition pos, std::string contents);

    CellValue getValue(CellPosition pos);

    void copyRect(CellPosition dst, CellPosition src, int w = 1, int h = 1);

private:

    std::map<CellPosition, Cell> table;
    std::map<CellPosition, std::set<CellPosition>> cellReferences;
   
    using Vertex = CellPosition;
    using Graph = std::map<Vertex, std::set<Vertex>>;
    enum STATE {FRESH, OPENED, CLOSED};
    using States = std::map<Vertex, STATE>;

    static bool isCyclic(const Graph& graph, const Vertex& start);
    static void dfs(const Vertex& vertex, const Graph& graph, States& states);
};

using CSpreadsheet = Spreadsheet; // for progtest
#endif
