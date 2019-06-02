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
    void argument_list();
    void argument_decl();
    void more_args();
    void more_args_p();
    void procedure_header();
    void statements();
    void var_decl();
    void more_var();
    void type();
    void array_type();

    template <typename T, typename... Ts>
    bool tokenIs(T param1, Ts... otherParams) {
        return tk == param1 || tokenIs(otherParams...);
    }

    template <typename T>
    bool tokenIs(T param1) {
        return tk == param1;
    }

    void syntaxError(const char* t) {
        cout << "\033[1;31msyntax error: \033[0m" << "expected a " << t << " in line " << lex.getLine() << endl;
    }

public:
    parser(exprLex& lex) : lex(lex) { }
    void parse();
};