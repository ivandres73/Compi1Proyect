Token exprLex::getNextToken() {
    StateId state = StateId::Start_q0;
    int ch = getNextChar();

    while (true) {
        switch (state) {
            // Start
            case StateId::Start_q0:
                if (ch == '\n') {
                    state = StateId::Start_q0;
                    ch = getNextChar();
                } else if (ch == ' ') {
                    state = StateId::Start_q0;
                    ch = getNextChar();
                } else if (ch == '\t') {
                    state = StateId::Start_q0;
                    ch = getNextChar();
                } else {
                    state = StateId::Start_q1;
                    text="";
                }
                break;
            case StateId::Start_q1:
                if (ch == '\'') {
                    state = StateId::Char_q0;
                    ch = getNextChar();
                } else if (ch == '/') {
                    state = StateId::Comments_q0;
                    ch = getNextChar();
                } else if ((ch >= '1') && (ch <= '9')) {
                    text += ch;
                    state = StateId::Dec_q0;
                    ch = getNextChar();
                } else if (ch == '"') {
                    state = StateId::String_q0;
                    ch = getNextChar();
                } else if (ch == EOF) {
                    return Token::EndFile;
                } else if (ch == '0') {
                    state = StateId::BinHex_q0;
                    ch = getNextChar();
                } else {
                    state = StateId::KewIden_q0;
                }
                break;
            // BinHex
            case StateId::BinHex_q0:
                if (ch == 'b') {
                    text += ch;
                    state = StateId::BinHex_q2;
                    ch = getNextChar();
                } else if (ch == 'B') {
                    text += ch;
                    state = StateId::BinHex_q2;
                    ch = getNextChar();
                } else if ((ch >= '1') && (ch <= '9')) {
                    text += ch;
                    state = StateId::Dec_q0;
                    ch = getNextChar();
                } else if (ch == '0') {
                    state = StateId::Dec_q0;
                    ch = getNextChar();
                } else if (ch == 'x') {
                    text += ch;
                    state = StateId::BinHex_q1;
                    ch = getNextChar();
                } else if (ch == 'X') {
                    text += ch;
                    state = StateId::BinHex_q1;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::DecConst;
                }
                break;
            case StateId::BinHex_q1:
                if (isxdigit(ch)) {
                    text += ch;
                    state = StateId::BinHex_q1;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::HexConst;
                }
                break;
            case StateId::BinHex_q2:
                if ((ch >= '0') && (ch <= '1')) {
                    text += ch;
                    state = StateId::BinHex_q2;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::BinConst;
                }
                break;
            // Char
            case StateId::Char_q0:
                if ((ch >= 'a') && (ch <= 'z')) {
                    text += ch;
                    state = StateId::Char_q1;
                    ch = getNextChar();
                } else if ((ch >= '0') && (ch <= '9')) {
                    text += ch;
                    state = StateId::Char_q1;
                    ch = getNextChar();
                } else if (ch == '\'') {
                    state = StateId::Char_q5;
                    ch = getNextChar();
                } else if ((ch >= 'A') && (ch <= 'Z')) {
                    text += ch;
                    state = StateId::Char_q1;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::Char_q1;
                    ch = getNextChar();
                }
                break;
            case StateId::Char_q1:
                if (ch == '\'') {
                    return Token::CharConst;
                } else {
                    // Trying next automaton 'Oper'
                    state = StateId::Oper_q0;
                }
                break;
            case StateId::Char_q5:
                if (ch == '\'') {
                    text += ch;
                    state = StateId::Char_q1;
                    ch = getNextChar();
                } else {
                    state = StateId::Start_q0;
                    throw "Char literal can't be empty";
                }
                break;
            // Comments
            case StateId::Comments_q0:
                if (ch == '/') {
                    state = StateId::Comments_q1;
                    ch = getNextChar();
                } else if (ch == '*') {
                    state = StateId::Comments_q3;
                    ch = getNextChar();
                } else {
                    // Trying next automaton 'Oper'
                    state = StateId::Oper_q0;
                }
                break;
            case StateId::Comments_q1:
                if (ch == EOF) {
                    state = StateId::Start_q0;
                    ch = getNextChar();
                } else if (ch == '\n') {
                    state = StateId::Start_q0;
                    ch = getNextChar();
                } else {
                    state = StateId::Comments_q1;
                    ch = getNextChar();
                }
                break;
            case StateId::Comments_q3:
                if (ch == '*') {
                    state = StateId::Comments_q4;
                    ch = getNextChar();
                } else if (ch == EOF) {
                    state = StateId::Start_q0;
                    ch = getNextChar();
                } else {
                    state = StateId::Comments_q3;
                    ch = getNextChar();
                }
                break;
            case StateId::Comments_q4:
                if (ch == '/') {
                    state = StateId::Start_q0;
                    ch = getNextChar();
                } else if (ch == EOF) {
                    state = StateId::Start_q0;
                    ch = getNextChar();
                } else {
                    state = StateId::Comments_q3;
                }
                break;
            // Dec
            case StateId::Dec_q0:
                if (isdigit(ch)) {
                    text += ch;
                    state = StateId::Dec_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::DecConst;
                }
                break;
            // Iden
            case StateId::Iden_q0:
                if ((ch >= '0') && (ch <= '9')) {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else if ((ch >= 'a') && (ch <= 'z')) {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else if ((ch >= 'A') && (ch <= 'Z')) {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else if (ch == '_') {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::Iden;
                }
                break;
            // KewA
            case StateId::KewA_q0:
                if (ch == 'r') {
                    text += ch;
                    state = StateId::KewA_q1;
                    ch = getNextChar();
                } else if (ch == 'R') {
                    text += ch;
                    state = StateId::KewA_q1;
                    ch = getNextChar();
                } else if (ch == 'b') {
                    text += ch;
                    state = StateId::KewA_q12;
                    ch = getNextChar();
                } else if (ch == 'B') {
                    text += ch;
                    state = StateId::KewA_q12;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewA_q1:
                if (ch == 'c') {
                    text += ch;
                    state = StateId::KewA_q7;
                    ch = getNextChar();
                } else if (ch == 'r') {
                    text += ch;
                    state = StateId::KewA_q2;
                    ch = getNextChar();
                } else if (ch == 'C') {
                    text += ch;
                    state = StateId::KewA_q7;
                    ch = getNextChar();
                } else if (ch == 'R') {
                    text += ch;
                    state = StateId::KewA_q2;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewA_q10:
                if (ch == 'O') {
                    text += ch;
                    state = StateId::KewA_q19;
                    ch = getNextChar();
                } else if (ch == 'o') {
                    text += ch;
                    state = StateId::KewA_q19;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewA_q12:
                if (ch == 'R') {
                    text += ch;
                    state = StateId::KewA_q13;
                    ch = getNextChar();
                } else if (ch == 'r') {
                    text += ch;
                    state = StateId::KewA_q13;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewA_q13:
                if (ch == 'i') {
                    text += ch;
                    state = StateId::KewA_q14;
                    ch = getNextChar();
                } else if (ch == 'I') {
                    text += ch;
                    state = StateId::KewA_q14;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewA_q14:
                if (ch == 'R') {
                    text += ch;
                    state = StateId::KewA_q17;
                    ch = getNextChar();
                } else if (ch == 'r') {
                    text += ch;
                    state = StateId::KewA_q17;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewA_q17:
                if (ch == ' ') {
                    return Token::KwAbrir;
                } else if (ch == '\t') {
                    return Token::KwAbrir;
                } else if (ch == EOF) {
                    return Token::KwAbrir;
                } else if (ch == '\n') {
                    return Token::KwAbrir;
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewA_q18:
                if (ch == ' ') {
                    return Token::KwArreglo;
                } else if (ch == '\t') {
                    return Token::KwArreglo;
                } else if (ch == EOF) {
                    return Token::KwArreglo;
                } else if (ch == '\n') {
                    return Token::KwArreglo;
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewA_q19:
                if (ch == ' ') {
                    return Token::KwArchivo;
                } else if (ch == '\t') {
                    return Token::KwArchivo;
                } else if (ch == '\n') {
                    return Token::KwArchivo;
                } else if (ch == EOF) {
                    return Token::KwArchivo;
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewA_q2:
                if (ch == 'E') {
                    text += ch;
                    state = StateId::KewA_q3;
                    ch = getNextChar();
                } else if (ch == 'e') {
                    text += ch;
                    state = StateId::KewA_q3;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewA_q3:
                if (ch == 'G') {
                    text += ch;
                    state = StateId::KewA_q4;
                    ch = getNextChar();
                } else if (ch == 'g') {
                    text += ch;
                    state = StateId::KewA_q4;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewA_q4:
                if (ch == 'l') {
                    text += ch;
                    state = StateId::KewA_q5;
                    ch = getNextChar();
                } else if (ch == 'L') {
                    text += ch;
                    state = StateId::KewA_q5;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewA_q5:
                if (ch == 'o') {
                    text += ch;
                    state = StateId::KewA_q18;
                    ch = getNextChar();
                } else if (ch == 'O') {
                    text += ch;
                    state = StateId::KewA_q18;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewA_q7:
                if (ch == 'H') {
                    text += ch;
                    state = StateId::KewA_q8;
                    ch = getNextChar();
                } else if (ch == 'h') {
                    text += ch;
                    state = StateId::KewA_q8;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewA_q8:
                if (ch == 'I') {
                    text += ch;
                    state = StateId::KewA_q9;
                    ch = getNextChar();
                } else if (ch == 'i') {
                    text += ch;
                    state = StateId::KewA_q9;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewA_q9:
                if (ch == 'V') {
                    text += ch;
                    state = StateId::KewA_q10;
                    ch = getNextChar();
                } else if (ch == 'v') {
                    text += ch;
                    state = StateId::KewA_q10;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            // KewB
            case StateId::KewB_q0:
                if (ch == 'O') {
                    text += ch;
                    state = StateId::KewB_q1;
                    ch = getNextChar();
                } else if (ch == 'o') {
                    text += ch;
                    state = StateId::KewB_q1;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewB_q1:
                if (ch == 'O') {
                    text += ch;
                    state = StateId::KewB_q2;
                    ch = getNextChar();
                } else if (ch == 'o') {
                    text += ch;
                    state = StateId::KewB_q2;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewB_q2:
                if (ch == 'L') {
                    text += ch;
                    state = StateId::KewB_q3;
                    ch = getNextChar();
                } else if (ch == 'l') {
                    text += ch;
                    state = StateId::KewB_q3;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewB_q3:
                if (ch == 'e') {
                    text += ch;
                    state = StateId::KewB_q4;
                    ch = getNextChar();
                } else if (ch == 'E') {
                    text += ch;
                    state = StateId::KewB_q4;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewB_q4:
                if (ch == 'A') {
                    text += ch;
                    state = StateId::KewB_q5;
                    ch = getNextChar();
                } else if (ch == 'a') {
                    text += ch;
                    state = StateId::KewB_q5;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewB_q5:
                if (ch == 'N') {
                    text += ch;
                    state = StateId::KewB_q6;
                    ch = getNextChar();
                } else if (ch == 'n') {
                    text += ch;
                    state = StateId::KewB_q6;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewB_q6:
                if (ch == 'o') {
                    text += ch;
                    state = StateId::KewB_q7;
                    ch = getNextChar();
                } else if (ch == 'O') {
                    text += ch;
                    state = StateId::KewB_q7;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewB_q7:
                if (ch == ' ') {
                    return Token::KwBooleano;
                } else if (ch == EOF) {
                    return Token::KwBooleano;
                } else if (ch == '\n') {
                    return Token::KwBooleano;
                } else if (ch == '\t') {
                    return Token::KwBooleano;
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            // KewC
            case StateId::KewC_q0:
                if (ch == 'o') {
                    text += ch;
                    state = StateId::KewC_q20;
                    ch = getNextChar();
                } else if (ch == 'a') {
                    text += ch;
                    state = StateId::KewC_q1;
                    ch = getNextChar();
                } else if (ch == 'O') {
                    text += ch;
                    state = StateId::KewC_q20;
                    ch = getNextChar();
                } else if (ch == 'A') {
                    text += ch;
                    state = StateId::KewC_q1;
                    ch = getNextChar();
                } else if (ch == 'E') {
                    text += ch;
                    state = StateId::KewC_q14;
                    ch = getNextChar();
                } else if (ch == 'e') {
                    text += ch;
                    state = StateId::KewC_q14;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewC_q1:
                if (ch == 'D') {
                    text += ch;
                    state = StateId::KewC_q2;
                    ch = getNextChar();
                } else if (ch == 's') {
                    text += ch;
                    state = StateId::KewC_q12;
                    ch = getNextChar();
                } else if (ch == 'd') {
                    text += ch;
                    state = StateId::KewC_q2;
                    ch = getNextChar();
                } else if (ch == 'S') {
                    text += ch;
                    state = StateId::KewC_q12;
                    ch = getNextChar();
                } else if (ch == 'R') {
                    text += ch;
                    state = StateId::KewC_q7;
                    ch = getNextChar();
                } else if (ch == 'r') {
                    text += ch;
                    state = StateId::KewC_q7;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewC_q10:
                if (ch == 'e') {
                    text += ch;
                    state = StateId::KewC_q6;
                    ch = getNextChar();
                } else if (ch == 'E') {
                    text += ch;
                    state = StateId::KewC_q6;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewC_q12:
                if (ch == 'o') {
                    text += ch;
                    state = StateId::KewC_q23;
                    ch = getNextChar();
                } else if (ch == 'O') {
                    text += ch;
                    state = StateId::KewC_q23;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewC_q14:
                if (ch == 'r') {
                    text += ch;
                    state = StateId::KewC_q16;
                    ch = getNextChar();
                } else if (ch == 'R') {
                    text += ch;
                    state = StateId::KewC_q16;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewC_q16:
                if (ch == 'R') {
                    text += ch;
                    state = StateId::KewC_q17;
                    ch = getNextChar();
                } else if (ch == 'r') {
                    text += ch;
                    state = StateId::KewC_q17;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewC_q17:
                if (ch == 'a') {
                    text += ch;
                    state = StateId::KewC_q18;
                    ch = getNextChar();
                } else if (ch == 'A') {
                    text += ch;
                    state = StateId::KewC_q18;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewC_q18:
                if (ch == 'r') {
                    text += ch;
                    state = StateId::KewC_q26;
                    ch = getNextChar();
                } else if (ch == 'R') {
                    text += ch;
                    state = StateId::KewC_q26;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewC_q2:
                if (ch == 'e') {
                    text += ch;
                    state = StateId::KewC_q3;
                    ch = getNextChar();
                } else if (ch == 'E') {
                    text += ch;
                    state = StateId::KewC_q3;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewC_q20:
                if (ch == 'm') {
                    text += ch;
                    state = StateId::KewC_q21;
                    ch = getNextChar();
                } else if (ch == 'M') {
                    text += ch;
                    state = StateId::KewC_q21;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewC_q21:
                if (ch == 'o') {
                    text += ch;
                    state = StateId::KewC_q27;
                    ch = getNextChar();
                } else if (ch == 'O') {
                    text += ch;
                    state = StateId::KewC_q27;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewC_q23:
                if (ch == '\t') {
                    return Token::KwCaso;
                } else if (ch == EOF) {
                    return Token::KwCaso;
                } else if (ch == '\n') {
                    return Token::KwCaso;
                } else if (ch == ' ') {
                    return Token::KwCaso;
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewC_q24:
                if (ch == ' ') {
                    return Token::KwCadena;
                } else if (ch == EOF) {
                    return Token::KwCadena;
                } else if (ch == '\n') {
                    return Token::KwCadena;
                } else if (ch == '\t') {
                    return Token::KwCadena;
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewC_q25:
                if (ch == ' ') {
                    return Token::KwCaracter;
                } else if (ch == '\n') {
                    return Token::KwCaracter;
                } else if (ch == EOF) {
                    return Token::KwCaracter;
                } else if (ch == '\t') {
                    return Token::KwCaracter;
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewC_q26:
                if (ch == ' ') {
                    return Token::KwCerrar;
                } else if (ch == '\t') {
                    return Token::KwCerrar;
                } else if (ch == '\n') {
                    return Token::KwCerrar;
                } else if (ch == EOF) {
                    return Token::KwCerrar;
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewC_q27:
                if (ch == '\n') {
                    return Token::KwComo;
                } else if (ch == EOF) {
                    return Token::KwComo;
                } else if (ch == '\t') {
                    return Token::KwComo;
                } else if (ch == ' ') {
                    return Token::KwComo;
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewC_q3:
                if (ch == 'n') {
                    text += ch;
                    state = StateId::KewC_q4;
                    ch = getNextChar();
                } else if (ch == 'N') {
                    text += ch;
                    state = StateId::KewC_q4;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewC_q4:
                if (ch == 'A') {
                    text += ch;
                    state = StateId::KewC_q24;
                    ch = getNextChar();
                } else if (ch == 'a') {
                    text += ch;
                    state = StateId::KewC_q24;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewC_q6:
                if (ch == 'R') {
                    text += ch;
                    state = StateId::KewC_q25;
                    ch = getNextChar();
                } else if (ch == 'r') {
                    text += ch;
                    state = StateId::KewC_q25;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewC_q7:
                if (ch == 'a') {
                    text += ch;
                    state = StateId::KewC_q8;
                    ch = getNextChar();
                } else if (ch == 'A') {
                    text += ch;
                    state = StateId::KewC_q8;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewC_q8:
                if (ch == 'c') {
                    text += ch;
                    state = StateId::KewC_q9;
                    ch = getNextChar();
                } else if (ch == 'C') {
                    text += ch;
                    state = StateId::KewC_q9;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewC_q9:
                if (ch == 'T') {
                    text += ch;
                    state = StateId::KewC_q10;
                    ch = getNextChar();
                } else if (ch == 't') {
                    text += ch;
                    state = StateId::KewC_q10;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            // KewD
            case StateId::KewD_q0:
                if (ch == 'e') {
                    text += ch;
                    state = StateId::KewD_q4;
                    ch = getNextChar();
                } else if (ch == 'E') {
                    text += ch;
                    state = StateId::KewD_q4;
                    ch = getNextChar();
                } else if (ch == 'i') {
                    text += ch;
                    state = StateId::KewD_q1;
                    ch = getNextChar();
                } else if (ch == 'I') {
                    text += ch;
                    state = StateId::KewD_q1;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewD_q1:
                if (ch == 'v') {
                    text += ch;
                    state = StateId::KewD_q3;
                    ch = getNextChar();
                } else if (ch == 'V') {
                    text += ch;
                    state = StateId::KewD_q3;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewD_q3:
                if (ch == ' ') {
                    return Token::KwDiv;
                } else if (ch == EOF) {
                    return Token::KwDiv;
                } else if (ch == '\t') {
                    return Token::KwDiv;
                } else if (ch == '\n') {
                    return Token::KwDiv;
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewD_q4:
                if (ch == '\n') {
                    return Token::KwDe;
                } else if (ch == EOF) {
                    return Token::KwDe;
                } else if (ch == ' ') {
                    return Token::KwDe;
                } else if (ch == '\t') {
                    return Token::KwDe;
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            // KewE
            case StateId::KewE_q0:
                if (ch == 'n') {
                    text += ch;
                    state = StateId::KewE_q1;
                    ch = getNextChar();
                } else if (ch == 's') {
                    text += ch;
                    state = StateId::KewE_q12;
                    ch = getNextChar();
                } else if (ch == 'N') {
                    text += ch;
                    state = StateId::KewE_q1;
                    ch = getNextChar();
                } else if (ch == 'S') {
                    text += ch;
                    state = StateId::KewE_q12;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewE_q1:
                if (ch == 't') {
                    text += ch;
                    state = StateId::KewE_q2;
                    ch = getNextChar();
                } else if (ch == 'T') {
                    text += ch;
                    state = StateId::KewE_q2;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewE_q10:
                if (ch == 's') {
                    text += ch;
                    state = StateId::KewE_q29;
                    ch = getNextChar();
                } else if (ch == 'S') {
                    text += ch;
                    state = StateId::KewE_q29;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewE_q11:
                if (ch == 'T') {
                    text += ch;
                    state = StateId::KewE_q18;
                    ch = getNextChar();
                } else if (ch == 't') {
                    text += ch;
                    state = StateId::KewE_q18;
                    ch = getNextChar();
                } else if (ch == 'B') {
                    text += ch;
                    state = StateId::KewE_q16;
                    ch = getNextChar();
                } else if (ch == 'b') {
                    text += ch;
                    state = StateId::KewE_q16;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewE_q12:
                if (ch == 'C') {
                    text += ch;
                    state = StateId::KewE_q13;
                    ch = getNextChar();
                } else if (ch == ' ') {
                    return Token::KwEs;
                } else if (ch == 'c') {
                    text += ch;
                    state = StateId::KewE_q13;
                    ch = getNextChar();
                } else if (ch == EOF) {
                    return Token::KwEs;
                } else if (ch == '\n') {
                    return Token::KwEs;
                } else if (ch == '\t') {
                    return Token::KwEs;
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewE_q13:
                if (ch == 'r') {
                    text += ch;
                    state = StateId::KewE_q14;
                    ch = getNextChar();
                } else if (ch == 'R') {
                    text += ch;
                    state = StateId::KewE_q14;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewE_q14:
                if (ch == 'i') {
                    text += ch;
                    state = StateId::KewE_q11;
                    ch = getNextChar();
                } else if (ch == 'I') {
                    text += ch;
                    state = StateId::KewE_q11;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewE_q16:
                if (ch == 'a') {
                    text += ch;
                    state = StateId::KewE_q27;
                    ch = getNextChar();
                } else if (ch == 'i') {
                    text += ch;
                    state = StateId::KewE_q22;
                    ch = getNextChar();
                } else if (ch == 'A') {
                    text += ch;
                    state = StateId::KewE_q27;
                    ch = getNextChar();
                } else if (ch == 'I') {
                    text += ch;
                    state = StateId::KewE_q22;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewE_q18:
                if (ch == 'u') {
                    text += ch;
                    state = StateId::KewE_q19;
                    ch = getNextChar();
                } else if (ch == 'U') {
                    text += ch;
                    state = StateId::KewE_q19;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewE_q19:
                if (ch == 'r') {
                    text += ch;
                    state = StateId::KewE_q20;
                    ch = getNextChar();
                } else if (ch == 'R') {
                    text += ch;
                    state = StateId::KewE_q20;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewE_q2:
                if (ch == 'O') {
                    text += ch;
                    state = StateId::KewE_q7;
                    ch = getNextChar();
                } else if (ch == 'o') {
                    text += ch;
                    state = StateId::KewE_q7;
                    ch = getNextChar();
                } else if (ch == 'e') {
                    text += ch;
                    state = StateId::KewE_q3;
                    ch = getNextChar();
                } else if (ch == 'E') {
                    text += ch;
                    state = StateId::KewE_q3;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewE_q20:
                if (ch == 'A') {
                    text += ch;
                    state = StateId::KewE_q25;
                    ch = getNextChar();
                } else if (ch == 'a') {
                    text += ch;
                    state = StateId::KewE_q25;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewE_q22:
                if (ch == 'R') {
                    text += ch;
                    state = StateId::KewE_q26;
                    ch = getNextChar();
                } else if (ch == 'r') {
                    text += ch;
                    state = StateId::KewE_q26;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewE_q25:
                if (ch == ' ') {
                    return Token::KwEscritura;
                } else if (ch == '\t') {
                    return Token::KwEscritura;
                } else if (ch == EOF) {
                    return Token::KwEscritura;
                } else if (ch == '\n') {
                    return Token::KwEscritura;
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewE_q26:
                if (ch == ' ') {
                    return Token::KwEscribir;
                } else if (ch == '\t') {
                    return Token::KwEscribir;
                } else if (ch == EOF) {
                    return Token::KwEscribir;
                } else if (ch == '\n') {
                    return Token::KwEscribir;
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewE_q27:
                if (ch == '\t') {
                    return Token::KwEscriba;
                } else if (ch == '\n') {
                    return Token::KwEscriba;
                } else if (ch == EOF) {
                    return Token::KwEscriba;
                } else if (ch == ' ') {
                    return Token::KwEscriba;
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewE_q28:
                if (ch == ' ') {
                    return Token::KwEntero;
                } else if (ch == '\t') {
                    return Token::KwEntero;
                } else if (ch == '\n') {
                    return Token::KwEntero;
                } else if (ch == EOF) {
                    return Token::KwEntero;
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewE_q29:
                if (ch == ' ') {
                    return Token::KwEntonces;
                } else if (ch == EOF) {
                    return Token::KwEntonces;
                } else if (ch == '\n') {
                    return Token::KwEntonces;
                } else if (ch == '\t') {
                    return Token::KwEntonces;
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewE_q3:
                if (ch == 'R') {
                    text += ch;
                    state = StateId::KewE_q4;
                    ch = getNextChar();
                } else if (ch == 'r') {
                    text += ch;
                    state = StateId::KewE_q4;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewE_q4:
                if (ch == 'o') {
                    text += ch;
                    state = StateId::KewE_q28;
                    ch = getNextChar();
                } else if (ch == 'O') {
                    text += ch;
                    state = StateId::KewE_q28;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewE_q7:
                if (ch == 'N') {
                    text += ch;
                    state = StateId::KewE_q8;
                    ch = getNextChar();
                } else if (ch == 'n') {
                    text += ch;
                    state = StateId::KewE_q8;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewE_q8:
                if (ch == 'C') {
                    text += ch;
                    state = StateId::KewE_q9;
                    ch = getNextChar();
                } else if (ch == 'c') {
                    text += ch;
                    state = StateId::KewE_q9;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewE_q9:
                if (ch == 'e') {
                    text += ch;
                    state = StateId::KewE_q10;
                    ch = getNextChar();
                } else if (ch == 'E') {
                    text += ch;
                    state = StateId::KewE_q10;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            // KewF
            case StateId::KewF_q0:
                if (ch == 'u') {
                    text += ch;
                    state = StateId::KewF_q7;
                    ch = getNextChar();
                } else if (ch == 'I') {
                    text += ch;
                    state = StateId::KewF_q6;
                    ch = getNextChar();
                } else if (ch == 'U') {
                    text += ch;
                    state = StateId::KewF_q7;
                    ch = getNextChar();
                } else if (ch == 'i') {
                    text += ch;
                    state = StateId::KewF_q6;
                    ch = getNextChar();
                } else if (ch == 'A') {
                    text += ch;
                    state = StateId::KewF_q12;
                    ch = getNextChar();
                } else if (ch == 'a') {
                    text += ch;
                    state = StateId::KewF_q12;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewF_q1:
                if (ch == 'C') {
                    text += ch;
                    state = StateId::KewF_q2;
                    ch = getNextChar();
                } else if (ch == 'c') {
                    text += ch;
                    state = StateId::KewF_q2;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewF_q10:
                if (ch == 'l') {
                    text += ch;
                    state = StateId::KewF_q17;
                    ch = getNextChar();
                } else if (ch == 'L') {
                    text += ch;
                    state = StateId::KewF_q17;
                    ch = getNextChar();
                } else {
                    // Trying next automaton 'Oper'
                    state = StateId::Oper_q0;
                }
                break;
            case StateId::KewF_q12:
                if (ch == 'l') {
                    text += ch;
                    state = StateId::KewF_q13;
                    ch = getNextChar();
                } else if (ch == 'L') {
                    text += ch;
                    state = StateId::KewF_q13;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewF_q13:
                if (ch == 'S') {
                    text += ch;
                    state = StateId::KewF_q14;
                    ch = getNextChar();
                } else if (ch == 's') {
                    text += ch;
                    state = StateId::KewF_q14;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewF_q14:
                if (ch == 'o') {
                    text += ch;
                    state = StateId::KewF_q19;
                    ch = getNextChar();
                } else if (ch == 'O') {
                    text += ch;
                    state = StateId::KewF_q19;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewF_q17:
                if (ch == ' ') {
                    return Token::KwFinal;
                } else if (ch == '\t') {
                    return Token::KwFinal;
                } else if (ch == EOF) {
                    return Token::KwFinal;
                } else if (ch == '\n') {
                    return Token::KwFinal;
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewF_q18:
                if (ch == '\n') {
                    return Token::KwFuncion;
                } else if (ch == EOF) {
                    return Token::KwFuncion;
                } else if (ch == '\t') {
                    return Token::KwFuncion;
                } else if (ch == ' ') {
                    return Token::KwFuncion;
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewF_q19:
                if (ch == ' ') {
                    return Token::KwFalso;
                } else if (ch == '\t') {
                    return Token::KwFalso;
                } else if (ch == EOF) {
                    return Token::KwFalso;
                } else if (ch == '\n') {
                    return Token::KwFalso;
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewF_q2:
                if (ch == 'I') {
                    text += ch;
                    state = StateId::KewF_q3;
                    ch = getNextChar();
                } else if (ch == 'i') {
                    text += ch;
                    state = StateId::KewF_q3;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewF_q3:
                if (ch == 'o') {
                    text += ch;
                    state = StateId::KewF_q4;
                    ch = getNextChar();
                } else if (ch == 'O') {
                    text += ch;
                    state = StateId::KewF_q4;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewF_q4:
                if (ch == 'n') {
                    text += ch;
                    state = StateId::KewF_q18;
                    ch = getNextChar();
                } else if (ch == 'N') {
                    text += ch;
                    state = StateId::KewF_q18;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewF_q6:
                if (ch == 'N') {
                    text += ch;
                    state = StateId::KewF_q8;
                    ch = getNextChar();
                } else if (ch == 'n') {
                    text += ch;
                    state = StateId::KewF_q8;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewF_q7:
                if (ch == 'n') {
                    text += ch;
                    state = StateId::KewF_q1;
                    ch = getNextChar();
                } else if (ch == 'N') {
                    text += ch;
                    state = StateId::KewF_q1;
                    ch = getNextChar();
                } else {
                    // Trying next automaton 'Oper'
                    state = StateId::Oper_q0;
                }
                break;
            case StateId::KewF_q8:
                if (ch == '\t') {
                    return Token::KwFin;
                } else if (ch == '\n') {
                    return Token::KwFin;
                } else if (ch == EOF) {
                    return Token::KwFin;
                } else if (ch == 'A') {
                    text += ch;
                    state = StateId::KewF_q10;
                    ch = getNextChar();
                } else if (ch == 'a') {
                    text += ch;
                    state = StateId::KewF_q10;
                    ch = getNextChar();
                } else if (ch == ' ') {
                    return Token::KwFin;
                } else {
                    // Trying next automaton 'Oper'
                    state = StateId::Oper_q0;
                }
                break;
            // KewH
            case StateId::KewH_q0:
                if (ch == 'a') {
                    text += ch;
                    state = StateId::KewH_q1;
                    ch = getNextChar();
                } else if (ch == 'A') {
                    text += ch;
                    state = StateId::KewH_q1;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewH_q1:
                if (ch == 's') {
                    text += ch;
                    state = StateId::KewH_q4;
                    ch = getNextChar();
                } else if (ch == 'G') {
                    text += ch;
                    state = StateId::KewH_q2;
                    ch = getNextChar();
                } else if (ch == 'S') {
                    text += ch;
                    state = StateId::KewH_q4;
                    ch = getNextChar();
                } else if (ch == 'g') {
                    text += ch;
                    state = StateId::KewH_q2;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewH_q2:
                if (ch == 'a') {
                    text += ch;
                    state = StateId::KewH_q7;
                    ch = getNextChar();
                } else if (ch == 'A') {
                    text += ch;
                    state = StateId::KewH_q7;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewH_q4:
                if (ch == 'T') {
                    text += ch;
                    state = StateId::KewH_q5;
                    ch = getNextChar();
                } else if (ch == 't') {
                    text += ch;
                    state = StateId::KewH_q5;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewH_q5:
                if (ch == 'A') {
                    text += ch;
                    state = StateId::KewH_q8;
                    ch = getNextChar();
                } else if (ch == 'a') {
                    text += ch;
                    state = StateId::KewH_q8;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewH_q7:
                if (ch == EOF) {
                    return Token::KwHaga;
                } else if (ch == ' ') {
                    return Token::KwHaga;
                } else if (ch == '\n') {
                    return Token::KwHaga;
                } else if (ch == '\t') {
                    return Token::KwHaga;
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewH_q8:
                if (ch == '\t') {
                    return Token::KwHasta;
                } else if (ch == '\n') {
                    return Token::KwHasta;
                } else if (ch == ' ') {
                    return Token::KwHasta;
                } else if (ch == EOF) {
                    return Token::KwHasta;
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            // KewIden
            case StateId::KewIden_q0:
                if (ch == '_') {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else if (ch == 'y') {
                    text += ch;
                    state = StateId::KewY_q0;
                    ch = getNextChar();
                } else if (ch == 'i') {
                    text += ch;
                    state = StateId::KewI_q0;
                    ch = getNextChar();
                } else if (ch == 'N') {
                    text += ch;
                    state = StateId::KewN_q0;
                    ch = getNextChar();
                } else if (ch == 'D') {
                    text += ch;
                    state = StateId::KewD_q0;
                    ch = getNextChar();
                } else if (ch == 'm') {
                    text += ch;
                    state = StateId::KewM_q0;
                    ch = getNextChar();
                } else if (ch == 'S') {
                    text += ch;
                    state = StateId::KewS_q0;
                    ch = getNextChar();
                } else if (ch == 't') {
                    text += ch;
                    state = StateId::KewT_q0;
                    ch = getNextChar();
                } else if (ch == 'f') {
                    text += ch;
                    state = StateId::KewF_q0;
                    ch = getNextChar();
                } else if (ch == 'P') {
                    text += ch;
                    state = StateId::KewP_q0;
                    ch = getNextChar();
                } else if (ch == 'z') {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else if (ch == 'x') {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else if (ch == 'Y') {
                    text += ch;
                    state = StateId::KewY_q0;
                    ch = getNextChar();
                } else if (ch == 'n') {
                    text += ch;
                    state = StateId::KewN_q0;
                    ch = getNextChar();
                } else if (ch == 'd') {
                    text += ch;
                    state = StateId::KewD_q0;
                    ch = getNextChar();
                } else if (ch == 'I') {
                    text += ch;
                    state = StateId::KewI_q0;
                    ch = getNextChar();
                } else if (ch == 'M') {
                    text += ch;
                    state = StateId::KewM_q0;
                    ch = getNextChar();
                } else if (ch == 'q') {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else if (ch == 'w') {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else if (ch == 'u') {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else if (ch == 's') {
                    text += ch;
                    state = StateId::KewS_q0;
                    ch = getNextChar();
                } else if (ch == 'k') {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else if (ch == 'j') {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else if (ch == 'T') {
                    text += ch;
                    state = StateId::KewT_q0;
                    ch = getNextChar();
                } else if (ch == 'F') {
                    text += ch;
                    state = StateId::KewF_q0;
                    ch = getNextChar();
                } else if (ch == 'g') {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else if (ch == 'p') {
                    text += ch;
                    state = StateId::KewP_q0;
                    ch = getNextChar();
                } else if (ch == 'W') {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else if (ch == 'b') {
                    text += ch;
                    state = StateId::KewB_q0;
                    ch = getNextChar();
                } else if (ch == 'U') {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else if (ch == 'r') {
                    text += ch;
                    state = StateId::KewR_q0;
                    ch = getNextChar();
                } else if (ch == 'L') {
                    text += ch;
                    state = StateId::KewL_q0;
                    ch = getNextChar();
                } else if (ch == 'Q') {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else if (ch == 'E') {
                    text += ch;
                    state = StateId::KewE_q0;
                    ch = getNextChar();
                } else if (ch == 'v') {
                    text += ch;
                    state = StateId::KewV_q0;
                    ch = getNextChar();
                } else if (ch == 'Z') {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else if (ch == 'X') {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else if (ch == 'G') {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else if (ch == 'o') {
                    text += ch;
                    state = StateId::KewO_q0;
                    ch = getNextChar();
                } else if (ch == 'h') {
                    text += ch;
                    state = StateId::KewH_q0;
                    ch = getNextChar();
                } else if (ch == 'J') {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else if (ch == 'a') {
                    text += ch;
                    state = StateId::KewA_q0;
                    ch = getNextChar();
                } else if (ch == 'K') {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else if (ch == 'c') {
                    text += ch;
                    state = StateId::KewC_q0;
                    ch = getNextChar();
                } else if (ch == 'B') {
                    text += ch;
                    state = StateId::KewB_q0;
                    ch = getNextChar();
                } else if (ch == 'R') {
                    text += ch;
                    state = StateId::KewR_q0;
                    ch = getNextChar();
                } else if (ch == 'l') {
                    text += ch;
                    state = StateId::KewL_q0;
                    ch = getNextChar();
                } else if (ch == 'e') {
                    text += ch;
                    state = StateId::KewE_q0;
                    ch = getNextChar();
                } else if (ch == 'V') {
                    text += ch;
                    state = StateId::KewV_q0;
                    ch = getNextChar();
                } else if (ch == 'O') {
                    text += ch;
                    state = StateId::KewO_q0;
                    ch = getNextChar();
                } else if (ch == 'H') {
                    text += ch;
                    state = StateId::KewH_q0;
                    ch = getNextChar();
                } else if (ch == 'A') {
                    text += ch;
                    state = StateId::KewA_q0;
                    ch = getNextChar();
                } else if (ch == 'C') {
                    text += ch;
                    state = StateId::KewC_q0;
                    ch = getNextChar();
                } else {
                    // Trying next automaton 'Oper'
                    state = StateId::Oper_q0;
                }
                break;
            // KewI
            case StateId::KewI_q0:
                if (ch == 'n') {
                    text += ch;
                    state = StateId::KewI_q1;
                    ch = getNextChar();
                } else if (ch == 'N') {
                    text += ch;
                    state = StateId::KewI_q1;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewI_q1:
                if (ch == 'i') {
                    text += ch;
                    state = StateId::KewI_q2;
                    ch = getNextChar();
                } else if (ch == 'I') {
                    text += ch;
                    state = StateId::KewI_q2;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewI_q2:
                if (ch == 'c') {
                    text += ch;
                    state = StateId::KewI_q3;
                    ch = getNextChar();
                } else if (ch == 'C') {
                    text += ch;
                    state = StateId::KewI_q3;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewI_q3:
                if (ch == 'i') {
                    text += ch;
                    state = StateId::KewI_q4;
                    ch = getNextChar();
                } else if (ch == 'I') {
                    text += ch;
                    state = StateId::KewI_q4;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewI_q4:
                if (ch == 'O') {
                    text += ch;
                    state = StateId::KewI_q6;
                    ch = getNextChar();
                } else if (ch == 'o') {
                    text += ch;
                    state = StateId::KewI_q6;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewI_q6:
                if (ch == '\t') {
                    return Token::KwInicio;
                } else if (ch == EOF) {
                    return Token::KwInicio;
                } else if (ch == ' ') {
                    return Token::KwInicio;
                } else if (ch == '\n') {
                    return Token::KwInicio;
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            // KewL
            case StateId::KewL_q0:
                if (ch == 'e') {
                    text += ch;
                    state = StateId::KewL_q6;
                    ch = getNextChar();
                } else if (ch == 'E') {
                    text += ch;
                    state = StateId::KewL_q6;
                    ch = getNextChar();
                } else if (ch == 'l') {
                    text += ch;
                    state = StateId::KewL_q1;
                    ch = getNextChar();
                } else if (ch == 'L') {
                    text += ch;
                    state = StateId::KewL_q1;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewL_q1:
                if (ch == 'A') {
                    text += ch;
                    state = StateId::KewL_q2;
                    ch = getNextChar();
                } else if (ch == 'a') {
                    text += ch;
                    state = StateId::KewL_q2;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewL_q10:
                if (ch == 't') {
                    text += ch;
                    state = StateId::KewL_q11;
                    ch = getNextChar();
                } else if (ch == 'T') {
                    text += ch;
                    state = StateId::KewL_q11;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewL_q11:
                if (ch == 'u') {
                    text += ch;
                    state = StateId::KewL_q12;
                    ch = getNextChar();
                } else if (ch == 'U') {
                    text += ch;
                    state = StateId::KewL_q12;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewL_q12:
                if (ch == 'r') {
                    text += ch;
                    state = StateId::KewL_q13;
                    ch = getNextChar();
                } else if (ch == 'R') {
                    text += ch;
                    state = StateId::KewL_q13;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewL_q13:
                if (ch == 'a') {
                    text += ch;
                    state = StateId::KewL_q19;
                    ch = getNextChar();
                } else if (ch == 'A') {
                    text += ch;
                    state = StateId::KewL_q19;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewL_q16:
                if (ch == '\n') {
                    return Token::KwLlamar;
                } else if (ch == EOF) {
                    return Token::KwLlamar;
                } else if (ch == '\t') {
                    return Token::KwLlamar;
                } else if (ch == ' ') {
                    return Token::KwLlamar;
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewL_q17:
                if (ch == ' ') {
                    return Token::KwLea;
                } else if (ch == '\t') {
                    return Token::KwLea;
                } else if (ch == EOF) {
                    return Token::KwLea;
                } else if (ch == '\n') {
                    return Token::KwLea;
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewL_q18:
                if (ch == '\n') {
                    return Token::KwLeer;
                } else if (ch == EOF) {
                    return Token::KwLeer;
                } else if (ch == '\t') {
                    return Token::KwLeer;
                } else if (ch == ' ') {
                    return Token::KwLeer;
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewL_q19:
                if (ch == ' ') {
                    return Token::KwLectura;
                } else if (ch == '\n') {
                    return Token::KwLectura;
                } else if (ch == EOF) {
                    return Token::KwLectura;
                } else if (ch == '\t') {
                    return Token::KwLectura;
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewL_q2:
                if (ch == 'M') {
                    text += ch;
                    state = StateId::KewL_q3;
                    ch = getNextChar();
                } else if (ch == 'm') {
                    text += ch;
                    state = StateId::KewL_q3;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewL_q3:
                if (ch == 'A') {
                    text += ch;
                    state = StateId::KewL_q4;
                    ch = getNextChar();
                } else if (ch == 'a') {
                    text += ch;
                    state = StateId::KewL_q4;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewL_q4:
                if (ch == 'R') {
                    text += ch;
                    state = StateId::KewL_q16;
                    ch = getNextChar();
                } else if (ch == 'r') {
                    text += ch;
                    state = StateId::KewL_q16;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewL_q6:
                if (ch == 'A') {
                    text += ch;
                    state = StateId::KewL_q17;
                    ch = getNextChar();
                } else if (ch == 'a') {
                    text += ch;
                    state = StateId::KewL_q17;
                    ch = getNextChar();
                } else if (ch == 'e') {
                    text += ch;
                    state = StateId::KewL_q8;
                    ch = getNextChar();
                } else if (ch == 'c') {
                    text += ch;
                    state = StateId::KewL_q10;
                    ch = getNextChar();
                } else if (ch == 'E') {
                    text += ch;
                    state = StateId::KewL_q8;
                    ch = getNextChar();
                } else if (ch == 'C') {
                    text += ch;
                    state = StateId::KewL_q10;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewL_q8:
                if (ch == 'R') {
                    text += ch;
                    state = StateId::KewL_q18;
                    ch = getNextChar();
                } else if (ch == 'r') {
                    text += ch;
                    state = StateId::KewL_q18;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            // KewM
            case StateId::KewM_q0:
                if (ch == 'i') {
                    text += ch;
                    state = StateId::KewM_q1;
                    ch = getNextChar();
                } else if (ch == 'O') {
                    text += ch;
                    state = StateId::KewM_q9;
                    ch = getNextChar();
                } else if (ch == 'I') {
                    text += ch;
                    state = StateId::KewM_q1;
                    ch = getNextChar();
                } else if (ch == 'o') {
                    text += ch;
                    state = StateId::KewM_q9;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewM_q1:
                if (ch == 'e') {
                    text += ch;
                    state = StateId::KewM_q2;
                    ch = getNextChar();
                } else if (ch == 'E') {
                    text += ch;
                    state = StateId::KewM_q2;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewM_q11:
                if (ch == EOF) {
                    return Token::KwMod;
                } else if (ch == ' ') {
                    return Token::KwMod;
                } else if (ch == '\n') {
                    return Token::KwMod;
                } else if (ch == '\t') {
                    return Token::KwMod;
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewM_q2:
                if (ch == 'N') {
                    text += ch;
                    state = StateId::KewM_q3;
                    ch = getNextChar();
                } else if (ch == 'n') {
                    text += ch;
                    state = StateId::KewM_q3;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewM_q3:
                if (ch == 't') {
                    text += ch;
                    state = StateId::KewM_q4;
                    ch = getNextChar();
                } else if (ch == 'T') {
                    text += ch;
                    state = StateId::KewM_q4;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewM_q4:
                if (ch == 'R') {
                    text += ch;
                    state = StateId::KewM_q5;
                    ch = getNextChar();
                } else if (ch == 'r') {
                    text += ch;
                    state = StateId::KewM_q5;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewM_q5:
                if (ch == 'A') {
                    text += ch;
                    state = StateId::KewM_q7;
                    ch = getNextChar();
                } else if (ch == 'a') {
                    text += ch;
                    state = StateId::KewM_q7;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewM_q6:
                if (ch == '\t') {
                    return Token::KwMientras;
                } else if (ch == ' ') {
                    return Token::KwMientras;
                } else if (ch == EOF) {
                    return Token::KwMientras;
                } else if (ch == '\n') {
                    return Token::KwMientras;
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewM_q7:
                if (ch == 'S') {
                    text += ch;
                    state = StateId::KewM_q6;
                    ch = getNextChar();
                } else if (ch == 's') {
                    text += ch;
                    state = StateId::KewM_q6;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewM_q9:
                if (ch == 'd') {
                    text += ch;
                    state = StateId::KewM_q11;
                    ch = getNextChar();
                } else if (ch == 'D') {
                    text += ch;
                    state = StateId::KewM_q11;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            // KewN
            case StateId::KewN_q0:
                if (ch == 'o') {
                    text += ch;
                    state = StateId::KewN_q2;
                    ch = getNextChar();
                } else if (ch == 'O') {
                    text += ch;
                    state = StateId::KewN_q2;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewN_q2:
                if (ch == '\t') {
                    return Token::KwNo;
                } else if (ch == EOF) {
                    return Token::KwNo;
                } else if (ch == ' ') {
                    return Token::KwNo;
                } else if (ch == '\n') {
                    return Token::KwNo;
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            // KewO
            case StateId::KewO_q0:
                if (ch == '\t') {
                    return Token::KwO;
                } else if (ch == EOF) {
                    return Token::KwO;
                } else if (ch == ' ') {
                    return Token::KwO;
                } else if (ch == '\n') {
                    return Token::KwO;
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            // KewP
            case StateId::KewP_q0:
                if (ch == 'r') {
                    text += ch;
                    state = StateId::KewP_q4;
                    ch = getNextChar();
                } else if (ch == 'A') {
                    text += ch;
                    state = StateId::KewP_q1;
                    ch = getNextChar();
                } else if (ch == 'R') {
                    text += ch;
                    state = StateId::KewP_q4;
                    ch = getNextChar();
                } else if (ch == 'a') {
                    text += ch;
                    state = StateId::KewP_q1;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewP_q1:
                if (ch == 'r') {
                    text += ch;
                    state = StateId::KewP_q2;
                    ch = getNextChar();
                } else if (ch == 'R') {
                    text += ch;
                    state = StateId::KewP_q2;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewP_q10:
                if (ch == 'i') {
                    text += ch;
                    state = StateId::KewP_q11;
                    ch = getNextChar();
                } else if (ch == 'I') {
                    text += ch;
                    state = StateId::KewP_q11;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewP_q11:
                if (ch == 'E') {
                    text += ch;
                    state = StateId::KewP_q12;
                    ch = getNextChar();
                } else if (ch == 'e') {
                    text += ch;
                    state = StateId::KewP_q12;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewP_q12:
                if (ch == 'N') {
                    text += ch;
                    state = StateId::KewP_q13;
                    ch = getNextChar();
                } else if (ch == 'n') {
                    text += ch;
                    state = StateId::KewP_q13;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewP_q13:
                if (ch == 'T') {
                    text += ch;
                    state = StateId::KewP_q14;
                    ch = getNextChar();
                } else if (ch == 't') {
                    text += ch;
                    state = StateId::KewP_q14;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewP_q14:
                if (ch == 'O') {
                    text += ch;
                    state = StateId::KewP_q18;
                    ch = getNextChar();
                } else if (ch == 'o') {
                    text += ch;
                    state = StateId::KewP_q18;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewP_q17:
                if (ch == '\t') {
                    return Token::KwPara;
                } else if (ch == '\n') {
                    return Token::KwPara;
                } else if (ch == EOF) {
                    return Token::KwPara;
                } else if (ch == ' ') {
                    return Token::KwPara;
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewP_q18:
                if (ch == '\n') {
                    return Token::KwProcedimiento;
                } else if (ch == EOF) {
                    return Token::KwProcedimiento;
                } else if (ch == '\t') {
                    return Token::KwProcedimiento;
                } else if (ch == ' ') {
                    return Token::KwProcedimiento;
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewP_q2:
                if (ch == 'A') {
                    text += ch;
                    state = StateId::KewP_q17;
                    ch = getNextChar();
                } else if (ch == 'a') {
                    text += ch;
                    state = StateId::KewP_q17;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewP_q4:
                if (ch == 'o') {
                    text += ch;
                    state = StateId::KewP_q5;
                    ch = getNextChar();
                } else if (ch == 'O') {
                    text += ch;
                    state = StateId::KewP_q5;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewP_q5:
                if (ch == 'C') {
                    text += ch;
                    state = StateId::KewP_q6;
                    ch = getNextChar();
                } else if (ch == 'c') {
                    text += ch;
                    state = StateId::KewP_q6;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewP_q6:
                if (ch == 'E') {
                    text += ch;
                    state = StateId::KewP_q7;
                    ch = getNextChar();
                } else if (ch == 'e') {
                    text += ch;
                    state = StateId::KewP_q7;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewP_q7:
                if (ch == 'd') {
                    text += ch;
                    state = StateId::KewP_q8;
                    ch = getNextChar();
                } else if (ch == 'D') {
                    text += ch;
                    state = StateId::KewP_q8;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewP_q8:
                if (ch == 'I') {
                    text += ch;
                    state = StateId::KewP_q9;
                    ch = getNextChar();
                } else if (ch == 'i') {
                    text += ch;
                    state = StateId::KewP_q9;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewP_q9:
                if (ch == 'm') {
                    text += ch;
                    state = StateId::KewP_q10;
                    ch = getNextChar();
                } else if (ch == 'M') {
                    text += ch;
                    state = StateId::KewP_q10;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            // KewR
            case StateId::KewR_q0:
                if (ch == 'e') {
                    text += ch;
                    state = StateId::KewR_q1;
                    ch = getNextChar();
                } else if (ch == 'E') {
                    text += ch;
                    state = StateId::KewR_q1;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewR_q1:
                if (ch == 'P') {
                    text += ch;
                    state = StateId::KewR_q9;
                    ch = getNextChar();
                } else if (ch == 't') {
                    text += ch;
                    state = StateId::KewR_q6;
                    ch = getNextChar();
                } else if (ch == 'p') {
                    text += ch;
                    state = StateId::KewR_q9;
                    ch = getNextChar();
                } else if (ch == 'T') {
                    text += ch;
                    state = StateId::KewR_q6;
                    ch = getNextChar();
                } else if (ch == 'a') {
                    text += ch;
                    state = StateId::KewR_q2;
                    ch = getNextChar();
                } else if (ch == 'G') {
                    text += ch;
                    state = StateId::KewR_q13;
                    ch = getNextChar();
                } else if (ch == 'A') {
                    text += ch;
                    state = StateId::KewR_q2;
                    ch = getNextChar();
                } else if (ch == 'g') {
                    text += ch;
                    state = StateId::KewR_q13;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewR_q10:
                if (ch == 'n') {
                    text += ch;
                    state = StateId::KewR_q11;
                    ch = getNextChar();
                } else if (ch == 'N') {
                    text += ch;
                    state = StateId::KewR_q11;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewR_q11:
                if (ch == 'E') {
                    text += ch;
                    state = StateId::KewR_q20;
                    ch = getNextChar();
                } else if (ch == 'e') {
                    text += ch;
                    state = StateId::KewR_q20;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewR_q13:
                if (ch == 'i') {
                    text += ch;
                    state = StateId::KewR_q14;
                    ch = getNextChar();
                } else if (ch == 'I') {
                    text += ch;
                    state = StateId::KewR_q14;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewR_q14:
                if (ch == 'S') {
                    text += ch;
                    state = StateId::KewR_q16;
                    ch = getNextChar();
                } else if (ch == 's') {
                    text += ch;
                    state = StateId::KewR_q16;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewR_q16:
                if (ch == 't') {
                    text += ch;
                    state = StateId::KewR_q17;
                    ch = getNextChar();
                } else if (ch == 'T') {
                    text += ch;
                    state = StateId::KewR_q17;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewR_q17:
                if (ch == 'R') {
                    text += ch;
                    state = StateId::KewR_q18;
                    ch = getNextChar();
                } else if (ch == 'r') {
                    text += ch;
                    state = StateId::KewR_q18;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewR_q18:
                if (ch == 'O') {
                    text += ch;
                    state = StateId::KewR_q21;
                    ch = getNextChar();
                } else if (ch == 'o') {
                    text += ch;
                    state = StateId::KewR_q21;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewR_q2:
                if (ch == 'L') {
                    text += ch;
                    return Token::KwReal;
                } else if (ch == 'l') {
                    text += ch;
                    return Token::KwReal;
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewR_q20:
                if (ch == '\t') {
                    return Token::KwRetorne;
                } else if (ch == EOF) {
                    return Token::KwRetorne;
                } else if (ch == '\n') {
                    return Token::KwRetorne;
                } else if (ch == ' ') {
                    return Token::KwRetorne;
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewR_q21:
                if (ch == ' ') {
                    return Token::KwRegistro;
                } else if (ch == EOF) {
                    return Token::KwRegistro;
                } else if (ch == '\n') {
                    return Token::KwRegistro;
                } else if (ch == '\t') {
                    return Token::KwRegistro;
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewR_q22:
                if (ch == '\t') {
                    return Token::KwRepita;
                } else if (ch == '\n') {
                    return Token::KwRepita;
                } else if (ch == EOF) {
                    return Token::KwRepita;
                } else if (ch == ' ') {
                    return Token::KwRepita;
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewR_q4:
                if (ch == 'T') {
                    text += ch;
                    state = StateId::KewR_q5;
                    ch = getNextChar();
                } else if (ch == 't') {
                    text += ch;
                    state = StateId::KewR_q5;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewR_q5:
                if (ch == 'A') {
                    text += ch;
                    state = StateId::KewR_q22;
                    ch = getNextChar();
                } else if (ch == 'a') {
                    text += ch;
                    state = StateId::KewR_q22;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewR_q6:
                if (ch == 'o') {
                    text += ch;
                    state = StateId::KewR_q8;
                    ch = getNextChar();
                } else if (ch == 'O') {
                    text += ch;
                    state = StateId::KewR_q8;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewR_q8:
                if (ch == 'R') {
                    text += ch;
                    state = StateId::KewR_q10;
                    ch = getNextChar();
                } else if (ch == 'r') {
                    text += ch;
                    state = StateId::KewR_q10;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewR_q9:
                if (ch == 'I') {
                    text += ch;
                    state = StateId::KewR_q4;
                    ch = getNextChar();
                } else if (ch == 'i') {
                    text += ch;
                    state = StateId::KewR_q4;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            // KewS
            case StateId::KewS_q0:
                if (ch == 'i') {
                    text += ch;
                    state = StateId::KewS_q1;
                    ch = getNextChar();
                } else if (ch == 'I') {
                    text += ch;
                    state = StateId::KewS_q1;
                    ch = getNextChar();
                } else if (ch == 'e') {
                    text += ch;
                    state = StateId::KewS_q4;
                    ch = getNextChar();
                } else if (ch == 'E') {
                    text += ch;
                    state = StateId::KewS_q4;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewS_q1:
                if (ch == ' ') {
                    return Token::KwSi;
                } else if (ch == EOF) {
                    return Token::KwSi;
                } else if (ch == 'n') {
                    text += ch;
                    state = StateId::KewS_q2;
                    ch = getNextChar();
                } else if (ch == '\n') {
                    return Token::KwSi;
                } else if (ch == '\t') {
                    return Token::KwSi;
                } else if (ch == 'N') {
                    text += ch;
                    state = StateId::KewS_q2;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewS_q10:
                if (ch == 'a') {
                    text += ch;
                    state = StateId::KewS_q11;
                    ch = getNextChar();
                } else if (ch == 'A') {
                    text += ch;
                    state = StateId::KewS_q11;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewS_q11:
                if (ch == 'l') {
                    text += ch;
                    state = StateId::KewS_q12;
                    ch = getNextChar();
                } else if (ch == 'L') {
                    text += ch;
                    state = StateId::KewS_q12;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewS_q12:
                if (ch == '\t') {
                    return Token::KwSecuencial;
                } else if (ch == EOF) {
                    return Token::KwSecuencial;
                } else if (ch == '\n') {
                    return Token::KwSecuencial;
                } else if (ch == ' ') {
                    return Token::KwSecuencial;
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewS_q2:
                if (ch == 'o') {
                    text += ch;
                    state = StateId::KewS_q3;
                    ch = getNextChar();
                } else if (ch == 'O') {
                    text += ch;
                    state = StateId::KewS_q3;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewS_q3:
                if (ch == '\t') {
                    return Token::KwSino;
                } else if (ch == EOF) {
                    return Token::KwSino;
                } else if (ch == '\n') {
                    return Token::KwSino;
                } else if (ch == ' ') {
                    return Token::KwSino;
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewS_q4:
                if (ch == 'C') {
                    text += ch;
                    state = StateId::KewS_q5;
                    ch = getNextChar();
                } else if (ch == 'c') {
                    text += ch;
                    state = StateId::KewS_q5;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewS_q5:
                if (ch == 'u') {
                    text += ch;
                    state = StateId::KewS_q6;
                    ch = getNextChar();
                } else if (ch == 'U') {
                    text += ch;
                    state = StateId::KewS_q6;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewS_q6:
                if (ch == 'e') {
                    text += ch;
                    state = StateId::KewS_q7;
                    ch = getNextChar();
                } else if (ch == 'E') {
                    text += ch;
                    state = StateId::KewS_q7;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewS_q7:
                if (ch == 'n') {
                    text += ch;
                    state = StateId::KewS_q8;
                    ch = getNextChar();
                } else if (ch == 'N') {
                    text += ch;
                    state = StateId::KewS_q8;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewS_q8:
                if (ch == 'C') {
                    text += ch;
                    state = StateId::KewS_q9;
                    ch = getNextChar();
                } else if (ch == 'c') {
                    text += ch;
                    state = StateId::KewS_q9;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewS_q9:
                if (ch == 'i') {
                    text += ch;
                    state = StateId::KewS_q10;
                    ch = getNextChar();
                } else if (ch == 'I') {
                    text += ch;
                    state = StateId::KewS_q10;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            // KewT
            case StateId::KewT_q0:
                if (ch == 'i') {
                    text += ch;
                    state = StateId::KewT_q1;
                    ch = getNextChar();
                } else if (ch == 'I') {
                    text += ch;
                    state = StateId::KewT_q1;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewT_q1:
                if (ch == 'p') {
                    text += ch;
                    state = StateId::KewT_q2;
                    ch = getNextChar();
                } else if (ch == 'P') {
                    text += ch;
                    state = StateId::KewT_q2;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewT_q2:
                if (ch == 'o') {
                    text += ch;
                    state = StateId::KewT_q3;
                    ch = getNextChar();
                } else if (ch == 'O') {
                    text += ch;
                    state = StateId::KewT_q3;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewT_q3:
                if (ch == ' ') {
                    return Token::KwTipo;
                } else if (ch == EOF) {
                    return Token::KwTipo;
                } else if (ch == '\t') {
                    return Token::KwTipo;
                } else if (ch == '\n') {
                    return Token::KwTipo;
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            // KewV
            case StateId::KewV_q0:
                if (ch == 'A') {
                    text += ch;
                    state = StateId::KewV_q1;
                    ch = getNextChar();
                } else if (ch == 'E') {
                    text += ch;
                    state = StateId::KewV_q4;
                    ch = getNextChar();
                } else if (ch == 'a') {
                    text += ch;
                    state = StateId::KewV_q1;
                    ch = getNextChar();
                } else if (ch == 'e') {
                    text += ch;
                    state = StateId::KewV_q4;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewV_q1:
                if (ch == 'R') {
                    text += ch;
                    state = StateId::KewV_q2;
                    ch = getNextChar();
                } else if (ch == 'r') {
                    text += ch;
                    state = StateId::KewV_q2;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewV_q10:
                if (ch == 'O') {
                    text += ch;
                    state = StateId::KewV_q11;
                    ch = getNextChar();
                } else if (ch == 'o') {
                    text += ch;
                    state = StateId::KewV_q11;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewV_q11:
                if (ch == '\n') {
                    return Token::KwVerdadero;
                } else if (ch == ' ') {
                    return Token::KwVerdadero;
                } else if (ch == EOF) {
                    return Token::KwVerdadero;
                } else if (ch == '\t') {
                    return Token::KwVerdadero;
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewV_q2:
                if (ch == '\t') {
                    return Token::KwVar;
                } else if (ch == EOF) {
                    return Token::KwVar;
                } else if (ch == ' ') {
                    return Token::KwVar;
                } else if (ch == '\n') {
                    return Token::KwVar;
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewV_q4:
                if (ch == 'R') {
                    text += ch;
                    state = StateId::KewV_q5;
                    ch = getNextChar();
                } else if (ch == 'r') {
                    text += ch;
                    state = StateId::KewV_q5;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewV_q5:
                if (ch == 'D') {
                    text += ch;
                    state = StateId::KewV_q6;
                    ch = getNextChar();
                } else if (ch == 'd') {
                    text += ch;
                    state = StateId::KewV_q6;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewV_q6:
                if (ch == 'a') {
                    text += ch;
                    state = StateId::KewV_q7;
                    ch = getNextChar();
                } else if (ch == 'A') {
                    text += ch;
                    state = StateId::KewV_q7;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewV_q7:
                if (ch == 'd') {
                    text += ch;
                    state = StateId::KewV_q8;
                    ch = getNextChar();
                } else if (ch == 'D') {
                    text += ch;
                    state = StateId::KewV_q8;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewV_q8:
                if (ch == 'e') {
                    text += ch;
                    state = StateId::KewV_q9;
                    ch = getNextChar();
                } else if (ch == 'E') {
                    text += ch;
                    state = StateId::KewV_q9;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewV_q9:
                if (ch == 'r') {
                    text += ch;
                    state = StateId::KewV_q10;
                    ch = getNextChar();
                } else if (ch == 'R') {
                    text += ch;
                    state = StateId::KewV_q10;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            // KewY
            case StateId::KewY_q0:
                if (ch == '\n') {
                    return Token::KwY;
                } else if (ch == '\t') {
                    return Token::KwY;
                } else if (ch == EOF) {
                    return Token::KwY;
                } else if (ch == ' ') {
                    return Token::KwY;
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            // Oper
            case StateId::Oper_q0:
                if (ch == '>') {
                    text += ch;
                    state = StateId::Oper_q15;
                    ch = getNextChar();
                } else if (ch == ':') {
                    text += ch;
                    return Token::Colon;
                } else if (ch == '(') {
                    text += ch;
                    return Token::OpenParens;
                } else if (ch == '+') {
                    text += ch;
                    return Token::Add;
                } else if (ch == '-') {
                    text += ch;
                    return Token::Sub;
                } else if (ch == '=') {
                    text += ch;
                    return Token::EqualTo;
                } else if (ch == '*') {
                    text += ch;
                    return Token::Mul;
                } else if (ch == ',') {
                    text += ch;
                    return Token::Comma;
                } else if (ch == '<') {
                    text += ch;
                    state = StateId::Oper_q9;
                    ch = getNextChar();
                } else if (ch == '^') {
                    text += ch;
                    return Token::Xor;
                } else if (ch == '[') {
                    text += ch;
                    return Token::OpenBra;
                } else if (ch == ']') {
                    text += ch;
                    return Token::CloseBra;
                } else if (ch == ')') {
                    text += ch;
                    return Token::CloseParens;
                } else {
                    reportError(ch);
                    ch = getNextChar();
                    state = StateId::Start_q0;
                }
                break;
            case StateId::Oper_q15:
                if (ch == '=') {
                    text += ch;
                    return Token::GreatEqual;
                } else {
                    ungetChar(ch);
                    return Token::GreatThan;
                }
                break;
            case StateId::Oper_q9:
                if (ch == '>') {
                    text += ch;
                    return Token::NotEqual;
                } else if (ch == '=') {
                    text += ch;
                    return Token::LessEqual;
                } else if (ch == '-') {
                    text += ch;
                    return Token::Assign;
                } else {
                    ungetChar(ch);
                    return Token::LessThan;
                }
                break;
            // String
            case StateId::String_q0:
                if (ch == '\n') {
                    state = StateId::String_q2;
                    ch = getNextChar();
                } else if (ch == '"') {
                    state = StateId::String_q3;
                    ch = getNextChar();
                } else {
                    text += ch;
                    state = StateId::String_q0;
                    ch = getNextChar();
                }
                break;
            case StateId::String_q2:
                if (ch == '"') {
                    state = StateId::Start_q0;
                    ch = getNextChar();
                } else {
                    state = StateId::String_q2;
                    ch = getNextChar();
                    throw "String literal must be inline";
                }
                break;
            case StateId::String_q3:
                if (ch == '"') {
                    text += ch;
                    state = StateId::String_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::StringConst;
                }
                break;
        }
    }
}
const char* exprLex::toString(Token tk) {
    switch (tk) {
        case Token::Add: return "Add";
        case Token::Assign: return "Assign";
        case Token::BinConst: return "BinConst";
        case Token::CharConst: return "CharConst";
        case Token::CloseBra: return "CloseBra";
        case Token::CloseParens: return "CloseParens";
        case Token::Colon: return "Colon";
        case Token::Comma: return "Comma";
        case Token::DecConst: return "DecConst";
        case Token::EndFile: return "EndFile";
        case Token::EqualTo: return "EqualTo";
        case Token::GreatEqual: return "GreatEqual";
        case Token::GreatThan: return "GreatThan";
        case Token::HexConst: return "HexConst";
        case Token::Iden: return "Iden";
        case Token::KwAbrir: return "KwAbrir";
        case Token::KwArchivo: return "KwArchivo";
        case Token::KwArreglo: return "KwArreglo";
        case Token::KwBooleano: return "KwBooleano";
        case Token::KwCadena: return "KwCadena";
        case Token::KwCaracter: return "KwCaracter";
        case Token::KwCaso: return "KwCaso";
        case Token::KwCerrar: return "KwCerrar";
        case Token::KwComo: return "KwComo";
        case Token::KwDe: return "KwDe";
        case Token::KwDiv: return "KwDiv";
        case Token::KwEntero: return "KwEntero";
        case Token::KwEntonces: return "KwEntonces";
        case Token::KwEs: return "KwEs";
        case Token::KwEscriba: return "KwEscriba";
        case Token::KwEscribir: return "KwEscribir";
        case Token::KwEscritura: return "KwEscritura";
        case Token::KwFalso: return "KwFalso";
        case Token::KwFin: return "KwFin";
        case Token::KwFinal: return "KwFinal";
        case Token::KwFuncion: return "KwFuncion";
        case Token::KwHaga: return "KwHaga";
        case Token::KwHasta: return "KwHasta";
        case Token::KwInicio: return "KwInicio";
        case Token::KwLea: return "KwLea";
        case Token::KwLectura: return "KwLectura";
        case Token::KwLeer: return "KwLeer";
        case Token::KwLlamar: return "KwLlamar";
        case Token::KwMientras: return "KwMientras";
        case Token::KwMod: return "KwMod";
        case Token::KwNo: return "KwNo";
        case Token::KwO: return "KwO";
        case Token::KwPara: return "KwPara";
        case Token::KwProcedimiento: return "KwProcedimiento";
        case Token::KwReal: return "KwReal";
        case Token::KwRegistro: return "KwRegistro";
        case Token::KwRepita: return "KwRepita";
        case Token::KwRetorne: return "KwRetorne";
        case Token::KwSecuencial: return "KwSecuencial";
        case Token::KwSi: return "KwSi";
        case Token::KwSino: return "KwSino";
        case Token::KwTipo: return "KwTipo";
        case Token::KwVar: return "KwVar";
        case Token::KwVerdadero: return "KwVerdadero";
        case Token::KwY: return "KwY";
        case Token::LessEqual: return "LessEqual";
        case Token::LessThan: return "LessThan";
        case Token::Mul: return "Mul";
        case Token::NotEqual: return "NotEqual";
        case Token::OpenBra: return "OpenBra";
        case Token::OpenParens: return "OpenParens";
        case Token::StringConst: return "StringConst";
        case Token::Sub: return "Sub";
        case Token::Xor: return "Xor";
        default: return "Unknown";
    }
}