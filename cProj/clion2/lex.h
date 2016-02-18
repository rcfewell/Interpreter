#ifndef LEX_H
#define LEX_H

#include <iostream>
#include <fstream>

using namespace std;

enum tokenType {
    EOFT, ERROR, IDENT, INTTYPE, DBLTYPE, NUMLIT,

    //preops that aren't binops
    NOT, TILDA,
    PLUSPLUS, MINUSMINUS,

    //binops
    PLUS, MINUS, MULT, DIV, MOD, EXP, AND, OR, XOR,
    ASSIGN, LT, GT, SHIFTL,
    SHIFTR, PLUSEQ, MINUSEQ, MULTEQ,
    DIVEQ, MODEQ, EXPEQ, NOTEQ, LOGAND, ANDEQ, LOGOR, OREQ,
    XOREQ, EQUALTO, SHIFTLEQ, LTE, SHIFTREQ, GTE,

    //not binops
    RPAREN, LPAREN, SEMI, QUEST, COLON, COMMA};

class lex
{
    public:
        lex(char * filename, ostream*, ostream*);
        lex();
        ~lex();
        tokenType getToken ();
        string getLexeme ();
        int getLine();
        int getColumn();
        int getErrorCount();
        void errorMessage (string msg);
        string getTypeNames (int t);

    private:
        void error_message (string msg);

        string cLine;
        string cToken;
        string cLexeme;
        int cI;

        ifstream * inFile;

        ostream * lst;
        ostream * dbg;
        int lineNum;
        int colNum;
        int errorCount;
};
#endif
