#ifndef expression_h_09845924528375
#define expression_h_09845924528375

#include <string>
class CExprBuilder {
public:
    virtual void opAdd() = 0;

    virtual void opSub() = 0;

    virtual void opMul() = 0;

    virtual void opDiv() = 0;

    virtual void opPow() = 0;

    virtual void opNeg() = 0;

    virtual void opEq() = 0;

    virtual void opNe() = 0;

    virtual void opLt() = 0;

    virtual void opLe() = 0;

    virtual void opGt() = 0;

    virtual void opGe() = 0;

    virtual void valNumber(double val) = 0;

    virtual void valString(std::string val) = 0;

    virtual void valReference(std::string val) = 0;

    virtual void valRange(std::string val) = 0;

    virtual void funcCall(std::string fnName,
                          int paramCount) = 0;
};

void parseExpression(std::string expr,
                     CExprBuilder &builder);

#endif /* expression_h_09845924528375 */


