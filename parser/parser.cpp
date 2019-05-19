#include "parser.h"

void parser::parse() {
    tk = lex.getNextToken();
    program();
    if (tk != Token::EndFile) {
        throw "something's wrong\n";
    }
}

void parser::program() {

}

void parser::var_section() {

}

void parser::subprogram_decl() {

}

void parser::statements() {

}

void parser::var_decl() {

}

void parser::more_var() {

}

void parser::type() {

}