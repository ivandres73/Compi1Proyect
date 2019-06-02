#include "parser.h"

void parser::parse() {
    tk = lex.getNextToken();
    program();
    if (tk != Token::EndFile)
        throw "something's wrong\n";
}

void parser::program() {
    subtypes_section();
    var_section();
    subprogram_decl();
}

void parser::subtypes_section() {
    if (tk == Token::KwTipo) {
        subtype_decl();
        subtypes_section();
    } else{
        /*epsilon*/
    }
}

void parser::subtype_decl() {
    if (tk == Token::KwTipo) {
        tk = lex.getNextToken();
        if (tk == Token::Iden) {
            tk = lex.getNextToken();
            if (tk == Token::KwEs) {
                tk = lex.getNextToken();
                type();
                if (tk == Token::EndLine)
                    tk = lex.getNextToken();
                else
                    syntaxError("end of line");
            } else
                syntaxError("ES");
        } else
            syntaxError("type");
    } else
            syntaxError("TIPO");    
}

void parser::var_section() {
    if (tokenIs(Token::KwEntero, Token::KwReal, Token::KwCadena, Token::KwBooleano, Token::KwCaracter)) {
        var_decl();
        var_section();
    } else {
        /*epsilon*/
    }
}

void parser::subprogram_decl() {
    if (tk == Token::KwFuncion) {//agregar despues los firsts de procedure_header
        subprogram_header();
        if (tk == Token::EndLine) {
            tk = lex.getNextToken();
            var_section();
            if (tk == Token::KwInicio) {
                tk = lex.getNextToken();
                statements();
                if (tk == Token::EndLine) {
                    tk = lex.getNextToken();
                    if (tk == Token::KwFin) {
                        tk = lex.getNextToken();
                        if (tk == Token::EndLine)
                            tk = lex.getNextToken();
                        else
                            syntaxError("end of line");
                    } else {
                        syntaxError("fin");
                    }
                } else
                    syntaxError("end of line");
            } else
                syntaxError("inicio");
        } else {
            cout << lex.toString(tk);
            cout << "-" << lex.getText();
            syntaxError("end of line");
        }
    } else {
        /*epsilon*/
    }
}

void parser::subprogram_header() {
    if (tk == Token::KwFuncion) {
        function_header();
    }//agregar else de procedure_header
}

void parser::function_header() {
    if (tk == Token::KwFuncion) {
        tk = lex.getNextToken();
        if (tk == Token::Iden) {
            tk = lex.getNextToken();
            argument_list();
            if (tk == Token::Colon) {
                tk = lex.getNextToken();
                type();
            }
        }
    }
}

void parser::argument_list() {
    if (tk == Token::OpenParens) {
        tk = lex.getNextToken();
        argument_decl();
        if (tk == Token::CloseParens) {
            cout << lex.toString(tk) << "consumido como parentesis\n";
            tk = lex.getNextToken();
        }
    } else {
        /*epsilon*/
    }
}

void parser::argument_decl() {
    if (tokenIs(Token::KwEntero, Token::KwReal, Token::KwCadena, Token::KwBooleano, Token::KwCaracter)) {
        type();
        cout << "reconoci un argumento\n";
        if (tk == Token::Iden) {
            lex.getNextToken();
            more_args();
        }
    } else if (tk == Token::KwVar) {
        tk = lex.getNextToken();
        type();
        if (tk == Token::Iden) {
            tk = lex.getNextToken();
            more_args();
        }
    } else {
        /*epsilon*/
    }
}

void parser::more_args() {
    if (tk == Token::Comma) {
        tk = lex.getNextToken();
        more_args_p();
    } else {
        /*epsilon*/
    }
}

void parser::more_args_p() {
    if (tokenIs(Token::KwEntero, Token::KwReal, Token::KwCadena, Token::KwBooleano, Token::KwCaracter)) {
        type();
        if (tk == Token::Iden) {
            tk = lex.getNextToken();
            more_args();
        }
    } else if (tk == Token::KwVar) {
        tk = lex.getNextToken();
        type();
        if (tk == Token::Iden) {
            tk = lex.getNextToken();
            more_args();
        }
    }
}

void parser::statements() {

}

void parser::var_decl() {
    if (tokenIs(Token::KwEntero, Token::KwReal, Token::KwCadena, Token::KwBooleano, Token::KwCaracter)) {
        type();
        if (tk == Token::Iden) {
            tk = lex.getNextToken();
            more_var();
            if (tk == Token::EndLine)
                tk = lex.getNextToken();
        } else
            syntaxError("identifier");
    } else
        syntaxError("type");
}

void parser::more_var() {
    if (tk == Token::Comma) {
        tk = lex.getNextToken();
        if (tk == Token::Iden) {
            tk = lex.getNextToken();
            more_var();
        } else
            syntaxError("identifier");
    } else {
        /* epsilon */
    }
}

void parser::type() {
    if (tk == Token::KwEntero)
        tk = lex.getNextToken();
    else if (tk == Token::KwReal)
        tk = lex.getNextToken();
    else if (tk == Token::KwCadena)
        tk = lex.getNextToken();
    else if (tk == Token::KwBooleano)
        tk = lex.getNextToken();
    else if (tk == Token::KwCaracter)
        tk = lex.getNextToken();
    else
        syntaxError("type");
}