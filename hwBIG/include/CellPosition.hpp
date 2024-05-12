#ifndef CELL_POSITION_INCLUDED
#define CELL_POSITION_INCLUDED

#include <string_view>
#include <string>
#include <iostream>
#include <sstream>

struct CellPosition
{
    /**
     * @brief Construct a new CellPosition instance. Constructs all instance's parameters.
     * 
     * @param str string value of cell position, letters followed by numbers, e.g.: "A1", "AZBEASD1234"
     * 
     * @throw `std::invalid_argument` - if `str` is not in correct format
     */
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

    /**
     * @brief Construct a new CellPosition instance. Sets column and row numbers.
     * @brief Doesn't construct instance's string value and column's alphabetical form.
     * 
     * @param column number of column
     * @param row number of row
     */
    CellPosition(size_t column, size_t row)
        : identifier(), columnString(), columnIndex(column), rowIndex(row)
    {}

    /**
     * @brief Compares two CellPosition instances by comparing its parameters using operator <.
     * @brief Compares columns. If columns are equal, then compares rows.
     * 
     * @param other CellPosition to compare with
     * 
     * @return `true`  - if *this is less than other
     * @return `false` - if *this is greater or equal than other
     */
    bool operator<(const CellPosition& other) const;

    /**
     * @brief Returns instance's value in string. If the instance doesn't have string value
     * @brief , the string value is constructed.
     * 
     * @return `const std::string&` - instance's string value
     */
    const std::string& string() const { return getIdentifier(); }

    /**
     * @brief Returns instance's row number.
     * 
     * @return `size_t` - row number
     */
    size_t row() const { return rowIndex; }
    
    /**
     * @brief Returns instance's column in number form.
     * 
     * @return `size_t` - column's number
     */
    size_t column() const { return columnIndex; }

    /**
     * @brief Returns instance's column in alphabetical form. If the instance doesn't have
     * @brief the value ready, constructs it.
     * 
     * @return `const std::string&` - column's string value 
     */
    const std::string& columnAlpha() const { return getColumnString(); }

    /**
     * @brief Prints current instance in the following format: 
     * @brief "Position: ${string value} | Column: ${column number} | Row: ${row number}"
     * 
     * @param os output stream to print the instance in
     * @param pos instance to print
     * 
     * @return `std::ostream&` - the output stream
     */
    friend std::ostream& operator<<(std::ostream& os, const CellPosition& pos);

private:
    /**
     * @brief Constructs both the string value and column's alphabetical form of current instance.
     */
    void createAlphaValues() const;
    
    /**
     * @brief Returns string value of a instance. If not constructed, calls createAlphaValues().
     * 
     * @return `const std::string&` - instance's string value
     */
    const std::string& getIdentifier() const;

    /**
     * @brief Returns column's alphabetical form of a instance. If not constructed, calls createAlphaValues().
     * 
     * @return `const std::string&` - column's string value 
     */
    const std::string& getColumnString() const;

    /**
     * @brief Converts decimal number to base-26 number.
     * 
     * @param decimal decimal number to convert
     * 
     * @return `std::string` - `decimal` in base-26
     */
    static std::string decToBase26(size_t decimal);

    /**
     * @brief Converts base-26 number to decimal number.
     * 
     * @param hexavigesimal base-26 number to convert
     * 
     * @return `size_t` - `hexavigesimal` in base-10
     * 
     * @throw `std::invalid_argument` - if `hexavigesimal` is invalid base-26 number
     */
    static size_t base26ToDec(const std::string& hexavigesimal);
                                    // base26 == hexavigesimal https://en.wiktionary.org/wiki/hexavigesimal

    // mutable because it should save time not having to create strings from numbers
    // for example in a additive loop
    // only create it when first needed
    mutable std::string identifier; // should be retrieved by string()
    mutable std::string columnString; // should be retrieved by columnAlpha()

    size_t columnIndex;
    size_t rowIndex;
};

using CPos = CellPosition; // for progtest
#endif
