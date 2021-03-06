#include "exprLex.h"

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
    void string_args();
    void more_string_args();
    void more_statements();
    void lvalue();
    void lvalue_p();
    void rvalue();
    void rvalue_p();
    void args_call();
    void arg_decl();
    void more_arg_decl();
    void expr();
    void is_op();
    void expr_p();
    void var_decl();
    void more_var();
    void type();
    void array_type();
    void constant();
    void bool_const();
    void bin_op();
    void arith_op();
    void rel_op();
    void eq_op();
    void cond_op();
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
