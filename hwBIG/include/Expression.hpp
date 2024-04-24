#ifndef EXPRESSION_BUILDER_INCLUDED
#define EXPRESSION_BUILDER_INCLUDED

#include "../parser/expression.h"
#include "CellPosition.hpp"

#include <string>

struct ExpressionBuilder : public CExprBuilder
{
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
    void valRange(std::string val) override;

    void funcCall(std::string fnName, int paramCount) override;

private:
    // todo
};

struct Expression
{
    // todo
};
#endif
