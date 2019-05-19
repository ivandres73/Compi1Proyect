#include "exprLex.h"

class parser
{
private:
    Token tk;
    exprLex& lex;

    void program();
    void var_section();
    void subprogram_decl();
    void statements();
    void var_decl();
    void more_var();
    void type();

public:
    parser(exprLex& lex) : lex(lex) { }
    void parse();
};