#include "../include/AST.hpp"

#include <variant>
#include <string>
#include <memory>

/* 
    ? Maybe overload operators for CellValue would be nice?
*/


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

CellValue SubtractionNode::evaluate() const
{
    CellValue lhs = first->evaluate(),
              rhs = second->evaluate();

    if(!std::holds_alternative<double>(lhs) || !std::holds_alternative<double>(rhs))
    {
        return CellValue();
    }

    return std::get<double>(lhs) - std::get<double>(rhs);
}

//------------------------------------------------------------------------------

CellValue DivisionNode::evaluate() const
{
    CellValue lhs = first->evaluate(),
              rhs = second->evaluate();

    if
    (
        !std::holds_alternative<double>(lhs) || !std::holds_alternative<double>(rhs)
    ||  std::get<double>(rhs) == 0
    )
    {
        return CellValue();
    }

    return std::get<double>(lhs) / std::get<double>(rhs);
}

//------------------------------------------------------------------------------

CellValue MultiplicationNode::evaluate() const
{
    CellValue lhs = first->evaluate(),
              rhs = second->evaluate();

    if(!std::holds_alternative<double>(lhs) || !std::holds_alternative<double>(rhs))
    {
        return CellValue();
    }

    return std::get<double>(lhs) * std::get<double>(rhs);
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

CellValue LessThanNode::evaluate() const
{
    CellValue lhs = first->evaluate(),
              rhs = second->evaluate();

    if(std::holds_alternative<double>(lhs) && std::holds_alternative<double>(rhs))
    {
        return std::get<double>(lhs) < std::get<double>(rhs) ? 1. : 0.;
    }

    if(std::holds_alternative<std::string>(lhs) && std::holds_alternative<std::string>(rhs))
    {
        return std::get<std::string>(lhs) < std::get<std::string>(rhs) ? 1. : 0.;
    }

    return CellValue();
}

//------------------------------------------------------------------------------

CellValue LessOrEqualThanNode::evaluate() const
{
    CellValue lhs = first->evaluate(),
              rhs = second->evaluate();

    if(std::holds_alternative<double>(lhs) && std::holds_alternative<double>(rhs))
    {
        return std::get<double>(lhs) <= std::get<double>(rhs) ? 1. : 0.;
    }

    if(std::holds_alternative<std::string>(lhs) && std::holds_alternative<std::string>(rhs))
    {
        return std::get<std::string>(lhs) <= std::get<std::string>(rhs) ? 1. : 0.;
    }

    return CellValue();
}

//------------------------------------------------------------------------------

CellValue BiggerThanNode::evaluate() const
{
    CellValue lhs = first->evaluate(),
              rhs = second->evaluate();

    if(std::holds_alternative<double>(lhs) && std::holds_alternative<double>(rhs))
    {
        return std::get<double>(lhs) > std::get<double>(rhs) ? 1. : 0.;
    }

    if(std::holds_alternative<std::string>(lhs) && std::holds_alternative<std::string>(rhs))
    {
        return std::get<std::string>(lhs) > std::get<std::string>(rhs) ? 1. : 0.;
    }

    return CellValue();
}

//------------------------------------------------------------------------------

CellValue BiggerOrEqualThanNode::evaluate() const
{
    CellValue lhs = first->evaluate(),
              rhs = second->evaluate();

    if(std::holds_alternative<double>(lhs) && std::holds_alternative<double>(rhs))
    {
        return std::get<double>(lhs) >= std::get<double>(rhs) ? 1. : 0.;
    }

    if(std::holds_alternative<std::string>(lhs) && std::holds_alternative<std::string>(rhs))
    {
        return std::get<std::string>(lhs) >= std::get<std::string>(rhs) ? 1. : 0.;
    }

    return CellValue();
}

//------------------------------------------------------------------------------

CellValue EqualNode::evaluate() const
{
    CellValue lhs = first->evaluate(),
              rhs = second->evaluate();

    if(std::holds_alternative<double>(lhs) && std::holds_alternative<double>(rhs))
    {
        return std::get<double>(lhs) == std::get<double>(rhs) ? 1. : 0.;
    }

    if(std::holds_alternative<std::string>(lhs) && std::holds_alternative<std::string>(rhs))
    {
        return std::get<std::string>(lhs) == std::get<std::string>(rhs) ? 1. : 0.;
    }

    return CellValue();
}

//------------------------------------------------------------------------------

CellValue NotEqualNode::evaluate() const
{
    CellValue lhs = first->evaluate(),
              rhs = second->evaluate();

    if(std::holds_alternative<double>(lhs) && std::holds_alternative<double>(rhs))
    {
        return std::get<double>(lhs) != std::get<double>(rhs) ? 1. : 0.;
    }

    if(std::holds_alternative<std::string>(lhs) && std::holds_alternative<std::string>(rhs))
    {
        return std::get<std::string>(lhs) != std::get<std::string>(rhs) ? 1. : 0.;
    }

    return CellValue();
}

//------------------------------------------------------------------------------
