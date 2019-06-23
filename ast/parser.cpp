#include "parser.h"

void parser::parse() {
    tk = lex.getNextToken();
    program();
    if (tk != Token::EndFile)
        throw "something's wrong\n";
}

void parser::program() {
    optional_eol();
    subtypes_section();
    optional_eol();
    var_section();
    optional_eol();
    subprogram_decl();
    expect(Token::KwInicio, "inicio");
    optional_eol();
    statements();
    fin();
    optional_eol();
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
                Token::KwBooleano, Token::KwCaracter, Token::KwArreglo)) {
        vector<string> vars;
        Token type = tk;
        var_decl(vars);
        STMTSP stmt;
        switch (type) {
            case Token::KwEntero:
                stmt = DeclStmt(vars, "int");
                break;
            case Token::KwBooleano:
                stmt = DeclStmt(vars, "bool");
                break;
            case Token::KwCaracter:
                stmt = DeclStmt(vars, "char");
                break;
            case Token::KwArreglo:
                /* TO DO */
                break;
            case Token::KwReal:
                /* TO DO */
                break;
            case Token::KwCadena:
                /* TO DO */
                break;
        }
        try {
            stmt->exec(global_vars);
        } catch (string e) {
            cout << e << endl;
        }
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
        fin();
        expect(Token::EndLine, "end of line");
        optional_eol();
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
    if (tk == Token::OpenParens) {
        expect(Token::OpenParens, "open parens");
        argument_decl();
        expect(Token::CloseParens, "close parens");
    } else {
        /*epsilon*/
    }
}

void parser::argument_decl() {
    if (tokenIs(Token::KwEntero, Token::KwReal, Token::KwCadena,
                Token::KwBooleano, Token::KwCaracter, Token::KwArreglo)) {
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
                Token::KwBooleano, Token::KwCaracter, Token::KwArreglo)) {
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
    STMTSP stmt;
    if (tk == Token::Iden) {
        string id = lex.getText();
        lvalue();
        expect(Token::Assign, "assign");
        string vacio;
        EXPRSP expr = expr0(vacio);
        stmt = AssignStmt(id, expr);
        try {
            stmt->exec(global_vars);
        } catch (const string& e) {
            cout << e << endl;
        }
        more_statements();
    } else if (tk == Token::KwLlamar) {
        tk = lex.getNextToken();
        expect(Token::Iden, "identifier");
        args_call();
        more_statements();
    } else if (tk == Token::KwEscriba) {
        tk = lex.getNextToken();
        vector<string> args;
        string_args(args);
        stmt = WriteStmt(args);
        stmt->exec(global_vars);
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
        string vacio;
        expr0(vacio);
        optional_eol();
        expect(Token::KwHaga, "haga");
        optional_eol();
        statement();
        fin();
        expect(Token::KwMientras, "mientras");
        more_statements();
    } else if (tk == Token::KwRepita) {
        tk = lex.getNextToken();
        expect(Token::EndLine, "end of line");
        statement();
        expect(Token::KwHasta, "hasta");
        string vacio;
        expr0(vacio);
        more_statements();
    } else if (tk == Token::KwPara) {
        tk = lex.getNextToken();
        lvalue();
        expect(Token::Assign, "assign operator");
        string vacio;
        expr0(vacio);
        expect(Token::KwHasta, "hasta");
        expr0(vacio);
        expect(Token::KwHaga, "haga");
        expect(Token::EndLine, "end of line");
        statement();
        fin();
        expect(Token::KwPara, "para");
        more_statements();
    } else if (tk == Token::KwRetorne) {
        tk = lex.getNextToken();
        string vacio;
        expr0(vacio);
        more_statements();
    } else
        syntaxError("statement");
}

void parser::if_statement() {
    expect(Token::KwSi, "SI");
    string vacio;
    expr0(vacio);
    optional_eol();
    expect(Token::KwEntonces, "entonces");
    optional_eol();
    statement();
    else_if_block();
    fin();
    expect(Token::KwSi, "si after fin");
}

void parser::else_if_block() {
    if (tk == Token::KwSino) {
        tk = lex.getNextToken();
        else_if_block_p();
    } else {
        /*epsilon*/
    }
}

void parser::else_if_block_p() {
    if (tk == Token::KwSi) {
        tk = lex.getNextToken();
        string vacio;
        expr0(vacio);
        optional_eol();
        expect(Token::KwEntonces, "entonces");
        optional_eol();
        statement();
        else_if_block();
    } else if (tokenIs(Token::Iden, Token::KwLlamar, Token::KwEscriba,
                       Token::KwLea, Token::KwSi, Token::EndLine,
                       Token::KwMientras, Token::KwRepita,
                       Token::KwPara, Token::KwRetorne)) {
        optional_eol();
        statement();
    } else
        syntaxError("SI or a statement");
}

void parser::optional_eol() {
    if (tk == Token::EndLine) {
        tk = lex.getNextToken();
        optional_eol();
    } else {
        /*epsilon*/
    }
}

void parser::string_args(vector<string>& args) {
    if (tk == Token::StringConst) {
        args.push_back(lex.getText());
        tk = lex.getNextToken();
        more_string_args(args);
    } else if (tokenIs(Token::Iden, Token::IntConst, Token::CharConst,
                       Token::StringConst, Token::KwVerdadero,
                       Token::KwFalso, Token::Sub, Token::KwNo,
                       Token::OpenParens)) {
        try {
            string tipoExpr = "int";
            EXPRSP expr = expr0(tipoExpr);
            int valor = expr->eval(global_vars);
            if (tipoExpr == "int")
                args.push_back(to_string(valor));
            else if (tipoExpr == "bool") {
                if (valor == 0)
                    args.push_back("Falso");
                else
                    args.push_back("Verdadero");
            } else //if (typeExpr == "char")
                args.push_back(to_string(static_cast<char>(valor)));
        } catch (const string& e) {
            cout << e << endl;
        }
        more_string_args(args);
    } else
        syntaxError("expresion");
}

void parser::more_string_args(vector<string>& args) {
    if (tk == Token::Comma) {
        tk = lex.getNextToken();
        string_args(args);
    } else {
        /*epsilon*/
    }
}

void parser::more_statements() {
    if (tk == Token::EndLine) {
        tk = lex.getNextToken();
        optional_eol();
        statements();
    } else {
        /*epsilon*/
    }
}

void parser::lvalue() {
    expect(Token::Iden, "identifier");
    lvalue_p();
}

void parser::lvalue_p() {
    if (tk == Token::OpenBra) {
        tk = lex.getNextToken();
        string vacio;
        expr0(vacio);
        expect(Token::CloseBra, "Close Bra");
    } else {
        /*epsilon*/
    }
}

EXPRSP parser::rvalue(string& tipoExpr) {
    EXPRSP expr = nullptr;
    if (tk == Token::Iden) {
        shared_ptr<IdenExpr> ie = IdExpr(lex.getText());
        expr = ie;
        if (ie->getType(global_vars) == "int")
            tipoExpr = "int";
        else if (ie->getType(global_vars) == "bool")
            tipoExpr = "bool";
        else if (ie->getType(global_vars) == "char")
            tipoExpr = "char";
        tk = lex.getNextToken();
        rvalue_p();
    } else
        syntaxError("identifier");
    
    return expr;
}

void parser::rvalue_p() {
    if (tk == Token::OpenBra) {
        tk = lex.getNextToken();
        string vacio;
        expr0(vacio);
        expect(Token::CloseBra, "close bracket");
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
        expect(Token::CloseParens, "close parens");
    } else {
        /*epsilon*/
    }
}

void parser::arg_decl() {
    if (tokenIs(Token::Iden, Token::IntConst, Token::CharConst,
                Token::StringConst, Token::KwVerdadero,
                Token::KwFalso, Token::Sub, Token::KwNo,
                Token::OpenParens)) {
        string vacio;
        expr0(vacio);
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

EXPRSP parser::expr0(string& tipoExpr) {
    EXPRSP expr = expr1(tipoExpr);
    if (expr == nullptr) {
        syntaxError("expression");
        return nullptr;
    }
    if (tokenIs(Token::EqualTo, Token::NotEqual, Token::LessThan,
                   Token::GreatThan, Token::LessEqual,
                   Token::GreatEqual)) {
        Token op = tk;
        tk = lex.getNextToken();
        EXPRSP expr2 = expr1(tipoExpr);
        if (expr2 == nullptr)
            return nullptr;
        
        switch (op) {
        case Token::EqualTo:
            expr = EQExpr(expr, expr2);
            break;
        case Token::NotEqual:
            expr = NEQExpr(expr, expr2);
            break;
        case Token::LessThan:
            expr = LTExpr(expr, expr2);
            break;
        case Token::GreatThan:
            expr = GTExpr(expr, expr2);
            break;
        case Token::LessEqual:
            expr = LTEExpr(expr, expr2);
            break;
        case Token::GreatEqual:
            expr = GTEExpr(expr, expr2);
            break;
        }
    }
    return expr;
}

EXPRSP parser::expr1(string& tipoExpr) {
    EXPRSP expr1 = expr2(tipoExpr);
    while (tokenIs(Token::Add, Token::Sub, Token::KwO)) {
        Token op = tk;
        tk = lex.getNextToken();
        EXPRSP expr = expr2(tipoExpr);
        if (expr == nullptr)
            return nullptr;

        switch (op) {
        case Token::Add:
            expr1 = AddExpr(expr1, expr);
            break;
        case Token::Sub:
            expr1 = SubExpr(expr1, expr);
            break;
        case Token::KwO:
            expr1 = OrExpr(expr1, expr);
            break;
        }
    }

    return expr1;
}

EXPRSP parser::expr2(string& tipoExpr) {
    EXPRSP expr1 = expr3(tipoExpr);
    while(tokenIs(Token::Mul, Token::KwDiv, Token::KwMod, Token::KwY)) {
        Token op = tk;
        tk = lex.getNextToken();
        EXPRSP expr2 = expr3(tipoExpr);
        if (expr2 == nullptr)
            return nullptr;
        
        switch (op) {
        case Token::Mul:
            expr1 = MulExpr(expr1, expr2);
            break;
        case Token::KwDiv:
            expr1 = DivExpr(expr1, expr2);
            break;
        case Token::KwMod:
            expr1 = ModExpr(expr1, expr2);
            break;
        case Token::KwY:
            expr1 = AndExpr(expr1, expr2);
            break;
        }
    }
    return expr1;
}

EXPRSP parser::expr3(string& tipoExpr) {
    EXPRSP expr1 = expr4(tipoExpr);
    while (tk == Token::Pow) {
        Token op = tk;
        tk = lex.getNextToken();
        EXPRSP expr2 = expr4(tipoExpr);
        if (expr2 == nullptr)
            return nullptr;
        
        switch (op) {
        case Token::Pow:
            expr1 = PowExpr(expr1, expr2);
            break;
        
        }
    }
    return expr1;
}

EXPRSP parser::expr4(string& tipoExpr) {
    EXPRSP expr1 = expr5(tipoExpr);
    while (tokenIs(Token::KwNo, Token::Sub)) {
        Token op = tk;
        tk = lex.getNextToken();
        expr1 = expr5(tipoExpr);
        if (expr1 == nullptr)
            return nullptr;

        switch (op) {
        case Token::KwNo:
            expr1 = NotExpr(expr1);
            break;
        case Token::Sub:
            expr1 = NegExpr(expr1);
            break;
        }
    }

    return expr1;
}

EXPRSP parser::expr5(string& tipoExpr) {
    EXPRSP expr1 = nullptr;
    if (tk == Token::Iden) {
        expr1 = rvalue(tipoExpr);
    } else if (tokenIs(Token::IntConst, Token::CharConst,
                       Token::KwFalso, Token::KwVerdadero)) {
        expr1 = constant(tipoExpr);
    } else if (tokenIs(Token::OpenParens)) {
        tk = lex.getNextToken();
        expr1 = expr0(tipoExpr);
        expect(Token::CloseParens, "close parens");
    }

    return expr1;
}

void parser::var_decl(vector<string>& vars) {
    if (tokenIs(Token::KwEntero, Token::KwReal, Token::KwCadena,
                Token::KwBooleano, Token::KwCaracter, Token::KwArreglo)) {
        type();
        vars.push_back(lex.getText());
        expect(Token::Iden, "identifier");
        more_var(vars);
        expect(Token::EndLine, "end of line");
    } else
        syntaxError("type");
}

void parser::more_var(vector<string>& vars) {
    if (tk == Token::Comma) {
        tk = lex.getNextToken();
        vars.push_back(lex.getText());
        expect(Token::Iden, "identifier");
        more_var(vars);
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
    expect(Token::KwArreglo, "arreglo");
    expect(Token::OpenBra, "open bracket");
    expect(Token::IntConst, "int constant");
    expect(Token::CloseBra, "close bracket");
    expect(Token::KwDe, "DE");
    type();
}

EXPRSP parser::constant(string& tipoExpr) {
    EXPRSP value = nullptr;
    if (tk == Token::IntConst) {
        if (lex.getText() == "") {//esta validacion es por un error del lexer
            value = NumExpr("0");
        } else {
            value = NumExpr(lex.getText());
        }

        tk = lex.getNextToken();
    } else if (tk == Token::CharConst) {
        value = CharExpr(lex.getText());
        tk = lex.getNextToken();
        tipoExpr = "char";
    } else if (tokenIs(Token::KwVerdadero, Token::KwFalso)) {
        tipoExpr = "bool";
        return bool_const();
    } else
        syntaxError("constant value");
    return value;
}

EXPRSP parser::bool_const() {
    EXPRSP value;
    if (tk == Token::KwFalso) {
        value = BoolExpr(false);
        tk = lex.getNextToken();
    } else if (tk == Token::KwVerdadero) {
        value = BoolExpr(true);
        tk = lex.getNextToken();
    } else {
        syntaxError("boolean expresion");
        return nullptr;
    }
    return value;
}

void parser::fin() {
    if (tokenIs(Token::KwFin, Token::KwFinal)) {
        tk = lex.getNextToken();
    } else
        syntaxError("fin");
}