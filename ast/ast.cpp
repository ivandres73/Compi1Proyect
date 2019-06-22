#include "ast.h"

BinExpr::BinExpr(EXPRSP&& e1, EXPRSP&& e2, int8_t pr) : expr1(move(e1)), expr2(move(e2)) {
    prec = pr;
}

PowExpr::PowExpr(EXPRSP e1, EXPRSP e2) : BinExpr(move(e1), move(e2), 2) { }
int PowExpr::eval() {
    return pow(expr1->eval(), expr2->eval());
}
string PowExpr::toString() {
    return expr1->toString() + "^" + expr2->toString();
}

NegExpr::NegExpr(EXPRSP e) : expr(move(e)) { prec = 1; }
int NegExpr::eval() {
    return -expr->eval();
}
string NegExpr::toString() {
    return "-" + expr->toString();
}

NotExpr::NotExpr(EXPRSP ex) : expr(move(ex)) { prec = 1; }
int NotExpr::eval() {
    return !expr->eval();
}
string NotExpr::toString() {
    return "!" + expr->toString();
}

WriteStmt::WriteStmt(vector<string> v) : args(v) {}
void WriteStmt::exec() {
    for (auto i : args)
        cout << i;
}
string WriteStmt::toString() {
    return "escriba";
}