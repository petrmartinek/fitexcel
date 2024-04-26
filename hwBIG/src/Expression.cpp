#include "../include/Expression.hpp"
#include "../parser/expression.h"
#include "../include/ExpressionBuilder.hpp"
#include "../include/AST.hpp"

#include <string>
#include <memory>

Node_sp Expression::createTree(const std::string& expression)
{
    ExpressionBuilder builder;

    parseExpression(expression, builder);

    return builder.waitingList.top(); // ? maybe add level of abstraction
}
