#include "../include/AST.hpp"

#include <variant>
#include <string>
#include <memory>
#include <cmath>

std::string CellValueNode::toString() const
{
    if(std::holds_alternative<double>(value))
    {
        return std::to_string(std::get<double>(value));
    }

    if(std::holds_alternative<std::string>(value))
    {
        return "\"" + std::get<std::string>(value) + "\"";
    }

    return "";
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
