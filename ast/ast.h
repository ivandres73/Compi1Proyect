#include <memory>

using std::move;

class ASTNode {
    public:
        
};

class Expr : public ASTNode{
    public:
        virtual int eval() = 0;
        virtual int8_t getPrec() = 0;

    protected:
        int8_t prec;
};

using EXPRSP = std::shared_ptr<Expr>;

class BinExpr : public Expr {
    public:
        BinExpr(EXPRSP&& e1, EXPRSP&& e2, int8_t pr) : expr1(move(e1)), expr2(move(e2)) { prec = pr; }

    protected:
        EXPRSP expr1;
        EXPRSP expr2;
};

/* Expresiones Binarias (sumas, restas...) */

#define DEFINE_BINEXPR(name, oper, prec) \
class name##Expr: public BinExpr { \
    public: \
        name##Expr(EXPRSP e1, EXPRSP e2) : BinExpr(move(e1), move(e2), prec) {} \
        int eval() override {  \
            if (expr1->getPrec() <= expr2->getPrec()) \
                return expr1->eval() + expr2->eval(); \
            return expr2->eval() + expr1->eval(); \
        } \
        int8_t getPrec() override { return prec; } \
};

DEFINE_BINEXPR(Add, +, 4);
DEFINE_BINEXPR(Sub, -, 4);
DEFINE_BINEXPR(Or, ||, 4);
DEFINE_BINEXPR(Mul, *, 3);
DEFINE_BINEXPR(Div, /, 3);
DEFINE_BINEXPR(Mod, %, 3);
DEFINE_BINEXPR(And, &&, 3);

/* Expresiones de Constantes
* las constantes son las expresiones que tienen la
* mas alta prioridad (-1)
*/

#define DEFINE_CONSTEXPR(name, type)                     \
class name##Expr : public Expr {                         \
    public:                                              \
        name##Expr(type val) : value(val) { prec = -1; } \
        int eval() override { return value; }            \
        int8_t getPrec() override { return prec; }       \
                                                         \
    private:                                             \
        type value;                                      \
};

DEFINE_CONSTEXPR(Num, int);
DEFINE_CONSTEXPR(Char, char);
DEFINE_CONSTEXPR(Bool, bool);