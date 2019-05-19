#include "parser.h"

int main(int argc, char** argv) {
    if (argc < 2)
    {
        cout << "you must add a file as argument..\n";
        return 1;
    }
    ifstream in(argv[1], std::ios::in);
    exprLex lex(in);
    parser parser(lex);

    try {
        parser.parse();
    } catch (const char* e) {
        std::cerr << e;
    }

    in.close();
    return 0;
}