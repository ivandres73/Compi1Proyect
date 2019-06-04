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
    expect(Token::KwInicio, "inicio");
    optional_eol();
    statements();
    expect(Token::KwFin, "fin");
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
    expect(Token::KwTipo, "TIPO");
    expect(Token::Iden, "identifier");
    expect(Token::KwEs, "ES");
    type();
    expect(Token::EndLine, "end of line");
}

void parser::var_section() {
    if (tokenIs(Token::KwEntero, Token::KwReal, Token::KwCadena,
                Token::KwBooleano, Token::KwCaracter)) {
        var_decl();
        var_section();
    } else {
        /*epsilon*/
    }
}

void parser::subprogram_decl() {
    if (tokenIs(Token::KwFuncion, Token::KwProcedimiento)) {
        subprogram_header();
        expect(Token::EndLine, "end of line");
        var_section();
        expect(Token::KwInicio, "inicio");
        expect(Token::EndLine, "end of line");
        statements();
        expect(Token::KwFin, "fin");
        expect(Token::EndLine, "end of line");
        subprogram_decl();
    } else {
        /*epsilon*/
    }
}

void parser::subprogram_header() {
    if (tk == Token::KwFuncion)
        function_header();
    else if (tk == Token::KwProcedimiento)
        procedure_header();
    else
        syntaxError("funcion o procedimiento");
}

void parser::function_header() {
    expect(Token::KwFuncion, "funcion");
    expect(Token::Iden, "identifier");    
    argument_list();
    expect(Token::Colon, "colon");
    type();
        
}

void parser::procedure_header() {
    expect(Token::KwProcedimiento, "procedimiento");
    expect(Token::Iden, "identifier");
    argument_list();
}

void parser::argument_list() {
    expect(Token::OpenParens, "open parens");
    argument_decl();
    expect(Token::CloseParens, "close parens");
}

void parser::argument_decl() {
    if (tokenIs(Token::KwEntero, Token::KwReal, Token::KwCadena,
                Token::KwBooleano, Token::KwCaracter)) {
        type();
        expect(Token::Iden, "identifier");
        more_args();
    } else if (tk == Token::KwVar) {
        tk = lex.getNextToken();
        type();
        expect(Token::Iden, "identifier");
        more_args();
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
    if (tokenIs(Token::KwEntero, Token::KwReal, Token::KwCadena,
                Token::KwBooleano, Token::KwCaracter)) {
        type();
        expect(Token::Iden, "identifier");
        more_args();
    } else if (tk == Token::KwVar) {
        tk = lex.getNextToken();
        type();
        expect(Token::Iden, "identifier");
        more_args();
    } else
        syntaxError("type");
}

void parser::statements() {
    if (tokenIs(Token::Iden, Token::KwLlamar, Token::KwEscriba,
                Token::KwLea, Token::KwSi, Token::KwMientras,
                Token::KwRepita, Token::KwPara, Token::KwRetorne))
        statement();
    else {
        /*epsilon*/
    }   
}

void parser::statement() {
    if (tk == Token::Iden) {
        lvalue();
        expect(Token::Assign, "assign");
            expr();
            more_statements();
    } else if (tk == Token::KwLlamar) {
        tk = lex.getNextToken();
        expect(Token::Iden, "identifier");
            args_call();
            more_statements();
    } else if (tk == Token::KwEscriba) {
        tk = lex.getNextToken();
        string_args();
        more_statements();
    } else if (tk == Token::KwLea) {
        tk = lex.getNextToken();
        lvalue();
        more_statements();
    } else if (tk == Token::KwSi) {
        if_statement();
        more_statements();
    } else if (tk == Token::KwMientras) {
        tk = lex.getNextToken();
        expr();
        optional_eol();
        expect(Token::KwHaga, "haga");
        optional_eol();
        statement();
        expect(Token::KwFin, "fin");
        expect(Token::KwMientras, "mientras");
        more_statements();
    } else if (tk == Token::KwRepita) {
        tk = lex.getNextToken();
        expect(Token::EndLine, "end of line");
        statement();
        expect(Token::KwHasta, "hasta");
        expr();
        more_statements();
    } else if (tk == Token::KwPara) {
        tk = lex.getNextToken();
        lvalue();
        expect(Token::Assign, "assign operator");
        expr();
        expect(Token::KwHasta, "hasta");
        expr();
        expect(Token::KwHaga, "haga");
        expect(Token::EndLine, "end of line");
        statement();
        expect(Token::KwFin, "fin");
        expect(Token::KwPara, "para");
        more_statements();
    } else if (tk == Token::KwRetorne) {
        tk = lex.getNextToken();
        expr();
        more_statements();
    } else
        syntaxError("statement");
}

void parser::if_statement() {
    expect(Token::KwSi, "SI");
    expr();
    optional_eol();
    expect(Token::KwEntonces, "entonces");
    optional_eol();
    if_stmt();
}

void parser::if_stmt() {
    if (tk == Token::Iden) {
        lvalue();
        if (tk == Token::Assign) {
            tk = lex.getNextToken();
            expr();
            more_if_stmt();
        }
    } else if (tk == Token::KwLlamar) {
        tk = lex.getNextToken();
        if (tk == Token::Iden) {
            tk = lex.getNextToken();
            args_call();
            more_if_stmt();
        }
    } else if (tk == Token::KwEscriba) {
        tk = lex.getNextToken();
        string_args();
        more_if_stmt();
    } else if (tk == Token::KwLea) {
        tk = lex.getNextToken();
        lvalue();
        more_if_stmt();
    } else if (tk == Token::KwSi) {
        if_statement();
        more_if_stmt();
    } else if (tk == Token::KwMientras) {
        tk = lex.getNextToken();
        expr();
        optional_eol();
        expect(Token::KwHaga, "haga");
        optional_eol();
        statement();
        expect(Token::KwFin, "fin");
        expect(Token::KwMientras, "mientras");
        more_if_stmt();
    } else if (tk == Token::KwRepita) {
        tk = lex.getNextToken();
        expect(Token::EndLine, "end of line");
        statement();
        expect(Token::KwHasta, "hasta");
        expr();
        more_if_stmt();
    } else if (tk == Token::KwPara) {
        tk = lex.getNextToken();
        lvalue();
        expect(Token::Assign, "assign operator");
        expr();
        expect(Token::KwHasta, "hasta");
        expr();
        expect(Token::KwHaga, "haga");
        expect(Token::EndLine, "end of line");
        statement();
        expect(Token::KwFin, "fin");
        expect(Token::KwPara, "para");
        more_if_stmt();
    } else if (tk == Token::KwRetorne) {
        tk = lex.getNextToken();
        expr();
        more_if_stmt();
    } else
        syntaxError("statement inside if");
}

void parser::more_if_stmt() {
    if (tk == Token::EndLine) {
        tk = lex.getNextToken();
        more_if_stmt_p();
    } else
        syntaxError("end of line");
}

void parser::more_if_stmt_p() {
    if (tk == Token::Iden) {
        lvalue();
        if (tk == Token::Assign) {
            tk = lex.getNextToken();
            expr();
            more_if_stmt();
        } else
            syntaxError("assign");
    } else if (tk == Token::KwLlamar) {
        tk = lex.getNextToken();
        if (tk == Token::Iden) {
            tk = lex.getNextToken();
            args_call();
            more_if_stmt();
        } else
            syntaxError("identifier");
    } else if (tk == Token::KwEscriba) {
        tk = lex.getNextToken();
        string_args();
        more_if_stmt();
    } else if (tk == Token::KwLea) {
        tk = lex.getNextToken();
        lvalue();
        more_if_stmt();
    } else if (tk == Token::KwSi) {
        if_statement();
        more_if_stmt();
    } else if (tk == Token::KwMientras) {
        tk = lex.getNextToken();
        expr();
        optional_eol();
        if (tk == Token::KwHaga) {
            tk = lex.getNextToken();
            optional_eol();
            statement();
            if (tk == Token::KwFin) {
                tk = lex.getNextToken();
                if (tk == Token::KwMientras) {
                    tk = lex.getNextToken();
                    more_statements();
                } else
                    syntaxError("mientras");
            } else
                syntaxError("fin");
        } else
            syntaxError("haga");
    } else if (tk == Token::KwRepita) {
        tk = lex.getNextToken();
        if (tk == Token::EndLine) {
            tk = lex.getNextToken();
            statement();
            if (tk == Token::KwHasta) {
                tk = lex.getNextToken();
                expr();
                more_statements();
            } else
                syntaxError("hasta");
        } else
            syntaxError("end of line");
    } else if (tk == Token::KwPara) {
        tk = lex.getNextToken();
        lvalue();
        if (tk == Token::Assign) {
            tk = lex.getNextToken();
            expr();
            if (tk == Token::KwHasta) {
                tk = lex.getNextToken();
                expr();
                if (tk == Token::KwHaga) {
                    tk = lex.getNextToken();
                    if (tk == Token::EndLine) {
                        tk = lex.getNextToken();
                        statement();
                        if (tk == Token::KwFin) {
                            tk = lex.getNextToken();
                            if (tk == Token::KwPara) {
                                tk = lex.getNextToken();
                                more_statements();
                            } else
                                syntaxError("para");
                        } else
                            syntaxError("fin");
                    } else
                        syntaxError("end of line");
                } else
                    syntaxError("haga");
            } else
                syntaxError("hasta");
        } else
            syntaxError("assign operator");
    } else if (tk == Token::KwRetorne) {
        tk = lex.getNextToken();
        expr();
        more_if_stmt();
    } else if (tk == Token::KwSino) {
        else_if_block();
        if (tk == Token::KwFin) {
            tk = lex.getNextToken();
            if (tk == Token::KwSi)
                tk = lex.getNextToken();
            else
                syntaxError("SI");
        } else
            syntaxError("fin");
    } else if (tk == Token::KwFin) {
        tk = lex.getNextToken();
        if (tk == Token::KwSi)
            tk = lex.getNextToken();
        else
            syntaxError("SI");
    } else
        syntaxError("fin si");
}

void parser::else_if_block() {
    if (tk == Token::KwSino) {
        tk = lex.getNextToken();
        else_if_block_p();
    } else {
        /*epsilon*/
    }
}

void parser::more_else_if_block() {
    if (tk == Token::EndLine) {
        tk = lex.getNextToken();
        else_if_block();
    } else
        syntaxError("end of line");
}

void parser::else_if_block_p() {
    if (tk == Token::KwSi) {
        tk = lex.getNextToken();
        expr();
        optional_eol();
        if (tk == Token::KwEntonces) {
            tk = lex.getNextToken();
            optional_eol();
            statement();
            more_else_if_block();
        } else
            syntaxError("entonces");
    } else if (tokenIs(Token::Iden, Token::KwLlamar, Token::KwEscriba,
                       Token::KwLea, Token::KwSi, Token::EndLine,
                       Token::KwMientras, Token::KwRepita,
                       Token::KwPara, Token::KwRetorne)) {
        optional_eol();
        statement();
    } else
        syntaxError("SI");
}

void parser::optional_eol() {
    if (tk == Token::EndLine)
        tk = lex.getNextToken();
    else {
        /*epsilon*/
    }
}

void parser::string_args() {
    if (tk == Token::StringConst) {
        tk = lex.getNextToken();
        more_string_args();
    } else if (tokenIs(Token::Iden, Token::IntConst, Token::CharConst,
                       Token::StringConst, Token::KwVerdadero,
                       Token::KwFalso, Token::Sub, Token::KwNo,
                       Token::OpenParens)) {
        expr();
        more_string_args();
    } else
        syntaxError("expresion");
}

void parser::more_string_args() {
    if (tk == Token::Comma) {
        tk = lex.getNextToken();
        string_args();
    } else {
        /*epsilon*/
    }
}

void parser::more_statements() {
    if (tk == Token::EndLine) {
        tk = lex.getNextToken();
        statements();
    } else {
        /*epsilon*/
    }
}

void parser::lvalue() {
    if (tk == Token::Iden) {
        tk = lex.getNextToken();
        lvalue_p();
    } else
        syntaxError("identifier");
}

void parser::lvalue_p() {
    if (tk == Token::OpenBra) {
        tk = lex.getNextToken();
        expr();
        if (tk == Token::CloseBra)
            tk = lex.getNextToken();
        else
            syntaxError("Close Parens");
    } else {
        /*epsilon*/
    }   
}

void parser::rvalue() {
    if (tk == Token::Iden) {
        tk = lex.getNextToken();
        rvalue_p();
    } else
        syntaxError("identifier");
}

void parser::rvalue_p() {
    if (tk == Token::OpenBra) {
        tk = lex.getNextToken();
        expr();
        if (tk == Token::CloseBra)
            tk = lex.getNextToken();
        else
            syntaxError("close bracket");
    } else if(tk == Token::OpenParens)
        args_call();
    else {
        /*epsilon*/
    } 
}

void parser::args_call() {
    if (tk == Token::OpenParens) {
        tk = lex.getNextToken();
        arg_decl();
        if (tk == Token::CloseParens)
            tk = lex.getNextToken();
        else
            syntaxError("Close Parens");
    } else
        syntaxError("open Parens");
}

void parser::arg_decl() {
    if (tokenIs(Token::Iden, Token::IntConst, Token::CharConst,
                Token::StringConst, Token::KwVerdadero,
                Token::KwFalso, Token::Sub, Token::KwNo,
                Token::OpenParens)) {
        expr();
        more_arg_decl();
    } else {
        /*epsilon*/
    }
}

void parser::more_arg_decl() {
    if (tk == Token::Comma) {
        tk = lex.getNextToken();
        arg_decl();
    } else {
        /*epsilon*/
    }
}

void parser::expr() {
    if (tk == Token::Iden) {
        rvalue();
        expr_p();
    } else if (tokenIs(Token::IntConst, Token::CharConst, /*Token::StringConst,*/
                       Token::KwVerdadero, Token::KwFalso)) {
        constant();
        expr_p();
    }else if (tk == Token::Sub) {
        tk = lex.getNextToken();
        expr();
    } else if (tk == Token::KwNo) {
        tk = lex.getNextToken();
        expr();
    } else if (tk == Token::OpenParens) {
        tk = lex.getNextToken();
        expr();
        if (tk == Token::CloseParens)
            tk = lex.getNextToken();
        else
            syntaxError("close parens");
    } else
        syntaxError("expresion");
}

void parser::expr_p() {
    if (tokenIs(Token::Add, Token::Sub, Token::Mul, Token::KwDiv,
                Token::KwMod, Token::LessThan, Token::GreatThan,
                Token::LessEqual, Token::GreatEqual, Token::EqualTo,
                Token::NotEqual, Token::KwY, Token::KwO, Token::Xor)) {
        bin_op();
        expr();
        expr_p();
    } else {
        /*epsilon*/
    }
}

void parser::var_decl() {
    if (tokenIs(Token::KwEntero, Token::KwReal, Token::KwCadena,
                Token::KwBooleano, Token::KwCaracter)) {
        type();
        if (tk == Token::Iden) {
            tk = lex.getNextToken();
            more_var();
            if (tk == Token::EndLine)
                tk = lex.getNextToken();
            else
                syntaxError("end of line");
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
    else if (tk == Token::KwArreglo)
        array_type();
    else
        syntaxError("type");
}

void parser::array_type() {
    if (tk == Token::KwArreglo) {
        tk = lex.getNextToken();
        if (tk == Token::OpenBra) {
            tk = lex.getNextToken();
            if (tk == Token::IntConst) {
                tk = lex.getNextToken();
                if (tk == Token::CloseBra) {
                    tk = lex.getNextToken();
                    if (tk == Token::KwDe) {
                        tk = lex.getNextToken();
                        type();
                    } else
                        syntaxError("DE");
                } else
                    syntaxError("Close bracket");
            } else
                syntaxError("int constant");
        } else
            syntaxError("Open bracket");
    } else
        syntaxError("arreglo");
}

void parser::constant() {
    if (tk == Token::IntConst)
        tk = lex.getNextToken();
    else if (tk == Token::CharConst)
        tk = lex.getNextToken();
    //else if (tk == Token::StringConst)
    //    tk = lex.getNextToken();
    else if (tokenIs(Token::KwVerdadero, Token::KwFalso))
        bool_const();
    else
        syntaxError("constant value");
}

void parser::bool_const() {
    if (tk == Token::KwFalso)
        tk = lex.getNextToken();
    else if (tk == Token::KwVerdadero)
        tk = lex.getNextToken();
    else
        syntaxError("boolean expresion");
}

void parser::bin_op() {
    if (tokenIs(Token::Add, Token::Sub, Token::Mul, Token::KwDiv,
                Token::KwMod)) {
        arith_op();
    } else if (tokenIs(Token::LessThan, Token::GreatThan,
                       Token::LessEqual, Token::GreatEqual)) {
        rel_op();
    } else if (tokenIs(Token::EqualTo, Token::NotEqual)) {
        eq_op();
    } else if (tokenIs(Token::KwY, Token::KwO, Token::Xor)) {
        cond_op();
    } else
        syntaxError("operator");
}

void parser::arith_op() {
    if (tokenIs(Token::Add, Token::Sub, Token::Mul, Token::KwDiv,
                Token::KwMod)) {
        tk = lex.getNextToken();
    } else
        syntaxError("arithmetic operator");
}

void parser::rel_op() {
    if (tokenIs(Token::LessThan, Token::GreatThan, Token::LessEqual,
                Token::GreatEqual)) {
        tk = lex.getNextToken();
    } else
        syntaxError("relational operator");
}

void parser::eq_op() {
    if (tokenIs(Token::EqualTo, Token::NotEqual)) {
        tk = lex.getNextToken();
    } else
        syntaxError("equal operator");
}

void parser::cond_op() {
    if (tokenIs(Token::KwY, Token::KwO, Token::Xor)) {
        tk = lex.getNextToken();
    } else
        syntaxError("boolean operator");
}