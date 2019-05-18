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
        cout << "error with " << ch << " at line " << line << endl;
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
};