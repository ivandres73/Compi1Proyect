#include "exprLex.h"
#include "ast.h"

using std::stoi;
using std::make_shared;
// constantes
#define NumExpr(x) make_shared<NumExpr>(stoi(x));
#define CharExpr(x) make_shared<CharExpr>(stoi(x));
#define BoolExpr(x) make_shared<BoolExpr>(x);

//operadores de 1ra precedencia
#define NegExpr(x) make_shared<NegExpr>(x);
#define NotExpr(x) make_shared<NotExpr>(x);

//operadores de 2da precedencia
#define PowExpr(x, y) make_shared<PowExpr>(x, y);

//operadores de 3ra precedencia
#define MulExpr(x, y) make_shared<MulExpr>(x, y);
#define DivExpr(x, y) make_shared<DivExpr>(x, y);
#define ModExpr(x, y) make_shared<ModExpr>(x, y);
#define AndExpr(x, y) make_shared<AndExpr>(x, y);

//operadores de 4ta precedencia
#define AddExpr(x, y) make_shared<AddExpr>(x, y);
#define SubExpr(x, y) make_shared<SubExpr>(x, y);
#define OrExpr(x, y) make_shared<OrExpr>(x, y);

//operadores de 5ta precedencia (los de menos prioridad)
#define EQExpr(x, y) make_shared<EQExpr>(x, y);
#define NEQExpr(x, y) make_shared<NEQExpr>(x, y);
#define LTExpr(x, y) make_shared<LTExpr>(x, y);
#define GTExpr(x, y) make_shared<GTExpr>(x, y);
#define LTEExpr(x, y) make_shared<LTEExpr>(x, y);
#define GTEExpr(x, y) make_shared<GTEExpr>(x, y);

//Statements
#define WriteStmt(x) make_shared<WriteStmt>(x);

class parser
{
private:
    Token tk;
    exprLex& lex;

    void program();
    void subtypes_section();
    void subtype_decl();
    void var_section();
    void subprogram_decl();
    void subprogram_header();
    void function_header();
    void procedure_header();
    void argument_list();
    void argument_decl();
    void more_args();
    void more_args_p();
    void statements();
    void statement();
    void if_statement();
    void else_if_block();
    void else_if_block_p();
    void optional_eol();
    void string_args(vector<string>&);
    void more_string_args(vector<string>&);
    void more_statements();
    void lvalue();
    void lvalue_p();
    void rvalue();
    void rvalue_p();
    void args_call();
    void arg_decl();
    void more_arg_decl();
    EXPRSP expr0();
    EXPRSP expr1();
    EXPRSP expr2();
    EXPRSP expr3();
    EXPRSP expr4();
    EXPRSP expr5();
    void var_decl();
    void more_var();
    void type();
    void array_type();
    EXPRSP constant();
    EXPRSP bool_const();
    void fin();

    template <typename T, typename... Ts>
    bool tokenIs(T param1, Ts... otherParams) {
        return tk == param1 || tokenIs(otherParams...);
    }

    template <typename T>
    bool tokenIs(T param1) {
        return tk == param1;
    }

    void expect(Token t, const char* er) {
        if (tk == t)
            tk = lex.getNextToken();
        else
            syntaxError(er);
    }

    void syntaxError(const char* t) {
        cout << "\033[1;31msyntax error: \033[0m" << "expected a " << t << " in line "
             << lex.getLine() << " (found a " << lex.toString(tk) << ")" << endl;
    }

public:
    parser(exprLex& lex) : lex(lex) { }
    void parse();
};