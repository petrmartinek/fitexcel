#ifndef EXPRESSION_BUILDER_INCLUDED
#define EXPRESSION_BUILDER_INCLUDED

#include "../parser/expression.h"
#include "CellPosition.hpp"
#include "AST.hpp"
#include "CellValue.hpp"

#include <string>
#include <memory>
#include <stack>
#include <vector>
#include <map>
#include <set>

using Node_sp = std::shared_ptr<Node>;
using CellReferenceNode_sp = std::shared_ptr<CellReferenceNode>;

struct ExpressionBuilder : public CExprBuilder
{
    ExpressionBuilder() = default;

    explicit ExpressionBuilder(std::map<CellPosition, Node_sp> * table)
        : table(table)
    {}

    void opAdd() override;
    void opSub() override;
    void opMul() override;
    void opDiv() override;
    void opPow() override;
    void opNeg() override;
    void opEq() override;
    void opNe() override;
    void opLt() override;
    void opLe() override;
    void opGt() override;
    void opGe() override;

    void valNumber(double val) override;
    void valString(std::string val) override;
    void valReference(std::string val) override;
    void valRange(std::string val) override {}

    void funcCall(std::string fnName, int paramCount) override {}

    std::stack<Node_sp> waitingList;
    std::set<CellPosition> references;
    
    std::map<CellPosition, Node_sp> * table;
};

#endif
