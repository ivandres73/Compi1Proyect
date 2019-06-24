#include "ast.h"

BinExpr::BinExpr(EXPRSP&& e1, EXPRSP&& e2, int8_t pr) :
expr1(move(e1)),expr2(move(e2)) {
    prec = pr;
}

PowExpr::PowExpr(EXPRSP e1, EXPRSP e2) : BinExpr(move(e1), move(e2),
2) {}
int PowExpr::eval(Context& ctx) {
    return pow(expr1->eval(ctx), expr2->eval(ctx));
}
string PowExpr::toString() {
    return expr1->toString() + "^" + expr2->toString();
}

NegExpr::NegExpr(EXPRSP e) : expr(move(e)) { prec = 1; }
int NegExpr::eval(Context& ctx) {
    return -expr->eval(ctx);
}
string NegExpr::toString() {
    return "-" + expr->toString();
}

NotExpr::NotExpr(EXPRSP ex) : expr(move(ex)) { prec = 1; }
int NotExpr::eval(Context& ctx) {
    return !expr->eval(ctx);
}
string NotExpr::toString() {
    return "!" + expr->toString();
}

IdenExpr::IdenExpr(string str) : id(str) {}
int IdenExpr::eval(Context& ctx) {
    if (ctx.vars.find(id) == ctx.vars.end())
        throw id + " was not declared";
    else if (ctx.vars[id].value == UNINITIALIZED)
        throw id + " has not been initialized";
    else
        return ctx.vars[id].value;
}
string IdenExpr::toString() { return id; }
string IdenExpr::getType(Context& ctx) {
    return ctx.vars[id].tipo;
}

WriteStmt::WriteStmt(vector<string>& v, vector<EXPRSP>& e) :
exprs_type(v), exprs(e) {}
void WriteStmt::exec(Context& ctx) {
    int pos = 0;
    for (auto i : exprs) {
        if (i == nullptr) {//imprimir constantes string
            cout << exprs_type[pos];
        } else {
            try {
                if (exprs_type[pos] == "int") {
                    cout << i->eval(ctx);
                } else if (exprs_type[pos] == "bool") {
                    if (i->eval(ctx) == 0)
                        cout << "Falso";
                    else
                        cout << "Verdadero";
                } else { //if (exprs_type[pos] == "char")
                    char car = static_cast<char>(i->eval(ctx));
                    cout << string(1, car);
                }
            } catch (const string& e) {
                cout << e << std::endl;
            }
        }
    }
}
string WriteStmt::toString() {
    return "escriba";
}

DeclareStmt::DeclareStmt(vector<string>& list, string ti) :
ids(list) , tipo(ti) {}
void DeclareStmt::exec(Context& ctx) {
    for (auto i : ids) {
        if (ctx.vars.find(i) != ctx.vars.end())
            throw i + " has already been declared";
        else {
            ctx.vars[i].value = UNINITIALIZED;
            ctx.vars[i].tipo = tipo;
        }
    }
}
string DeclareStmt::toString() {
    string vars = "";
    for (auto i : ids)
        vars += "," + i;
    return vars;
}

AssignStmt::AssignStmt(string str, EXPRSP expr) : id(str), val(expr) {}
void AssignStmt::exec(Context& ctx) {
    if (ctx.vars.find(id) == ctx.vars.end()) {
        throw id + " was not declared";
    } else {
        ctx.vars[id].value = val->eval(ctx);
    }
}
string AssignStmt::toString() {
    return id + "<-" + val->toString();
}

WhileStmt::WhileStmt(EXPRSP exp, STMTS& lis) : expr(exp), list(lis) {}
void WhileStmt::exec(Context& ctx) {
    while (expr->eval(ctx)) {
        for (auto i : list) {
            i->exec(ctx);
        }
    }
}
string WhileStmt::toString() {
    return "while";
}

IfStmt::IfStmt(EXPRSP exp, STMTS& stmt, vector<EXPRSP>& v_expr,
vector<STMTS>& v_stmt, STMTS& els) : expr_if(exp), stmts_if(stmt),
expr_elsif(v_expr), stmts_elsif(v_stmt), stmts_else(els) {

}
void IfStmt::exec(Context& ctx) {
    if (expr_if->eval(ctx)) {
        for (auto i : stmts_if) {
            i->exec(ctx);
        }
        return;
    }
    if (!expr_elsif.empty()) {
        int pos = 0;
        for (auto i : expr_elsif) {
            if (i->eval(ctx)) {
                STMTS lista = stmts_elsif[pos];
                for (auto v : lista) {
                    v->exec(ctx);
                }
                return;
            }
            pos++;
        }
    }
    if (!stmts_else.empty()) {
        for (auto i : stmts_else) {
            i->exec(ctx);
        }
    }
}
string IfStmt::toString() {
    return "if";
}

ForStmt::ForStmt(shared_ptr<AssignStmt>& a, EXPRSP e, STMTS& s) : inicio(a),
breque(e), stmts(s) {}
void ForStmt::exec(Context& ctx) {
    inicio->exec(ctx);
    string id = inicio->getId();
    int fin = breque->eval(ctx);
    for (; ctx.vars[id].value <= fin; ctx.vars[id].value++) {
        for (auto v : stmts) {
            v->exec(ctx);
        }
    }
}
string ForStmt::toString() {
    return "for";
}

DoWhileStmt::DoWhileStmt(EXPRSP e, STMTS& s) : expr(e), lista(s) {}
void DoWhileStmt::exec(Context& ctx) {
    do {
        for (auto i : lista)
            i->exec(ctx);
    } while (!expr->eval(ctx));
}
string DoWhileStmt::toString() {
    return "do-while";
}