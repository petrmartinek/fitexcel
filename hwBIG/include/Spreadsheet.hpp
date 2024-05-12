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

// uncomment the number if want to test it

constexpr unsigned MY_SPREADSHEET_CYCLIC_DEPS = 0x01;
constexpr unsigned MY_SPREADSHEET_FUNCTIONS = 0/* x02 */;
constexpr unsigned MY_SPREADSHEET_FILE_IO = 0/* x04 */;
constexpr unsigned MY_SPREADSHEET_SPEED = 0/* x08 */;
constexpr unsigned MY_SPREADSHEET_PARSER = 0/* x10 */;

struct Spreadsheet
{
    static unsigned capabilities()
    {
        return MY_SPREADSHEET_CYCLIC_DEPS | MY_SPREADSHEET_FUNCTIONS | MY_SPREADSHEET_FILE_IO | MY_SPREADSHEET_SPEED | MY_SPREADSHEET_PARSER;
    }

    Spreadsheet() = default;

    // rule of two three

    Spreadsheet(const Spreadsheet& other);
    const Spreadsheet& operator=(const Spreadsheet& other);
    ~Spreadsheet() = default;

    /**
     * @brief Saves current instance to output stream as series of strings ready to be loaded back in.
     * 
     * @param os std::ostream& to be written into 
     * 
     * @return `true` - if saved successfully
     * @return `false` - if `os` cannot be written into
     */
    bool save(std::ostream &os) const;

    /**
     * @brief Loads previously saved instance from input stream.
     * @brief If load fails, the current instance doesn't change.
     * 
     * @param is std::istream& to be read
     * 
     * @return `true` - if loaded successfully
     * @return `false` - if `is` cannot be read or the data saved is invalid
     */
    bool load(std::istream &is);

    /**
     * @brief Sets cell on position `pos` with expression or value.
     * 
     * @param pos CellPosition position of cell to set
     * @param contents expression or value to set the cell to
     * 
     * @return `true` - if `contents` is valid expression or value
     * @return `false` - if `contents` is invalid expression
     */
    bool setCell(CellPosition pos, std::string contents);

    /**
     * @brief Returns the computed value in cell on position `pos`.
     * 
     * @param pos CellPosition of cell to get value from
     * 
     * @return `CellValue` - computed value in cell on position `pos`.
     */
    CellValue getValue(CellPosition pos);

    /**
     * @brief Copies rectangle of cells from position `dst` of width `w` and height `h`
     * @brief to position `src`
     * 
     * @param dst destination position
     * @param src source position
     * @param w width of the rectangle
     * @param h height of the rectangle
     */
    void copyRect(CellPosition dst, CellPosition src, int w = 1, int h = 1);

private:

    std::map<CellPosition, Cell> table;
    std::map<CellPosition, std::set<CellPosition>> cellReferences;
   
    using Vertex = CellPosition;
    using Graph = std::map<Vertex, std::set<Vertex>>;

    enum STATE {FRESH, OPENED, CLOSED};
    
    using States = std::map<Vertex, STATE>;

    /**
     * @brief Checks if cell references in expression are cyclic.
     * 
     * @param graph graph of relation between cells
     * @param start starting cell
     * @return `true` - if expression is cyclic
     * @return `false` - if expression is not cyclic
     */
    static bool isCyclic(const Graph& graph, const Vertex& start);

    /**
     * @brief Recursive DFS for cyclic dependency. 
     * 
     * @param vertex vising vertex
     * @param graph graph to traverse
     * @param states states of vertexes
     * 
     * @throw `std::runtime_error` - when cyclic dependency is detected
     */
    static void dfs(const Vertex& vertex, const Graph& graph, States& states);
};

using CSpreadsheet = Spreadsheet; // for progtest
#endif
