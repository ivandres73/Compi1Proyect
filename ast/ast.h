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

// Structs que guardara el valor de las variables
struct entry {
    string tipo;
    int value;
};
struct Context {
    unordered_map<string, entry> vars;
    unordered_map<string, vector<entry>> arreglos;
    unordered_map<string, int> size_arreglos;
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
                return expr1->eval(ctx) oper expr2->eval(ctx); \
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
        name##Expr(type val) : value(val) {              \
            prec = -1;                                   \
        }                                                \
        int eval(Context&) override { return value; }    \
        string toString() override {                     \
            return to_string(value);                     \
        }                                                \
        type getValue() { return value; }                \
    private:                                             \
        type value;                                      \
};

class IdenExpr : public Expr {
    public:
        IdenExpr(string, EXPRSP);
        int eval(Context&) override;
        string toString() override;
        string getType(Context&);
    private:
        string id;
        EXPRSP array_expr;//pos del array declarado
};

DEFINE_CONSTEXPR(Num, int);
DEFINE_CONSTEXPR(Char, char);
DEFINE_CONSTEXPR(Bool, bool);

class Statement : public ASTNode {
    public:
        virtual void exec(Context&) = 0;
};

using STMTSP = shared_ptr<Statement>;
using STMTS = vector<STMTSP>;

class WriteStmt : public Statement {
    public:
        WriteStmt(vector<string>&, vector<EXPRSP>&);
        void exec(Context&) override;
        string toString() override;

    private:
        vector<string> exprs_type;
        vector<EXPRSP> exprs;
};

class DeclareStmt : public Statement {
    public:
        DeclareStmt(vector<string>&, string);
        void exec(Context&) override;
        string toString() override;

    private:
        vector<string> ids;
        string tipo;
};

class AssignStmt : public Statement {
    public:
        AssignStmt(string, EXPRSP);
        void exec(Context&) override;
        string toString() override;
        string getId() { return id; }
    
    private:
        string id;
        EXPRSP val;
};

class WhileStmt : public Statement {
    public:
        WhileStmt(EXPRSP, STMTS&);
        void exec(Context&) override;
        string toString() override;

    private:
        EXPRSP expr;
        STMTS list;
};

class IfStmt : public Statement {
    public:
        IfStmt(EXPRSP, STMTS&, vector<EXPRSP>&, vector<STMTS>&,
               STMTS&);
        void exec(Context&) override;
        string toString() override;

    private:
        EXPRSP expr_if;
        STMTS stmts_if;
        vector<EXPRSP> expr_elsif;
        vector<STMTS> stmts_elsif;
        STMTS stmts_else;
};

class ForStmt : public Statement {
    public:
        ForStmt(shared_ptr<AssignStmt>&, EXPRSP, STMTS&);
        void exec(Context&) override;
        string toString() override;

    private:
        shared_ptr<AssignStmt> inicio;
        EXPRSP breque;
        STMTS stmts;
};

class DoWhileStmt : public Statement {
    public:
        DoWhileStmt(EXPRSP, STMTS&);
        void exec(Context&) override;
        string toString() override;

    private:
        EXPRSP expr;
        STMTS lista;
};