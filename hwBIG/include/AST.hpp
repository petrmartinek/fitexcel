#ifndef AST_INCLUDED
#define AST_INCLUDED

#include <variant>
#include <string>

using CellValue = std::variant<std::monostate, double, std::string>;

struct Node
{
    // todo
};

//------------------------------------------------------------------------------

struct CellValueNode : public Node
{
    // todo
};

struct CellReferenceNode : public Node
{
    // todo
};

struct BinaryOperatorNode : public Node
{
    // todo
};

struct UnaryOperatorNode : public Node
{
    // todo
};

//------------------------------------------------------------------------------

struct AdditionNode : public BinaryOperatorNode
{
    // todo
};

struct SubtractionNode : public BinaryOperatorNode
{
    // todo
};

struct DivisionNode : public BinaryOperatorNode
{
    // todo
};

struct MultiplicationNode : public BinaryOperatorNode
{
    // todo
};

//------------------------------------------------------------------------------

struct NegationNode : public UnaryOperatorNode
{
    // todo
};

//------------------------------------------------------------------------------

struct AbsoluteCellReferenceNode : public CellReferenceNode
{
    // todo
};

struct RelativeCellReferenceNode : public CellReferenceNode
{
    // todo
};

//------------------------------------------------------------------------------

using CValue = CellValue;
#endif
