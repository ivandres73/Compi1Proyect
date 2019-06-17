#include <memory>

using std::move;

class ASTNode {
    public:
        
};

class Expr : public ASTNode{
    public:
        virtual int eval() = 0;
        virtual uint8_t getPrec() = 0;
        uint8_t prec;
};

using EXPRUP = std::unique_ptr<Expr>;

class BinExpr : public Expr {
    public:
        BinExpr(EXPRUP&& e1, EXPRUP&& e2, uint8_t pr) : expr1(move(e1)), expr2(move(e2)) { prec = pr; }

    protected:
        EXPRUP expr1;
        EXPRUP expr2;
};

#define DEFINE_BINEXPR(name, oper, prec) \
class name##Expr: public BinExpr { \
    public: \
        name##Expr(EXPRUP e1, EXPRUP e2) : BinExpr(move(e1), move(e2), prec) {} \
        int eval() override {  \
            if (expr1->getPrec() <= expr2->getPrec()) \
                return expr1->eval() + expr2->eval(); \
            return expr2->eval() + expr1->eval(); \
        } \
};

class AddExpr: public BinExpr {
    public:
        AddExpr(EXPRUP e1, EXPRUP e2) : BinExpr(move(e1), move(e2), 4) {}
        int eval() override;
};