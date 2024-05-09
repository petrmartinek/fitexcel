#ifndef AST_INCLUDED
#define AST_INCLUDED

#include "CellPosition.hpp"

#include <variant>
#include <string>
#include <memory>
#include <map>
#include <utility>

using namespace std::literals;

using CellValue = std::variant<std::monostate, double, std::string>;

struct Node
{
    Node() = default;
    virtual ~Node() = default;

    virtual std::shared_ptr<Node> clone() const = 0;
    virtual void updateReferences(const std::pair<int, int>& moveDistance) = 0;

    virtual CellValue evaluate() const = 0;

    virtual std::string toString() const = 0;
};

//------------------------------------------------------------------------------

struct CellValueNode : public Node
{
    explicit CellValueNode(CellValue value)
        : Node(),
          value(std::move(value))
    {}

    std::shared_ptr<Node> clone() const override { return std::make_shared<CellValueNode>(*this); }
    void updateReferences(const std::pair<int, int>& moveDistance) override {}

    CellValue evaluate() const override { return value; }

    std::string toString() const override;

private:
    CellValue value;
};

struct CellReferenceNode : public Node
{
    using Node_sp = std::shared_ptr<Node>;

    static inline const char ABS_SYMBOL = '$';
    enum RELATIVE {NONE, COLUMN, ROW, COLUMN_AND_ROW};

    CellReferenceNode(const CellPosition& referencePosition, std::map<CellPosition, Node_sp> * table, enum RELATIVE relativeVal)
        : relative(relativeVal), position(referencePosition), lookupTable(table)
    {}

    // expects the clone to be used inside its original spreadsheet 
    std::shared_ptr<Node> clone() const override { return std::make_shared<CellReferenceNode>(*this); }

    void updateReferences(const std::pair<int, int>& moveDistance) override;

    CellValue evaluate() const override { return (*lookupTable)[position]->evaluate(); }

    std::string toString() const override;

private:
    RELATIVE relative;
    CellPosition position;
    std::map<CellPosition, Node_sp> * lookupTable;
};

struct BinaryOperatorNode : public Node
{
    BinaryOperatorNode(std::shared_ptr<Node> first, std::shared_ptr<Node> second)
        : Node(),
          first(first), second(second)
    {}

    void updateReferences(const std::pair<int, int>& moveDistance) override { first->updateReferences(moveDistance); second->updateReferences(moveDistance); }

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

    void updateReferences(const std::pair<int, int>& moveDistance) override { first->updateReferences(moveDistance); }

protected:
    std::shared_ptr<Node> first;
};

//------------------------------------------------------------------------------

struct AdditionNode : public BinaryOperatorNode
{
    using BinaryOperatorNode::BinaryOperatorNode;

    std::shared_ptr<Node> clone() const override { return std::make_shared<AdditionNode>(AdditionNode{first->clone(), second->clone()}); }

    CellValue evaluate() const override;

    std::string toString() const override { return std::string("("s + first->toString() + " + "s + second->toString() + ")"s); }
};

struct NumberOperatorNode : public BinaryOperatorNode
{
    using BinaryOperatorNode::BinaryOperatorNode;

    CellValue evaluate() const override;

private:
    virtual CellValue evaluateNumberOperation(double lhs, double rhs) const = 0;
};

struct RelationOperatorNode : public BinaryOperatorNode
{
    using BinaryOperatorNode::BinaryOperatorNode;

    CellValue evaluate() const override;

private:
    virtual double numberRelation(double lhs, double rhs) const = 0;
    virtual double textRelation(const std::string& lhs, const std::string& rhs) const = 0;
};

//------------------------------------------------------------------------------

struct SubtractionNode : public NumberOperatorNode
{
    using NumberOperatorNode::NumberOperatorNode;

    std::shared_ptr<Node> clone() const override { return std::make_shared<SubtractionNode>(SubtractionNode{first->clone(), second->clone()}); }

    std::string toString() const override { return std::string("("s + first->toString() + " - "s + second->toString() + ")"s); }

private:
    CellValue evaluateNumberOperation(double lhs, double rhs) const override;
};

struct DivisionNode : public NumberOperatorNode
{
    using NumberOperatorNode::NumberOperatorNode;

    std::shared_ptr<Node> clone() const override { return std::make_shared<DivisionNode>(DivisionNode{first->clone(), second->clone()}); }

    std::string toString() const override { return std::string("("s + first->toString() + " / "s + second->toString() + ")"s); }

private:
    CellValue evaluateNumberOperation(double lhs, double rhs) const override;
};

struct MultiplicationNode : public NumberOperatorNode
{
    using NumberOperatorNode::NumberOperatorNode;

    std::shared_ptr<Node> clone() const override { return std::make_shared<MultiplicationNode>(MultiplicationNode{first->clone(), second->clone()}); }

    std::string toString() const override { return std::string("("s + first->toString() + " * "s + second->toString() + ")"s); }

private:
    CellValue evaluateNumberOperation(double lhs, double rhs) const override;
};

struct PowerToNode : public NumberOperatorNode
{
    using NumberOperatorNode::NumberOperatorNode;

    std::shared_ptr<Node> clone() const override { return std::make_shared<PowerToNode>(PowerToNode{first->clone(), second->clone()}); }

    std::string toString() const override { return std::string("("s + first->toString() + " ^ "s + second->toString() + ")"s); }

private:
    CellValue evaluateNumberOperation(double lhs, double rhs) const override;
};

//------------------------------------------------------------------------------

struct LessThanNode : public RelationOperatorNode
{    
    using RelationOperatorNode::RelationOperatorNode;

    std::shared_ptr<Node> clone() const override { return std::make_shared<LessThanNode>(LessThanNode{first->clone(), second->clone()}); }

    std::string toString() const override { return std::string("("s + first->toString() + " < "s + second->toString() + ")"s); }

private:
    double numberRelation(double lhs, double rhs) const override;
    double textRelation(const std::string& lhs, const std::string& rhs) const override;
};

struct LessOrEqualThanNode : public RelationOperatorNode
{
    using RelationOperatorNode::RelationOperatorNode;

    std::shared_ptr<Node> clone() const override { return std::make_shared<LessOrEqualThanNode>(LessOrEqualThanNode{first->clone(), second->clone()}); }

    std::string toString() const override { return std::string("("s + first->toString() + " <= "s + second->toString() + ")"s); }

private:
    double numberRelation(double lhs, double rhs) const override;
    double textRelation(const std::string& lhs, const std::string& rhs) const override;
};
struct GreaterThanNode : public RelationOperatorNode
{
    using RelationOperatorNode::RelationOperatorNode;

    std::shared_ptr<Node> clone() const override { return std::make_shared<GreaterThanNode>(GreaterThanNode{first->clone(), second->clone()}); }

    std::string toString() const override { return std::string("("s + first->toString() + " > "s + second->toString() + ")"s); }

private:
    double numberRelation(double lhs, double rhs) const override;
    double textRelation(const std::string& lhs, const std::string& rhs) const override;
};

struct GreaterOrEqualThanNode : public RelationOperatorNode
{
    using RelationOperatorNode::RelationOperatorNode;

    std::shared_ptr<Node> clone() const override { return std::make_shared<GreaterOrEqualThanNode>(GreaterOrEqualThanNode{first->clone(), second->clone()}); }

    std::string toString() const override { return std::string("("s + first->toString() + " >= "s + second->toString() + ")"s); }

private:
    double numberRelation(double lhs, double rhs) const override;
    double textRelation(const std::string& lhs, const std::string& rhs) const override;
};

struct EqualNode : public RelationOperatorNode
{
    using RelationOperatorNode::RelationOperatorNode;

    std::shared_ptr<Node> clone() const override { return std::make_shared<EqualNode>(EqualNode{first->clone(), second->clone()}); }

    std::string toString() const override { return std::string("("s + first->toString() + " == "s + second->toString() + ")"s); }

private:
    double numberRelation(double lhs, double rhs) const override;
    double textRelation(const std::string& lhs, const std::string& rhs) const override;
};
struct NotEqualNode : public RelationOperatorNode
{
    using RelationOperatorNode::RelationOperatorNode;

    std::shared_ptr<Node> clone() const override { return std::make_shared<NotEqualNode>(NotEqualNode{first->clone(), second->clone()}); }

    std::string toString() const override { return std::string("("s + first->toString() + " <> "s + second->toString() + ")"s); }

private:
    double numberRelation(double lhs, double rhs) const override;
    double textRelation(const std::string& lhs, const std::string& rhs) const override;
};

//------------------------------------------------------------------------------

struct NegationNode : public UnaryOperatorNode
{
    using UnaryOperatorNode::UnaryOperatorNode;

    std::shared_ptr<Node> clone() const override { return std::make_shared<NegationNode>(first->clone()); }

    CellValue evaluate() const override;

    std::string toString() const override { return std::string("(-"s + first->toString() + ")"s); }
};

//------------------------------------------------------------------------------

using CValue = CellValue;
#endif
