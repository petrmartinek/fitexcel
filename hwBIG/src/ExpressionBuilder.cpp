#include "../include/ExpressionBuilder.hpp"
#include "../include/AST.hpp"
#include "../include/CellValue.hpp"

#include <string>
#include <memory>
#include <stack>
#include <iostream>
#include <vector>
#include <map>

void ExpressionBuilder::opAdd()
{
    Node_sp rhs = waitingList.top();
    waitingList.pop();
    Node_sp lhs = waitingList.top();
    waitingList.pop();

    waitingList.push(Node_sp(new AdditionNode{lhs, rhs}));
}

void ExpressionBuilder::opSub()
{
    Node_sp rhs = waitingList.top();
    waitingList.pop();
    Node_sp lhs = waitingList.top();
    waitingList.pop();

    waitingList.push(Node_sp(new SubtractionNode{lhs, rhs}));
}

void ExpressionBuilder::opMul()
{
    Node_sp rhs = waitingList.top();
    waitingList.pop();
    Node_sp lhs = waitingList.top();
    waitingList.pop();

    waitingList.push(Node_sp(new MultiplicationNode{lhs, rhs}));
}

void ExpressionBuilder::opDiv()
{
    Node_sp rhs = waitingList.top();
    waitingList.pop();
    Node_sp lhs = waitingList.top();
    waitingList.pop();

    waitingList.push(Node_sp(new DivisionNode{lhs, rhs}));
}

void ExpressionBuilder::opPow()
{
    Node_sp rhs = waitingList.top();
    waitingList.pop();
    Node_sp lhs = waitingList.top();
    waitingList.pop();

    waitingList.push(Node_sp(new PowerToNode{lhs, rhs}));
}

void ExpressionBuilder::opNeg()
{
    Node_sp val = waitingList.top();
    waitingList.pop();

    waitingList.push(Node_sp(new NegationNode{val}));
}

void ExpressionBuilder::opEq()
{
    Node_sp rhs = waitingList.top();
    waitingList.pop();
    Node_sp lhs = waitingList.top();
    waitingList.pop();

    waitingList.push(Node_sp(new EqualNode{lhs, rhs}));
}

void ExpressionBuilder::opNe()
{
    Node_sp rhs = waitingList.top();
    waitingList.pop();
    Node_sp lhs = waitingList.top();
    waitingList.pop();

    waitingList.push(Node_sp(new NotEqualNode{lhs, rhs}));
}

void ExpressionBuilder::opLt()
{
    Node_sp rhs = waitingList.top();
    waitingList.pop();
    Node_sp lhs = waitingList.top();
    waitingList.pop();

    waitingList.push(Node_sp(new LessThanNode{lhs, rhs}));
}

void ExpressionBuilder::opLe()
{
    Node_sp rhs = waitingList.top();
    waitingList.pop();
    Node_sp lhs = waitingList.top();
    waitingList.pop();

    waitingList.push(Node_sp(new LessOrEqualThanNode{lhs, rhs}));
}

void ExpressionBuilder::opGt()
{
    Node_sp rhs = waitingList.top();
    waitingList.pop();
    Node_sp lhs = waitingList.top();
    waitingList.pop();

    waitingList.push(Node_sp(new GreaterThanNode{lhs, rhs}));
}

void ExpressionBuilder::opGe()
{
    Node_sp rhs = waitingList.top();
    waitingList.pop();
    Node_sp lhs = waitingList.top();
    waitingList.pop();

    waitingList.push(Node_sp(new GreaterOrEqualThanNode{lhs, rhs}));
}

//------------------------------------------------------------------------------

void ExpressionBuilder::valNumber(double val)
{
    waitingList.push(Node_sp(new CellValueNode{CellValue{val}}));
}

void ExpressionBuilder::valString(std::string val)
{
    waitingList.push(Node_sp(new CellValueNode{CellValue{std::move(val)}}));
}

//------------------------------------------------------------------------------

void ExpressionBuilder::valReference(std::string val)
{    
    using enum CellReferenceNode::RELATIVE;

    int numberOfAbsoluteSymbols = std::count(val.begin(), val.end(), CellReferenceNode::ABS_SYMBOL);

    std::string valWithoutAbsoluteSymbols;
    std::copy_if(val.begin(), val.end(), std::back_inserter<std::string>(valWithoutAbsoluteSymbols),
        [](const char& c)
        {
            return c != CellReferenceNode::ABS_SYMBOL;
        }
    );

    CellPosition position{valWithoutAbsoluteSymbols};

    references.insert(position);

    // purely relative reference
    if(!numberOfAbsoluteSymbols)
    {
        waitingList.push(CellReferenceNode_sp{new CellReferenceNode{position, table, COLUMN_AND_ROW}});

        return;
    }

    // fully absolute reference
    if(numberOfAbsoluteSymbols == 2)
    {
        waitingList.push(CellReferenceNode_sp{new CellReferenceNode{position, table, NONE}});

        return;
    }
    
    // row or column is relative

    CellReferenceNode::RELATIVE type = (val.front() == CellReferenceNode::ABS_SYMBOL) ? ROW : COLUMN;  
    
    waitingList.push(CellReferenceNode_sp{new CellReferenceNode{position, table, type}});
}
