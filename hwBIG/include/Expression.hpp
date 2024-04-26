#ifndef EXPRESSION_INCLUDED
#define EXPRESSION_INCLUDED

#include "../parser/expression.h"
#include "ExpressionBuilder.hpp"
#include "AST.hpp"

#include <string>
#include <memory>

using Node_sp = std::shared_ptr<Node>;

struct Expression
{
    Expression()
        : start(Node_sp(new CellValueNode{{}}))
    {}
    explicit Expression(Node_sp node)
        : start(node)
    {}
    explicit Expression(const std::string& expression)
        : start(createTree(expression))
    {}

    CellValue evaluate() const { return start->evaluate(); }

private:
    Node_sp start;

    static Node_sp createTree(const std::string& expression);
};
#endif
