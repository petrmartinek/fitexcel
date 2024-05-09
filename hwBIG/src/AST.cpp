#include "../include/AST.hpp"

#include <variant>
#include <string>
#include <memory>
#include <cmath>
#include <utility>

using namespace std::literals;

std::string CellValueNode::toString() const
{
    if(std::holds_alternative<double>(value))
    {
        return std::to_string(std::get<double>(value));
    }

    if(std::holds_alternative<std::string>(value))
    {
        std::string outputString;
        // trying to make this mess little effective
        outputString.reserve(std::get<std::string>(value).size());

        const std::string& inputString = std::get<std::string>(value);
        for(const auto& c : inputString)
        {
            if(c != '"')
            {
                outputString += c;
                continue;
            }

            // c == "
            outputString += "\"\""s; // "quotes must be doubled: \"\" "
        }

        return "\""s + outputString + "\""s;
    }

    return "";
}

//------------------------------------------------------------------------------

void CellReferenceNode::updateReferences(const std::pair<int, int>& moveDistance)
{
    if(relative == NONE)
    {
        return;
    }

    size_t newColumn = position.column();
    size_t newRow = position.row();

    if(relative == COLUMN_AND_ROW || relative == COLUMN)
    {
        newColumn += moveDistance.first;
    }

    if(relative == COLUMN_AND_ROW || relative == ROW)
    {
        newRow += moveDistance.second;
    }

    position = CellPosition{newColumn, newRow};
}

CellValue CellReferenceNode::evaluate() const
{
    if(!lookupTable->contains(position))
    {
        return CellValue();
    }

    return (*lookupTable)[position]->evaluate();
}

std::string CellReferenceNode::toString() const
{
    std::string output;

    if(relative != COLUMN_AND_ROW && relative != COLUMN)
    {
        output += ABS_SYMBOL;
    }

    output += position.columnAlpha();

    if(relative != COLUMN_AND_ROW && relative != ROW)
    {
        output += ABS_SYMBOL;
    }

    output += std::to_string(position.row());

    return output;
}

//------------------------------------------------------------------------------

CellValue AdditionNode::evaluate() const
{
    CellValue lhs = first->evaluate(),
              rhs = second->evaluate();
    
    if(std::holds_alternative<std::monostate>(lhs) || std::holds_alternative<std::monostate>(rhs))
    {
        return CellValue();
    }

    if(std::holds_alternative<double>(lhs) && std::holds_alternative<double>(rhs))
    {
        return std::get<double>(lhs) + std::get<double>(rhs);
    }

    // not pretty
    std::string lhsString = std::holds_alternative<double>(lhs) ? std::to_string(std::get<double>(lhs)) : std::get<std::string>(lhs),
                rhsString = std::holds_alternative<double>(rhs) ? std::to_string(std::get<double>(rhs)) : std::get<std::string>(rhs);
    
    return lhsString + rhsString;
}

//------------------------------------------------------------------------------

CellValue NumberOperatorNode::evaluate() const
{
    CellValue lhs = first->evaluate(),
              rhs = second->evaluate();

    if(!std::holds_alternative<double>(lhs) || !std::holds_alternative<double>(rhs))
    {
        return CellValue();
    }

    return evaluateNumberOperation(std::get<double>(lhs), std::get<double>(rhs));
}

//------------------------------------------------------------------------------

CellValue SubtractionNode::evaluateNumberOperation(double lhs, double rhs) const
{
    return lhs - rhs;
}

//------------------------------------------------------------------------------

CellValue DivisionNode::evaluateNumberOperation(double lhs, double rhs) const
{
    if(rhs == 0)
    {
        return CellValue();
    }

    return lhs / rhs;
}

//------------------------------------------------------------------------------

CellValue MultiplicationNode::evaluateNumberOperation(double lhs, double rhs) const
{
    return lhs * rhs;
}

//------------------------------------------------------------------------------

CellValue PowerToNode::evaluateNumberOperation(double lhs, double rhs) const
{
    return std::pow(lhs, rhs);
}

//------------------------------------------------------------------------------

CellValue NegationNode::evaluate() const
{
    CellValue result = first->evaluate();

    if(!std::holds_alternative<double>(result))
    {
        return CellValue();
    }

    return std::get<double>(result) * (-1);
}

//------------------------------------------------------------------------------

CellValue RelationOperatorNode::evaluate() const
{
    CellValue lhs = first->evaluate(),
              rhs = second->evaluate();

    if(std::holds_alternative<double>(lhs) && std::holds_alternative<double>(rhs))
    {
        return numberRelation(std::get<double>(lhs), std::get<double>(rhs));
    }

    if(std::holds_alternative<std::string>(lhs) && std::holds_alternative<std::string>(rhs))
    {
        return textRelation(std::get<std::string>(lhs), std::get<std::string>(rhs));
    }

    return CellValue();
}

//------------------------------------------------------------------------------

double LessThanNode::numberRelation(double lhs, double rhs) const
{
    return lhs < rhs;
}

double LessThanNode::textRelation(const std::string& lhs, const std::string& rhs) const
{
    return lhs < rhs;
}

//------------------------------------------------------------------------------

double LessOrEqualThanNode::numberRelation(double lhs, double rhs) const
{
    return lhs <= rhs;
}

double LessOrEqualThanNode::textRelation(const std::string& lhs, const std::string& rhs) const
{
    return lhs <= rhs;
}

//------------------------------------------------------------------------------

double GreaterThanNode::numberRelation(double lhs, double rhs) const
{
    return lhs > rhs;
}

double GreaterThanNode::textRelation(const std::string& lhs, const std::string& rhs) const
{
    return lhs > rhs;
}

//------------------------------------------------------------------------------

double GreaterOrEqualThanNode::numberRelation(double lhs, double rhs) const
{
    return lhs >= rhs;
}

double GreaterOrEqualThanNode::textRelation(const std::string& lhs, const std::string& rhs) const
{
    return lhs >= rhs;
}

//------------------------------------------------------------------------------

double EqualNode::numberRelation(double lhs, double rhs) const
{
    return lhs == rhs;
}

double EqualNode::textRelation(const std::string& lhs, const std::string& rhs) const
{
    return lhs == rhs;
}

//------------------------------------------------------------------------------

double NotEqualNode::numberRelation(double lhs, double rhs) const
{
    return lhs != rhs;
}

double NotEqualNode::textRelation(const std::string& lhs, const std::string& rhs) const
{
    return lhs != rhs;
}

//------------------------------------------------------------------------------
