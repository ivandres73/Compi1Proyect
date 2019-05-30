#include "lexer.h"
#include <iostream>
#include <fstream>
#include <string>

using std::cout;
using std::endl;
using std::ifstream;
using std::string;

class exprLex {
private:
    int line;
    string text;
    ifstream& in;
    void reportError(char ch) {
        cout << "\033[1;31merror with \033[0m" << ch << "\033[1;31m at line \033[0m" << line << endl;
    }
    char getNextChar() {
        char ch = in.get();
        if (ch == '\n') line++;
        return ch;
    }
    void ungetChar(char ch) {
        if (ch == '\n') line--;
        in.unget();
    }
public:
    exprLex(ifstream& in) : in(in), line(1) { }
    const char* toString(Token);
    Token getNextToken();
    string getText() { return text; }
    int getLine() { return line; }
};
