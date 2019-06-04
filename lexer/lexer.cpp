Token exprLex::getNextToken() {
    StateId state = StateId::Start_q0;
    int ch = getNextChar();

    while (true) {
        switch (state) {
            // Start
            case StateId::Start_q0:
                if ((ch == ' ') || (ch == '\t')) {
                    state = StateId::Start_q0;
                    ch = getNextChar();
                } else {
                    state = StateId::Start_q1;
                    text="";
                }
                break;
            case StateId::Start_q1:
                if (ch == '"') {
                    state = StateId::String_q0;
                    ch = getNextChar();
                } else if (ch == '0') {
                    state = StateId::BinHex_q0;
                    ch = getNextChar();
                } else if ((ch >= '1') && (ch <= '9')) {
                    text += ch;
                    state = StateId::Dec_q0;
                    ch = getNextChar();
                } else if (ch == '\'') {
                    state = StateId::Char_q0;
                    ch = getNextChar();
                } else if (ch == EOF) {
                    return Token::EndFile;
                } else if (ch == '/') {
                    state = StateId::Comments_q0;
                    ch = getNextChar();
                } else if (ch == '\n') {
                    state = StateId::Start_q10;
                    ch = getNextChar();
                } else {
                    state = StateId::KewIden_q0;
                }
                break;
            case StateId::Start_q10:
                if ((ch == '\n') || (ch == ' ') || (ch == '\t')) {
                    state = StateId::Start_q10;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::EndLine;
                }
                break;
            // BinHex
            case StateId::BinHex_q0:
                if ((ch == 'b') || (ch == 'B')) {
                    text += ch;
                    state = StateId::BinHex_q2;
                    ch = getNextChar();
                } else if ((ch == 'x') || (ch == 'X')) {
                    text += ch;
                    state = StateId::BinHex_q1;
                    ch = getNextChar();
                } else if (ch == '0') {
                    state = StateId::Dec_q0;
                    ch = getNextChar();
                } else if ((ch >= '1') && (ch <= '9')) {
                    text += ch;
                    state = StateId::Dec_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::IntConst;
                }
                break;
            case StateId::BinHex_q1:
                if (isxdigit(ch)) {
                    text += ch;
                    state = StateId::BinHex_q1;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::IntConst;
                }
                break;
            case StateId::BinHex_q2:
                if ((ch >= '0') && (ch <= '1')) {
                    text += ch;
                    state = StateId::BinHex_q2;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::IntConst;
                }
                break;
            // Char
            case StateId::Char_q0:
                if (ch == '\'') {
                    state = StateId::Char_q5;
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
                if (ch == '*') {
                    state = StateId::Comments_q3;
                    ch = getNextChar();
                } else if (ch == '/') {
                    state = StateId::Comments_q1;
                    ch = getNextChar();
                } else {
                    // Trying next automaton 'Oper'
                    state = StateId::Oper_q0;
                }
                break;
            case StateId::Comments_q1:
                if ((ch == EOF) || (ch == '\n')) {
                    state = StateId::Start_q0;
                    ch = getNextChar();
                } else {
                    state = StateId::Comments_q1;
                    ch = getNextChar();
                }
                break;
            case StateId::Comments_q3:
                if (ch == EOF) {
                    state = StateId::Start_q0;
                    ch = getNextChar();
                } else if (ch == '*') {
                    state = StateId::Comments_q4;
                    ch = getNextChar();
                } else {
                    state = StateId::Comments_q3;
                    ch = getNextChar();
                }
                break;
            case StateId::Comments_q4:
                if ((ch == EOF) || (ch == '/')) {
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
                    return Token::IntConst;
                }
                break;
            // Iden
            case StateId::Iden_q0:
                if (((ch >= '0') && (ch <= '9')) || ((ch >= 'a') && (ch <= 'z')) || ((ch >= 'A') && (ch <= 'Z')) || (ch == '_')) {
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
                if ((ch == 'r') || (ch == 'R')) {
                    text += ch;
                    state = StateId::KewA_q1;
                    ch = getNextChar();
                } else if ((ch == 'B') || (ch == 'b')) {
                    text += ch;
                    state = StateId::KewA_q12;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewA_q1:
                if ((ch == 'c') || (ch == 'C')) {
                    text += ch;
                    state = StateId::KewA_q7;
                    ch = getNextChar();
                } else if ((ch == 'R') || (ch == 'r')) {
                    text += ch;
                    state = StateId::KewA_q2;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewA_q10:
                if ((ch == 'O') || (ch == 'o')) {
                    text += ch;
                    state = StateId::KewA_q19;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewA_q12:
                if ((ch == 'R') || (ch == 'r')) {
                    text += ch;
                    state = StateId::KewA_q13;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewA_q13:
                if ((ch == 'i') || (ch == 'I')) {
                    text += ch;
                    state = StateId::KewA_q14;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewA_q14:
                if ((ch == 'r') || (ch == 'R')) {
                    text += ch;
                    state = StateId::KewA_q17;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewA_q17:
                if (((ch >= 'A') && (ch <= 'Z')) || ((ch >= 'a') && (ch <= 'z')) || ((ch >= '0') && (ch <= '9')) || (ch == '_')) {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::KwAbrir;
                }
                break;
            case StateId::KewA_q18:
                if (((ch >= 'A') && (ch <= 'Z')) || (ch == '_') || ((ch >= 'a') && (ch <= 'z')) || ((ch >= '0') && (ch <= '9'))) {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::KwArreglo;
                }
                break;
            case StateId::KewA_q19:
                if (((ch >= '0') && (ch <= '9')) || ((ch >= 'a') && (ch <= 'z')) || (ch == '_') || ((ch >= 'A') && (ch <= 'Z'))) {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::KwArchivo;
                }
                break;
            case StateId::KewA_q2:
                if ((ch == 'e') || (ch == 'E')) {
                    text += ch;
                    state = StateId::KewA_q3;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewA_q3:
                if ((ch == 'G') || (ch == 'g')) {
                    text += ch;
                    state = StateId::KewA_q4;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewA_q4:
                if ((ch == 'l') || (ch == 'L')) {
                    text += ch;
                    state = StateId::KewA_q5;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewA_q5:
                if ((ch == 'o') || (ch == 'O')) {
                    text += ch;
                    state = StateId::KewA_q18;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewA_q7:
                if ((ch == 'h') || (ch == 'H')) {
                    text += ch;
                    state = StateId::KewA_q8;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewA_q8:
                if ((ch == 'I') || (ch == 'i')) {
                    text += ch;
                    state = StateId::KewA_q9;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewA_q9:
                if ((ch == 'V') || (ch == 'v')) {
                    text += ch;
                    state = StateId::KewA_q10;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            // KewB
            case StateId::KewB_q0:
                if ((ch == 'O') || (ch == 'o')) {
                    text += ch;
                    state = StateId::KewB_q1;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewB_q1:
                if ((ch == 'o') || (ch == 'O')) {
                    text += ch;
                    state = StateId::KewB_q2;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewB_q2:
                if ((ch == 'l') || (ch == 'L')) {
                    text += ch;
                    state = StateId::KewB_q3;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewB_q3:
                if ((ch == 'E') || (ch == 'e')) {
                    text += ch;
                    state = StateId::KewB_q4;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewB_q4:
                if ((ch == 'A') || (ch == 'a')) {
                    text += ch;
                    state = StateId::KewB_q5;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewB_q5:
                if ((ch == 'n') || (ch == 'N')) {
                    text += ch;
                    state = StateId::KewB_q6;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewB_q6:
                if ((ch == 'O') || (ch == 'o')) {
                    text += ch;
                    state = StateId::KewB_q7;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewB_q7:
                if (((ch >= 'A') && (ch <= 'Z')) || ((ch >= '0') && (ch <= '9')) || ((ch >= 'a') && (ch <= 'z')) || (ch == '_')) {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::KwBooleano;
                }
                break;
            // KewC
            case StateId::KewC_q0:
                if ((ch == 'e') || (ch == 'E')) {
                    text += ch;
                    state = StateId::KewC_q14;
                    ch = getNextChar();
                } else if ((ch == 'O') || (ch == 'o')) {
                    text += ch;
                    state = StateId::KewC_q20;
                    ch = getNextChar();
                } else if ((ch == 'A') || (ch == 'a')) {
                    text += ch;
                    state = StateId::KewC_q1;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewC_q1:
                if ((ch == 'd') || (ch == 'D')) {
                    text += ch;
                    state = StateId::KewC_q2;
                    ch = getNextChar();
                } else if ((ch == 's') || (ch == 'S')) {
                    text += ch;
                    state = StateId::KewC_q12;
                    ch = getNextChar();
                } else if ((ch == 'r') || (ch == 'R')) {
                    text += ch;
                    state = StateId::KewC_q7;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewC_q10:
                if ((ch == 'E') || (ch == 'e')) {
                    text += ch;
                    state = StateId::KewC_q6;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewC_q12:
                if ((ch == 'o') || (ch == 'O')) {
                    text += ch;
                    state = StateId::KewC_q23;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewC_q14:
                if ((ch == 'R') || (ch == 'r')) {
                    text += ch;
                    state = StateId::KewC_q16;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewC_q16:
                if ((ch == 'R') || (ch == 'r')) {
                    text += ch;
                    state = StateId::KewC_q17;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewC_q17:
                if ((ch == 'a') || (ch == 'A')) {
                    text += ch;
                    state = StateId::KewC_q18;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewC_q18:
                if ((ch == 'r') || (ch == 'R')) {
                    text += ch;
                    state = StateId::KewC_q26;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewC_q2:
                if ((ch == 'e') || (ch == 'E')) {
                    text += ch;
                    state = StateId::KewC_q3;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewC_q20:
                if ((ch == 'm') || (ch == 'M')) {
                    text += ch;
                    state = StateId::KewC_q21;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewC_q21:
                if ((ch == 'o') || (ch == 'O')) {
                    text += ch;
                    state = StateId::KewC_q27;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewC_q23:
                if (((ch >= 'a') && (ch <= 'z')) || ((ch >= '0') && (ch <= '9')) || (ch == '_') || ((ch >= 'A') && (ch <= 'Z'))) {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::KwCaso;
                }
                break;
            case StateId::KewC_q24:
                if (((ch >= 'A') && (ch <= 'Z')) || ((ch >= '0') && (ch <= '9')) || ((ch >= 'a') && (ch <= 'z')) || (ch == '_')) {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::KwCadena;
                }
                break;
            case StateId::KewC_q25:
                if ((ch == '_') || ((ch >= '0') && (ch <= '9')) || ((ch >= 'a') && (ch <= 'z')) || ((ch >= 'A') && (ch <= 'Z'))) {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::KwCaracter;
                }
                break;
            case StateId::KewC_q26:
                if ((ch == '_') || ((ch >= '0') && (ch <= '9')) || ((ch >= 'a') && (ch <= 'z')) || ((ch >= 'A') && (ch <= 'Z'))) {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::KwCerrar;
                }
                break;
            case StateId::KewC_q27:
                if (((ch >= 'A') && (ch <= 'Z')) || ((ch >= 'a') && (ch <= 'z')) || ((ch >= '0') && (ch <= '9')) || (ch == '_')) {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::KwComo;
                }
                break;
            case StateId::KewC_q3:
                if ((ch == 'n') || (ch == 'N')) {
                    text += ch;
                    state = StateId::KewC_q4;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewC_q4:
                if ((ch == 'a') || (ch == 'A')) {
                    text += ch;
                    state = StateId::KewC_q24;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewC_q6:
                if ((ch == 'R') || (ch == 'r')) {
                    text += ch;
                    state = StateId::KewC_q25;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewC_q7:
                if ((ch == 'A') || (ch == 'a')) {
                    text += ch;
                    state = StateId::KewC_q8;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewC_q8:
                if ((ch == 'c') || (ch == 'C')) {
                    text += ch;
                    state = StateId::KewC_q9;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewC_q9:
                if ((ch == 't') || (ch == 'T')) {
                    text += ch;
                    state = StateId::KewC_q10;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            // KewD
            case StateId::KewD_q0:
                if ((ch == 'i') || (ch == 'I')) {
                    text += ch;
                    state = StateId::KewD_q1;
                    ch = getNextChar();
                } else if ((ch == 'e') || (ch == 'E')) {
                    text += ch;
                    state = StateId::KewD_q4;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewD_q1:
                if ((ch == 'v') || (ch == 'V')) {
                    text += ch;
                    state = StateId::KewD_q3;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewD_q3:
                if ((ch == '_') || ((ch >= 'A') && (ch <= 'Z')) || ((ch >= 'a') && (ch <= 'z')) || ((ch >= '0') && (ch <= '9'))) {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::KwDiv;
                }
                break;
            case StateId::KewD_q4:
                if ((ch == '_') || ((ch >= 'A') && (ch <= 'Z')) || ((ch >= 'a') && (ch <= 'z')) || ((ch >= '0') && (ch <= '9'))) {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::KwDe;
                }
                break;
            // KewE
            case StateId::KewE_q0:
                if ((ch == 's') || (ch == 'S')) {
                    text += ch;
                    state = StateId::KewE_q12;
                    ch = getNextChar();
                } else if ((ch == 'N') || (ch == 'n')) {
                    text += ch;
                    state = StateId::KewE_q1;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewE_q1:
                if ((ch == 't') || (ch == 'T')) {
                    text += ch;
                    state = StateId::KewE_q2;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewE_q10:
                if ((ch == 's') || (ch == 'S')) {
                    text += ch;
                    state = StateId::KewE_q29;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewE_q11:
                if ((ch == 'B') || (ch == 'b')) {
                    text += ch;
                    state = StateId::KewE_q16;
                    ch = getNextChar();
                } else if ((ch == 't') || (ch == 'T')) {
                    text += ch;
                    state = StateId::KewE_q18;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewE_q12:
                if (((ch >= 'a') && (ch <= 'z')) || ((ch >= '0') && (ch <= '9')) || (ch == '_') || ((ch >= 'A') && (ch <= 'Z'))) {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else if ((ch == 'C') || (ch == 'c')) {
                    text += ch;
                    state = StateId::KewE_q13;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::KwEs;
                }
                break;
            case StateId::KewE_q13:
                if ((ch == 'r') || (ch == 'R')) {
                    text += ch;
                    state = StateId::KewE_q14;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewE_q14:
                if ((ch == 'i') || (ch == 'I')) {
                    text += ch;
                    state = StateId::KewE_q11;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewE_q16:
                if ((ch == 'i') || (ch == 'I')) {
                    text += ch;
                    state = StateId::KewE_q22;
                    ch = getNextChar();
                } else if ((ch == 'A') || (ch == 'a')) {
                    text += ch;
                    state = StateId::KewE_q27;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewE_q18:
                if ((ch == 'u') || (ch == 'U')) {
                    text += ch;
                    state = StateId::KewE_q19;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewE_q19:
                if ((ch == 'R') || (ch == 'r')) {
                    text += ch;
                    state = StateId::KewE_q20;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewE_q2:
                if ((ch == 'o') || (ch == 'O')) {
                    text += ch;
                    state = StateId::KewE_q7;
                    ch = getNextChar();
                } else if ((ch == 'E') || (ch == 'e')) {
                    text += ch;
                    state = StateId::KewE_q3;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewE_q20:
                if ((ch == 'a') || (ch == 'A')) {
                    text += ch;
                    state = StateId::KewE_q25;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewE_q22:
                if ((ch == 'r') || (ch == 'R')) {
                    text += ch;
                    state = StateId::KewE_q26;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewE_q25:
                if (((ch >= '0') && (ch <= '9')) || ((ch >= 'a') && (ch <= 'z')) || (ch == '_') || ((ch >= 'A') && (ch <= 'Z'))) {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::KwEscritura;
                }
                break;
            case StateId::KewE_q26:
                if ((ch == '_') || ((ch >= 'a') && (ch <= 'z')) || ((ch >= '0') && (ch <= '9')) || ((ch >= 'A') && (ch <= 'Z'))) {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::KwEscribir;
                }
                break;
            case StateId::KewE_q27:
                if (((ch >= 'A') && (ch <= 'Z')) || ((ch >= '0') && (ch <= '9')) || ((ch >= 'a') && (ch <= 'z')) || (ch == '_')) {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::KwEscriba;
                }
                break;
            case StateId::KewE_q28:
                if ((ch == '_') || ((ch >= 'a') && (ch <= 'z')) || ((ch >= '0') && (ch <= '9')) || ((ch >= 'A') && (ch <= 'Z'))) {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::KwEntero;
                }
                break;
            case StateId::KewE_q29:
                if (((ch >= 'A') && (ch <= 'Z')) || (ch == '_') || ((ch >= 'a') && (ch <= 'z')) || ((ch >= '0') && (ch <= '9'))) {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::KwEntonces;
                }
                break;
            case StateId::KewE_q3:
                if ((ch == 'r') || (ch == 'R')) {
                    text += ch;
                    state = StateId::KewE_q4;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewE_q4:
                if ((ch == 'O') || (ch == 'o')) {
                    text += ch;
                    state = StateId::KewE_q28;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewE_q7:
                if ((ch == 'n') || (ch == 'N')) {
                    text += ch;
                    state = StateId::KewE_q8;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewE_q8:
                if ((ch == 'c') || (ch == 'C')) {
                    text += ch;
                    state = StateId::KewE_q9;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewE_q9:
                if ((ch == 'E') || (ch == 'e')) {
                    text += ch;
                    state = StateId::KewE_q10;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            // KewF
            case StateId::KewF_q0:
                if ((ch == 'i') || (ch == 'I')) {
                    text += ch;
                    state = StateId::KewF_q6;
                    ch = getNextChar();
                } else if ((ch == 'a') || (ch == 'A')) {
                    text += ch;
                    state = StateId::KewF_q12;
                    ch = getNextChar();
                } else if ((ch == 'u') || (ch == 'U')) {
                    text += ch;
                    state = StateId::KewF_q7;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewF_q1:
                if ((ch == 'c') || (ch == 'C')) {
                    text += ch;
                    state = StateId::KewF_q2;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewF_q10:
                if ((ch == 'l') || (ch == 'L')) {
                    text += ch;
                    state = StateId::KewF_q17;
                    ch = getNextChar();
                } else {
                    // Trying next automaton 'Oper'
                    state = StateId::Oper_q0;
                }
                break;
            case StateId::KewF_q12:
                if ((ch == 'L') || (ch == 'l')) {
                    text += ch;
                    state = StateId::KewF_q13;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewF_q13:
                if ((ch == 'S') || (ch == 's')) {
                    text += ch;
                    state = StateId::KewF_q14;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewF_q14:
                if ((ch == 'o') || (ch == 'O')) {
                    text += ch;
                    state = StateId::KewF_q19;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewF_q17:
                if (((ch >= '0') && (ch <= '9')) || ((ch >= 'a') && (ch <= 'z')) || (ch == '_') || ((ch >= 'A') && (ch <= 'Z'))) {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::KwFinal;
                }
                break;
            case StateId::KewF_q18:
                if ((ch == '_') || ((ch >= '0') && (ch <= '9')) || ((ch >= 'a') && (ch <= 'z')) || ((ch >= 'A') && (ch <= 'Z'))) {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::KwFuncion;
                }
                break;
            case StateId::KewF_q19:
                if (((ch >= 'a') && (ch <= 'z')) || ((ch >= '0') && (ch <= '9')) || (ch == '_') || ((ch >= 'A') && (ch <= 'Z'))) {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::KwFalso;
                }
                break;
            case StateId::KewF_q2:
                if ((ch == 'I') || (ch == 'i')) {
                    text += ch;
                    state = StateId::KewF_q3;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewF_q3:
                if ((ch == 'o') || (ch == 'O')) {
                    text += ch;
                    state = StateId::KewF_q4;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewF_q4:
                if ((ch == 'N') || (ch == 'n')) {
                    text += ch;
                    state = StateId::KewF_q18;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewF_q6:
                if ((ch == 'N') || (ch == 'n')) {
                    text += ch;
                    state = StateId::KewF_q8;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewF_q7:
                if ((ch == 'n') || (ch == 'N')) {
                    text += ch;
                    state = StateId::KewF_q1;
                    ch = getNextChar();
                } else {
                    // Trying next automaton 'Oper'
                    state = StateId::Oper_q0;
                }
                break;
            case StateId::KewF_q8:
                if (((ch >= 'a') && (ch <= 'z')) || ((ch >= '0') && (ch <= '9')) || (ch == '_') || ((ch >= 'A') && (ch <= 'Z'))) {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else if ((ch == 'A') || (ch == 'a')) {
                    text += ch;
                    state = StateId::KewF_q10;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::KwFin;
                }
                break;
            // KewH
            case StateId::KewH_q0:
                if ((ch == 'A') || (ch == 'a')) {
                    text += ch;
                    state = StateId::KewH_q1;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewH_q1:
                if ((ch == 'S') || (ch == 's')) {
                    text += ch;
                    state = StateId::KewH_q4;
                    ch = getNextChar();
                } else if ((ch == 'g') || (ch == 'G')) {
                    text += ch;
                    state = StateId::KewH_q2;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewH_q2:
                if ((ch == 'A') || (ch == 'a')) {
                    text += ch;
                    state = StateId::KewH_q7;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewH_q4:
                if ((ch == 'T') || (ch == 't')) {
                    text += ch;
                    state = StateId::KewH_q5;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewH_q5:
                if ((ch == 'A') || (ch == 'a')) {
                    text += ch;
                    state = StateId::KewH_q8;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewH_q7:
                if (((ch >= 'A') && (ch <= 'Z')) || ((ch >= 'a') && (ch <= 'z')) || ((ch >= '0') && (ch <= '9')) || (ch == '_')) {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::KwHaga;
                }
                break;
            case StateId::KewH_q8:
                if ((ch == '_') || ((ch >= 'a') && (ch <= 'z')) || ((ch >= '0') && (ch <= '9')) || ((ch >= 'A') && (ch <= 'Z'))) {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::KwHasta;
                }
                break;
            // KewIden
            case StateId::KewIden_q0:
                if ((ch == '_') || (ch == 'z') || (ch == 'x') || (ch == 'q') || (ch == 'w') || (ch == 'u') || (ch == 'k') || (ch == 'j') || (ch == 'g')) {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else if ((ch == 'y') || (ch == 'Y')) {
                    text += ch;
                    state = StateId::KewY_q0;
                    ch = getNextChar();
                } else if ((ch == 'i') || (ch == 'I')) {
                    text += ch;
                    state = StateId::KewI_q0;
                    ch = getNextChar();
                } else if ((ch == 'N') || (ch == 'n')) {
                    text += ch;
                    state = StateId::KewN_q0;
                    ch = getNextChar();
                } else if ((ch == 'D') || (ch == 'd')) {
                    text += ch;
                    state = StateId::KewD_q0;
                    ch = getNextChar();
                } else if ((ch == 'm') || (ch == 'M')) {
                    text += ch;
                    state = StateId::KewM_q0;
                    ch = getNextChar();
                } else if ((ch == 'S') || (ch == 's')) {
                    text += ch;
                    state = StateId::KewS_q0;
                    ch = getNextChar();
                } else if ((ch == 't') || (ch == 'T')) {
                    text += ch;
                    state = StateId::KewT_q0;
                    ch = getNextChar();
                } else if ((ch == 'f') || (ch == 'F')) {
                    text += ch;
                    state = StateId::KewF_q0;
                    ch = getNextChar();
                } else if ((ch == 'P') || (ch == 'p')) {
                    text += ch;
                    state = StateId::KewP_q0;
                    ch = getNextChar();
                } else if ((ch == 'W') || (ch == 'U') || (ch == 'Q') || (ch == 'Z') || (ch == 'X') || (ch == 'G') || (ch == 'J') || (ch == 'K')) {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else if ((ch == 'b') || (ch == 'B')) {
                    text += ch;
                    state = StateId::KewB_q0;
                    ch = getNextChar();
                } else if ((ch == 'r') || (ch == 'R')) {
                    text += ch;
                    state = StateId::KewR_q0;
                    ch = getNextChar();
                } else if ((ch == 'L') || (ch == 'l')) {
                    text += ch;
                    state = StateId::KewL_q0;
                    ch = getNextChar();
                } else if ((ch == 'E') || (ch == 'e')) {
                    text += ch;
                    state = StateId::KewE_q0;
                    ch = getNextChar();
                } else if ((ch == 'v') || (ch == 'V')) {
                    text += ch;
                    state = StateId::KewV_q0;
                    ch = getNextChar();
                } else if ((ch == 'o') || (ch == 'O')) {
                    text += ch;
                    state = StateId::KewO_q0;
                    ch = getNextChar();
                } else if ((ch == 'h') || (ch == 'H')) {
                    text += ch;
                    state = StateId::KewH_q0;
                    ch = getNextChar();
                } else if ((ch == 'a') || (ch == 'A')) {
                    text += ch;
                    state = StateId::KewA_q0;
                    ch = getNextChar();
                } else if ((ch == 'c') || (ch == 'C')) {
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
                if ((ch == 'n') || (ch == 'N')) {
                    text += ch;
                    state = StateId::KewI_q1;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewI_q1:
                if ((ch == 'i') || (ch == 'I')) {
                    text += ch;
                    state = StateId::KewI_q2;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewI_q2:
                if ((ch == 'c') || (ch == 'C')) {
                    text += ch;
                    state = StateId::KewI_q3;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewI_q3:
                if ((ch == 'i') || (ch == 'I')) {
                    text += ch;
                    state = StateId::KewI_q4;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewI_q4:
                if ((ch == 'O') || (ch == 'o')) {
                    text += ch;
                    state = StateId::KewI_q6;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewI_q6:
                if ((ch == '_') || ((ch >= 'A') && (ch <= 'Z')) || ((ch >= 'a') && (ch <= 'z')) || ((ch >= '0') && (ch <= '9'))) {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::KwInicio;
                }
                break;
            // KewL
            case StateId::KewL_q0:
                if ((ch == 'L') || (ch == 'l')) {
                    text += ch;
                    state = StateId::KewL_q1;
                    ch = getNextChar();
                } else if ((ch == 'E') || (ch == 'e')) {
                    text += ch;
                    state = StateId::KewL_q6;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewL_q1:
                if ((ch == 'A') || (ch == 'a')) {
                    text += ch;
                    state = StateId::KewL_q2;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewL_q10:
                if ((ch == 'T') || (ch == 't')) {
                    text += ch;
                    state = StateId::KewL_q11;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewL_q11:
                if ((ch == 'U') || (ch == 'u')) {
                    text += ch;
                    state = StateId::KewL_q12;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewL_q12:
                if ((ch == 'r') || (ch == 'R')) {
                    text += ch;
                    state = StateId::KewL_q13;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewL_q13:
                if ((ch == 'A') || (ch == 'a')) {
                    text += ch;
                    state = StateId::KewL_q19;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewL_q16:
                if (((ch >= 'A') && (ch <= 'Z')) || (ch == '_') || ((ch >= 'a') && (ch <= 'z')) || ((ch >= '0') && (ch <= '9'))) {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::KwLlamar;
                }
                break;
            case StateId::KewL_q17:
                if (((ch >= 'a') && (ch <= 'z')) || ((ch >= '0') && (ch <= '9')) || (ch == '_') || ((ch >= 'A') && (ch <= 'Z'))) {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::KwLea;
                }
                break;
            case StateId::KewL_q18:
                if (((ch >= 'A') && (ch <= 'Z')) || (ch == '_') || ((ch >= 'a') && (ch <= 'z')) || ((ch >= '0') && (ch <= '9'))) {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::KwLeer;
                }
                break;
            case StateId::KewL_q19:
                if ((ch == '_') || ((ch >= '0') && (ch <= '9')) || ((ch >= 'a') && (ch <= 'z')) || ((ch >= 'A') && (ch <= 'Z'))) {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::KwLectura;
                }
                break;
            case StateId::KewL_q2:
                if ((ch == 'm') || (ch == 'M')) {
                    text += ch;
                    state = StateId::KewL_q3;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewL_q3:
                if ((ch == 'A') || (ch == 'a')) {
                    text += ch;
                    state = StateId::KewL_q4;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewL_q4:
                if ((ch == 'R') || (ch == 'r')) {
                    text += ch;
                    state = StateId::KewL_q16;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewL_q6:
                if ((ch == 'a') || (ch == 'A')) {
                    text += ch;
                    state = StateId::KewL_q17;
                    ch = getNextChar();
                } else if ((ch == 'C') || (ch == 'c')) {
                    text += ch;
                    state = StateId::KewL_q10;
                    ch = getNextChar();
                } else if ((ch == 'E') || (ch == 'e')) {
                    text += ch;
                    state = StateId::KewL_q8;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewL_q8:
                if ((ch == 'R') || (ch == 'r')) {
                    text += ch;
                    state = StateId::KewL_q18;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            // KewM
            case StateId::KewM_q0:
                if ((ch == 'o') || (ch == 'O')) {
                    text += ch;
                    state = StateId::KewM_q9;
                    ch = getNextChar();
                } else if ((ch == 'I') || (ch == 'i')) {
                    text += ch;
                    state = StateId::KewM_q1;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewM_q1:
                if ((ch == 'e') || (ch == 'E')) {
                    text += ch;
                    state = StateId::KewM_q2;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewM_q11:
                if (((ch >= 'A') && (ch <= 'Z')) || ((ch >= '0') && (ch <= '9')) || ((ch >= 'a') && (ch <= 'z')) || (ch == '_')) {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::KwMod;
                }
                break;
            case StateId::KewM_q2:
                if ((ch == 'N') || (ch == 'n')) {
                    text += ch;
                    state = StateId::KewM_q3;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewM_q3:
                if ((ch == 'T') || (ch == 't')) {
                    text += ch;
                    state = StateId::KewM_q4;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewM_q4:
                if ((ch == 'r') || (ch == 'R')) {
                    text += ch;
                    state = StateId::KewM_q5;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewM_q5:
                if ((ch == 'A') || (ch == 'a')) {
                    text += ch;
                    state = StateId::KewM_q7;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewM_q6:
                if (((ch >= 'A') && (ch <= 'Z')) || ((ch >= '0') && (ch <= '9')) || ((ch >= 'a') && (ch <= 'z')) || (ch == '_')) {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::KwMientras;
                }
                break;
            case StateId::KewM_q7:
                if ((ch == 's') || (ch == 'S')) {
                    text += ch;
                    state = StateId::KewM_q6;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewM_q9:
                if ((ch == 'D') || (ch == 'd')) {
                    text += ch;
                    state = StateId::KewM_q11;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            // KewN
            case StateId::KewN_q0:
                if ((ch == 'o') || (ch == 'O')) {
                    text += ch;
                    state = StateId::KewN_q2;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewN_q2:
                if (((ch >= 'A') && (ch <= 'Z')) || (ch == '_') || ((ch >= '0') && (ch <= '9')) || ((ch >= 'a') && (ch <= 'z'))) {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::KwNo;
                }
                break;
            // KewO
            case StateId::KewO_q0:
                if ((ch == ' ') || (ch == EOF) || (ch == '\t')) {
                    return Token::KwO;
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            // KewP
            case StateId::KewP_q0:
                if ((ch == 'r') || (ch == 'R')) {
                    text += ch;
                    state = StateId::KewP_q4;
                    ch = getNextChar();
                } else if ((ch == 'A') || (ch == 'a')) {
                    text += ch;
                    state = StateId::KewP_q1;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewP_q1:
                if ((ch == 'R') || (ch == 'r')) {
                    text += ch;
                    state = StateId::KewP_q2;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewP_q10:
                if ((ch == 'i') || (ch == 'I')) {
                    text += ch;
                    state = StateId::KewP_q11;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewP_q11:
                if ((ch == 'E') || (ch == 'e')) {
                    text += ch;
                    state = StateId::KewP_q12;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewP_q12:
                if ((ch == 'N') || (ch == 'n')) {
                    text += ch;
                    state = StateId::KewP_q13;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewP_q13:
                if ((ch == 't') || (ch == 'T')) {
                    text += ch;
                    state = StateId::KewP_q14;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewP_q14:
                if ((ch == 'o') || (ch == 'O')) {
                    text += ch;
                    state = StateId::KewP_q18;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewP_q17:
                if (((ch >= 'A') && (ch <= 'Z')) || ((ch >= '0') && (ch <= '9')) || ((ch >= 'a') && (ch <= 'z')) || (ch == '_')) {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::KwPara;
                }
                break;
            case StateId::KewP_q18:
                if (((ch >= 'A') && (ch <= 'Z')) || (ch == '_') || ((ch >= 'a') && (ch <= 'z')) || ((ch >= '0') && (ch <= '9'))) {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::KwProcedimiento;
                }
                break;
            case StateId::KewP_q2:
                if ((ch == 'A') || (ch == 'a')) {
                    text += ch;
                    state = StateId::KewP_q17;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewP_q4:
                if ((ch == 'o') || (ch == 'O')) {
                    text += ch;
                    state = StateId::KewP_q5;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewP_q5:
                if ((ch == 'c') || (ch == 'C')) {
                    text += ch;
                    state = StateId::KewP_q6;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewP_q6:
                if ((ch == 'E') || (ch == 'e')) {
                    text += ch;
                    state = StateId::KewP_q7;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewP_q7:
                if ((ch == 'd') || (ch == 'D')) {
                    text += ch;
                    state = StateId::KewP_q8;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewP_q8:
                if ((ch == 'i') || (ch == 'I')) {
                    text += ch;
                    state = StateId::KewP_q9;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewP_q9:
                if ((ch == 'M') || (ch == 'm')) {
                    text += ch;
                    state = StateId::KewP_q10;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            // KewR
            case StateId::KewR_q0:
                if ((ch == 'e') || (ch == 'E')) {
                    text += ch;
                    state = StateId::KewR_q1;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewR_q1:
                if ((ch == 'p') || (ch == 'P')) {
                    text += ch;
                    state = StateId::KewR_q9;
                    ch = getNextChar();
                } else if ((ch == 'a') || (ch == 'A')) {
                    text += ch;
                    state = StateId::KewR_q2;
                    ch = getNextChar();
                } else if ((ch == 'G') || (ch == 'g')) {
                    text += ch;
                    state = StateId::KewR_q13;
                    ch = getNextChar();
                } else if ((ch == 't') || (ch == 'T')) {
                    text += ch;
                    state = StateId::KewR_q6;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewR_q10:
                if ((ch == 'n') || (ch == 'N')) {
                    text += ch;
                    state = StateId::KewR_q11;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewR_q11:
                if ((ch == 'E') || (ch == 'e')) {
                    text += ch;
                    state = StateId::KewR_q20;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewR_q13:
                if ((ch == 'I') || (ch == 'i')) {
                    text += ch;
                    state = StateId::KewR_q14;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewR_q14:
                if ((ch == 's') || (ch == 'S')) {
                    text += ch;
                    state = StateId::KewR_q16;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewR_q16:
                if ((ch == 'T') || (ch == 't')) {
                    text += ch;
                    state = StateId::KewR_q17;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewR_q17:
                if ((ch == 'R') || (ch == 'r')) {
                    text += ch;
                    state = StateId::KewR_q18;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewR_q18:
                if ((ch == 'o') || (ch == 'O')) {
                    text += ch;
                    state = StateId::KewR_q21;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewR_q2:
                if ((ch == 'l') || (ch == 'L')) {
                    text += ch;
                    state = StateId::KewR_q23;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewR_q20:
                if (((ch >= 'A') && (ch <= 'Z')) || (ch == '_') || ((ch >= 'a') && (ch <= 'z')) || ((ch >= '0') && (ch <= '9'))) {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::KwRetorne;
                }
                break;
            case StateId::KewR_q21:
                if (((ch >= '0') && (ch <= '9')) || ((ch >= 'a') && (ch <= 'z')) || (ch == '_') || ((ch >= 'A') && (ch <= 'Z'))) {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::KwRegistro;
                }
                break;
            case StateId::KewR_q22:
                if (((ch >= 'a') && (ch <= 'z')) || ((ch >= '0') && (ch <= '9')) || (ch == '_') || ((ch >= 'A') && (ch <= 'Z'))) {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::KwRepita;
                }
                break;
            case StateId::KewR_q23:
                if ((ch == '_') || ((ch >= '0') && (ch <= '9')) || ((ch >= 'a') && (ch <= 'z')) || ((ch >= 'A') && (ch <= 'Z'))) {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::KwReal;
                }
                break;
            case StateId::KewR_q4:
                if ((ch == 'T') || (ch == 't')) {
                    text += ch;
                    state = StateId::KewR_q5;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewR_q5:
                if ((ch == 'a') || (ch == 'A')) {
                    text += ch;
                    state = StateId::KewR_q22;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewR_q6:
                if ((ch == 'o') || (ch == 'O')) {
                    text += ch;
                    state = StateId::KewR_q8;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewR_q8:
                if ((ch == 'r') || (ch == 'R')) {
                    text += ch;
                    state = StateId::KewR_q10;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewR_q9:
                if ((ch == 'i') || (ch == 'I')) {
                    text += ch;
                    state = StateId::KewR_q4;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            // KewS
            case StateId::KewS_q0:
                if ((ch == 'i') || (ch == 'I')) {
                    text += ch;
                    state = StateId::KewS_q1;
                    ch = getNextChar();
                } else if ((ch == 'e') || (ch == 'E')) {
                    text += ch;
                    state = StateId::KewS_q4;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewS_q1:
                if ((ch == '_') || ((ch >= 'a') && (ch <= 'z')) || ((ch >= '0') && (ch <= '9')) || ((ch >= 'A') && (ch <= 'Z'))) {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else if ((ch == 'n') || (ch == 'N')) {
                    text += ch;
                    state = StateId::KewS_q2;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::KwSi;
                }
                break;
            case StateId::KewS_q10:
                if ((ch == 'a') || (ch == 'A')) {
                    text += ch;
                    state = StateId::KewS_q11;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewS_q11:
                if ((ch == 'l') || (ch == 'L')) {
                    text += ch;
                    state = StateId::KewS_q12;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewS_q12:
                if ((ch == '_') || ((ch >= 'a') && (ch <= 'z')) || ((ch >= '0') && (ch <= '9')) || ((ch >= 'A') && (ch <= 'Z'))) {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::KwSecuencial;
                }
                break;
            case StateId::KewS_q2:
                if ((ch == 'o') || (ch == 'O')) {
                    text += ch;
                    state = StateId::KewS_q3;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewS_q3:
                if (((ch >= '0') && (ch <= '9')) || ((ch >= 'a') && (ch <= 'z')) || (ch == '_') || ((ch >= 'A') && (ch <= 'Z'))) {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::KwSino;
                }
                break;
            case StateId::KewS_q4:
                if ((ch == 'C') || (ch == 'c')) {
                    text += ch;
                    state = StateId::KewS_q5;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewS_q5:
                if ((ch == 'U') || (ch == 'u')) {
                    text += ch;
                    state = StateId::KewS_q6;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewS_q6:
                if ((ch == 'e') || (ch == 'E')) {
                    text += ch;
                    state = StateId::KewS_q7;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewS_q7:
                if ((ch == 'N') || (ch == 'n')) {
                    text += ch;
                    state = StateId::KewS_q8;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewS_q8:
                if ((ch == 'c') || (ch == 'C')) {
                    text += ch;
                    state = StateId::KewS_q9;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewS_q9:
                if ((ch == 'i') || (ch == 'I')) {
                    text += ch;
                    state = StateId::KewS_q10;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            // KewT
            case StateId::KewT_q0:
                if ((ch == 'i') || (ch == 'I')) {
                    text += ch;
                    state = StateId::KewT_q1;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewT_q1:
                if ((ch == 'p') || (ch == 'P')) {
                    text += ch;
                    state = StateId::KewT_q2;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewT_q2:
                if ((ch == 'o') || (ch == 'O')) {
                    text += ch;
                    state = StateId::KewT_q3;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewT_q3:
                if (((ch >= 'a') && (ch <= 'z')) || ((ch >= '0') && (ch <= '9')) || ((ch >= 'A') && (ch <= 'Z')) || (ch == '_')) {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::KwTipo;
                }
                break;
            // KewV
            case StateId::KewV_q0:
                if ((ch == 'E') || (ch == 'e')) {
                    text += ch;
                    state = StateId::KewV_q4;
                    ch = getNextChar();
                } else if ((ch == 'a') || (ch == 'A')) {
                    text += ch;
                    state = StateId::KewV_q1;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewV_q1:
                if ((ch == 'R') || (ch == 'r')) {
                    text += ch;
                    state = StateId::KewV_q2;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewV_q10:
                if ((ch == 'O') || (ch == 'o')) {
                    text += ch;
                    state = StateId::KewV_q11;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewV_q11:
                if (((ch >= '0') && (ch <= '9')) || ((ch >= 'a') && (ch <= 'z')) || ((ch >= 'A') && (ch <= 'Z')) || (ch == '_')) {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::KwVerdadero;
                }
                break;
            case StateId::KewV_q2:
                if ((ch == '_') || ((ch >= 'a') && (ch <= 'z')) || ((ch >= '0') && (ch <= '9')) || ((ch >= 'A') && (ch <= 'Z'))) {
                    text += ch;
                    state = StateId::Iden_q0;
                    ch = getNextChar();
                } else {
                    ungetChar(ch);
                    return Token::KwVar;
                }
                break;
            case StateId::KewV_q4:
                if ((ch == 'r') || (ch == 'R')) {
                    text += ch;
                    state = StateId::KewV_q5;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewV_q5:
                if ((ch == 'd') || (ch == 'D')) {
                    text += ch;
                    state = StateId::KewV_q6;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewV_q6:
                if ((ch == 'a') || (ch == 'A')) {
                    text += ch;
                    state = StateId::KewV_q7;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewV_q7:
                if ((ch == 'D') || (ch == 'd')) {
                    text += ch;
                    state = StateId::KewV_q8;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewV_q8:
                if ((ch == 'e') || (ch == 'E')) {
                    text += ch;
                    state = StateId::KewV_q9;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            case StateId::KewV_q9:
                if ((ch == 'r') || (ch == 'R')) {
                    text += ch;
                    state = StateId::KewV_q10;
                    ch = getNextChar();
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            // KewY
            case StateId::KewY_q0:
                if ((ch == EOF) || (ch == ' ') || (ch == '\t')) {
                    return Token::KwY;
                } else {
                    state = StateId::Iden_q0;
                }
                break;
            // Oper
            case StateId::Oper_q0:
                if (ch == '-') {
                    text += ch;
                    return Token::Sub;
                } else if (ch == '>') {
                    text += ch;
                    state = StateId::Oper_q15;
                    ch = getNextChar();
                } else if (ch == ':') {
                    text += ch;
                    return Token::Colon;
                } else if (ch == '=') {
                    text += ch;
                    return Token::EqualTo;
                } else if (ch == '*') {
                    text += ch;
                    return Token::Mul;
                } else if (ch == ',') {
                    text += ch;
                    return Token::Comma;
                } else if (ch == '(') {
                    text += ch;
                    return Token::OpenParens;
                } else if (ch == '[') {
                    text += ch;
                    return Token::OpenBra;
                } else if (ch == ')') {
                    text += ch;
                    return Token::CloseParens;
                } else if (ch == ']') {
                    text += ch;
                    return Token::CloseBra;
                } else if (ch == '+') {
                    text += ch;
                    return Token::Add;
                } else if (ch == '<') {
                    text += ch;
                    state = StateId::Oper_q9;
                    ch = getNextChar();
                } else if (ch == '^') {
                    text += ch;
                    return Token::Xor;
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
                if (ch == '-') {
                    text += ch;
                    return Token::Assign;
                } else if (ch == '>') {
                    text += ch;
                    return Token::NotEqual;
                } else if (ch == '=') {
                    text += ch;
                    return Token::LessEqual;
                } else {
                    ungetChar(ch);
                    return Token::LessThan;
                }
                break;
            // String
            case StateId::String_q0:
                if (ch == '"') {
                    state = StateId::String_q3;
                    ch = getNextChar();
                } else if (ch == '\n') {
                    state = StateId::String_q2;
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
        case Token::LessEqual: return "LessEqual";
        case Token::Assign: return "Assign";
        case Token::GreatThan: return "GreatThan";
        case Token::Xor: return "Xor";
        case Token::OpenBra: return "OpenBra";
        case Token::OpenParens: return "OpenParens";
        case Token::Mul: return "Mul";
        case Token::Colon: return "Colon";
        case Token::EqualTo: return "EqualTo";
        case Token::Sub: return "Sub";
        case Token::KwY: return "KwY";
        case Token::KwVar: return "KwVar";
        case Token::KwVerdadero: return "KwVerdadero";
        case Token::KwTipo: return "KwTipo";
        case Token::CloseBra: return "CloseBra";
        case Token::KwSino: return "KwSino";
        case Token::KwSecuencial: return "KwSecuencial";
        case Token::LessThan: return "LessThan";
        case Token::KwReal: return "KwReal";
        case Token::KwRepita: return "KwRepita";
        case Token::Add: return "Add";
        case Token::KwRegistro: return "KwRegistro";
        case Token::Comma: return "Comma";
        case Token::KwRetorne: return "KwRetorne";
        case Token::KwProcedimiento: return "KwProcedimiento";
        case Token::KwO: return "KwO";
        case Token::KwComo: return "KwComo";
        case Token::StringConst: return "StringConst";
        case Token::NotEqual: return "NotEqual";
        case Token::KwDe: return "KwDe";
        case Token::KwHaga: return "KwHaga";
        case Token::GreatEqual: return "GreatEqual";
        case Token::KwCerrar: return "KwCerrar";
        case Token::KwCadena: return "KwCadena";
        case Token::KwDiv: return "KwDiv";
        case Token::Iden: return "Iden";
        case Token::KwArreglo: return "KwArreglo";
        case Token::KwEscriba: return "KwEscriba";
        case Token::CloseParens: return "CloseParens";
        case Token::KwFinal: return "KwFinal";
        case Token::IntConst: return "IntConst";
        case Token::KwEscribir: return "KwEscribir";
        case Token::CharConst: return "CharConst";
        case Token::KwArchivo: return "KwArchivo";
        case Token::KwEntero: return "KwEntero";
        case Token::KwBooleano: return "KwBooleano";
        case Token::KwEs: return "KwEs";
        case Token::KwLlamar: return "KwLlamar";
        case Token::KwInicio: return "KwInicio";
        case Token::KwSi: return "KwSi";
        case Token::EndFile: return "EndFile";
        case Token::KwEscritura: return "KwEscritura";
        case Token::KwCaracter: return "KwCaracter";
        case Token::KwAbrir: return "KwAbrir";
        case Token::KwEntonces: return "KwEntonces";
        case Token::KwFuncion: return "KwFuncion";
        case Token::KwLectura: return "KwLectura";
        case Token::KwFalso: return "KwFalso";
        case Token::KwFin: return "KwFin";
        case Token::KwLea: return "KwLea";
        case Token::KwHasta: return "KwHasta";
        case Token::KwLeer: return "KwLeer";
        case Token::KwMod: return "KwMod";
        case Token::KwMientras: return "KwMientras";
        case Token::KwPara: return "KwPara";
        case Token::KwCaso: return "KwCaso";
        case Token::EndLine: return "EndLine";
        case Token::KwNo: return "KwNo";
        default: return "Unknown";
    }
}