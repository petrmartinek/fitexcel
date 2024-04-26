#include "../include/ExpressionBuilder.hpp"
#include "../include/AST.hpp"

#include <string>
#include <memory>
#include <stack>
#include <iostream>

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
    waitingList.push(Node_sp(new CellValueNode{CellValue{val}}));
}
