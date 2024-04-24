#ifndef AST_INCLUDED
#define AST_INCLUDED

#include <variant>
#include <string>
#include <memory>

using CellValue = std::variant<std::monostate, double, std::string>;

struct Node
{
    Node() = default;
    virtual ~Node() = default;

    virtual CellValue evaluate() const = 0;
};

//------------------------------------------------------------------------------

struct CellValueNode : public Node
{
    explicit CellValueNode(CellValue value)
        : Node(),
          value(std::move(value))
    {}

    CellValue evaluate() const override { return value; }

private:
    CellValue value;
};

struct CellReferenceNode : public Node
{
    // todo
};

struct BinaryOperatorNode : public Node
{
    BinaryOperatorNode(std::shared_ptr<Node> first, std::shared_ptr<Node> second)
        : Node(),
          first(first), second(second)
    {}

protected:
    std::shared_ptr<Node> first;
    std::shared_ptr<Node> second;
};

struct UnaryOperatorNode : public Node
{
    explicit UnaryOperatorNode(std::shared_ptr<Node> first)
        : Node(),
          first(first)
    {}

protected:
    std::shared_ptr<Node> first;
};

//------------------------------------------------------------------------------

struct AdditionNode : public BinaryOperatorNode
{
    using BinaryOperatorNode::BinaryOperatorNode;

    CellValue evaluate() const override;
};

struct SubtractionNode : public BinaryOperatorNode
{
    using BinaryOperatorNode::BinaryOperatorNode;

    CellValue evaluate() const override;
};

struct DivisionNode : public BinaryOperatorNode
{
    using BinaryOperatorNode::BinaryOperatorNode;

    CellValue evaluate() const override;
};

struct MultiplicationNode : public BinaryOperatorNode
{
    using BinaryOperatorNode::BinaryOperatorNode;

    CellValue evaluate() const override;
};

struct LessThanNode : public BinaryOperatorNode
{    
    // todo
};

struct LessOrEqualThanNode : public BinaryOperatorNode
{
    // todo
};

struct BiggerThanNode : public BinaryOperatorNode
{
    // todo
};

struct BiggerOrEqualThanNode : public BinaryOperatorNode
{
    // todo
};

struct EqualNode : public BinaryOperatorNode
{
    // todo
};

struct NotEqualNode : public BinaryOperatorNode
{
    // todo
};

//------------------------------------------------------------------------------

struct NegationNode : public UnaryOperatorNode
{
    CellValue evaluate() const override;
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
