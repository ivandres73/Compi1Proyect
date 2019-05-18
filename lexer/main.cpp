#include "exprLex.h"

int main(int argc, char** argv) {
    if (argc < 2)
    {
        cout << "you must add a file as argument..\n";
        return 1;
    }
    ifstream in(argv[1], std::ios::in);
    exprLex lex(in);

    Token tk;
    while (true) {
        try {
            tk = lex.getNextToken();
            cout << lex.toString(tk) << " '" << lex.getText() << "'" << endl;
            if (tk == Token::EndFile)
                break;
        } catch (const char* e) {
            std::cerr << e;
        }
    }

    in.close();
    return 0;
}