#include <memory>
#include <string>
#include <math.h>
#include <vector>
#include <iostream>
#include <unordered_map>

using std::move;
using std::to_string;
using std::string;
using std::vector;
using std::cout;
using std::shared_ptr;
using std::unordered_map;

//valor random para saber si variable fue inicializada
#define UNINITIALIZED -7337

// Struct que guardara el valor de las variables
struct Context {
    unordered_map<string, int> vars;
};

//Clase nodo (padre de todos)
class ASTNode {
    public:
        virtual string toString() = 0;
};

class Expr : public ASTNode {
    public:
        virtual int eval(Context&) = 0;
        int8_t getPrec() { return prec; }

    protected:
        int8_t prec;
};

using EXPRSP = shared_ptr<Expr>;

class BinExpr : public Expr {
    public:
        BinExpr(EXPRSP&&, EXPRSP&&, int8_t);

    protected:
        EXPRSP expr1;
        EXPRSP expr2;
};

/* Expresiones Binarias (sumas, restas...) */

#define DEFINE_BINEXPR(name, oper, prec) \
class name##Expr : public BinExpr { \
    public: \
        name##Expr(EXPRSP e1, EXPRSP e2) : BinExpr(move(e1), move(e2), prec) {} \
        int eval(Context& ctx) override {  \
            if (expr1->getPrec() <= expr2->getPrec()) \
                return expr1->eval(ctx) oper expr2->eval(ctx); \
            return expr2->eval(ctx) oper expr1->eval(ctx); \
        } \
        string toString() override { \
            return expr1->toString() + #oper + expr2->toString(); \
        } \
};

class PowExpr : public BinExpr {
    public:
        PowExpr(EXPRSP, EXPRSP);
        int eval(Context&) override;
        string toString() override;
};

class NegExpr : public Expr {
    public:
        NegExpr(EXPRSP);
        int eval(Context&) override;
        string toString() override;

    private:
        EXPRSP expr;
};

class NotExpr : public Expr {
    public:
        NotExpr(EXPRSP);
        int eval(Context&) override;
        string toString() override;

    private:
        EXPRSP expr;
};

DEFINE_BINEXPR(EQ, ==, 5);
DEFINE_BINEXPR(NEQ, !=, 5);
DEFINE_BINEXPR(LT, <, 5);
DEFINE_BINEXPR(GT, >, 5);
DEFINE_BINEXPR(LTE, <=, 5);
DEFINE_BINEXPR(GTE, >=, 5);
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
        int eval(Context&) override { return value; }            \
        string toString() override {                     \
            return to_string(value);                     \
        }                                                \
        type getValue() { return value; }                \
    private:                                             \
        type value;                                      \
};

class IdenExpr : public Expr {
    public:
        IdenExpr(string);
        int eval(Context&) override;
        string toString() override;

    private:
        string id;
};

DEFINE_CONSTEXPR(Num, int);
DEFINE_CONSTEXPR(Char, char);
DEFINE_CONSTEXPR(Bool, bool);

class Statement : public ASTNode {
    public:
        virtual void exec(Context&) = 0;
};

using STMTSP = shared_ptr<Statement>;
using STMTS = vector<Statement>;

class WriteStmt : public Statement {
    public:
        WriteStmt(vector<string>);
        void exec(Context&) override;
        string toString() override;

    private:
        vector<string> args;
};

class DeclareStmt : public Statement {
    public:
        DeclareStmt(vector<string>);
        void exec(Context&) override;
        string toString() override;

    private:
        vector<string> ids;
};

class AssignStmt : public Statement {
    public:
        AssignStmt(string, EXPRSP);
        void exec(Context&) override;
        string toString() override;
    
    private:
        string id;
        EXPRSP val;
};