#include <iostream>
#include <fstream>
#include <cstdlib>

#include "lex.h"

using namespace std;

lex::lex (char * fileName, ostream * indbg, ostream * inlst){
    inFile = new ifstream();
    inFile->open(fileName, fstream::in);
    if(!inFile->is_open()){
        cout << "error: could not open file\n";
        exit(1);
    }
    cI = 0;
    lineNum = 0;
    errorCount = 0;
    lst = inlst;
    dbg = indbg;

}

lex::lex (){
    // should never be called
}

lex::~lex(){
}

void lex::errorMessage (string msg){
    *dbg<< "Error at " << lineNum << ", " << (cI + 1) << ": " <<  msg << endl;
    *lst<< "Error at " << lineNum << ", " << (cI + 1) << ": " <<  msg << endl;
    errorCount++;
}

string lex::getLexeme (){
    return cLexeme;
}

string lex::getTypeNames (int t){
    string typeNames [] = {"EOFT", "ERROR", "IDENT", "INTTYPE", "DBLTYPE", "NUMLIT",
            "NOT", "TILDA", "PLUSPLUS", "MINUSMINUS",
            "PLUS", "MINUS", "MULT", "DIV", "MOD", "EXP", "AND", "OR", "XOR",
            "ASSIGN", "LT", "GT", "SHIFTL",
            "SHIFTR", "PLUSEQ", "MINUSEQ", "MULTEQ",
            "DIVEQ", "MODEQ", "EXPEQ", "NOTEQ", "LOGAND", "ANDEQ", "LOGOR", "OREQ",
            "XOREQ", "EQUALTO", "SHIFTLEQ", "LTE", "SHIFTREQ", "GTE", "RPAREN",
            "LPAREN", "SEMI", "QUEST", "COLON", "COMMA"};

    return typeNames[t];
}

int lex::getErrorCount() {
    return errorCount;
}

tokenType lex::getToken () {
    if (cLine.empty()){
        getline(*inFile, cLine);
        cI = 0;
        lineNum++;
        *dbg << "Line " << lineNum << ": " << cLine << endl;
        *lst << "Line " << lineNum << ": " << cLine << endl;
    }

    while (isspace(cLine[cI])){
        cI++;
    }

    while(cI >= cLine.length()){
        if(getline(*inFile, cLine)){
            cI = 0;
            lineNum++;
            while (isspace(cLine[cI])) cI++;

            *dbg << "Line " << lineNum << ": " << cLine << endl;
            *lst << "Line " << lineNum << ": " << cLine << endl;
        } else {
            cLexeme = "EOF";
            return EOFT;
        }
    }

    if (isalpha(cLine[cI]) || cLine[cI] == '_'){
        int charCount = 1;
        string ident;
        ident += cLine[cI];
        cI++;
        while (cLine[cI] != ' ' && (isalpha(cLine[cI]) || isdigit(cLine[cI]) || cLine[cI] == '_')){
            ident += cLine[cI++];
            charCount++;
        }

        if (ident == "int" || ident =="INT") {
            cLexeme = "int";
            return INTTYPE;
        }

        if (ident == "double" || ident =="DOUBLE") {
            cLexeme = "double";
            return DBLTYPE;
        }

        if (charCount > 31){
            string msg = "IDENT ";
            msg += ident;
            msg += " too long";
            errorMessage(msg);
            cLexeme = ident;
            return ERROR;
        }

        cLexeme = ident;
        return IDENT;

    } else if (isdigit(cLine[cI])) {
        int charCount = 0;
        string num;
        num += cLine[cI++];
        while (isdigit(cLine[cI])) {
            num += cLine[cI++];
            charCount++;
        }
        if (cLine[cI] == '.'){
            num += cLine[cI++];
            charCount++;
            while(isdigit(cLine[cI])){
                num += cLine[cI++];
                charCount++;
            }
            cLexeme = num;
        } else {
            cLexeme = num;
        }

        if (charCount > 31){
            string msg = "NUM ";
            msg += num;
            msg += " is too long";
            errorMessage(msg);
            cLexeme = num;
            return ERROR;
        }
        return NUMLIT;
    } else {
        switch (cLine[cI]){
            case '.':
                cI++;
                if (isdigit(cLine[cI])){
                    string num = ".";
                    while(isdigit(cLine[cI])){
                        num += cLine[cI++];
                    }

                    if (num.length() > 32){
                        string msg = "NUM ";
                        msg += num;
                        msg += " too long";
                        errorMessage(msg);
                        cLexeme = msg;
                        return ERROR;
                    }

                    cLexeme = num;
                    return NUMLIT;
                } else {
                    cLexeme = ".";
                    errorMessage("Invalid character found: .");
                    return ERROR;
                }

            case '~':
                cLexeme = cLine[cI++];
                return TILDA;

            case '(':
                cLexeme = cLine[cI++];
                return LPAREN;

            case ')':
                cLexeme = cLine[cI++];
                return RPAREN;

            case ';':
                cLexeme = cLine[cI++];
                return SEMI;

            case '?':
                cLexeme = cLine[cI++];
                return QUEST;

            case ':':
                cLexeme = cLine[cI++];
                return COLON;

            case ',':
                cLexeme = cLine[cI++];
                return COMMA;

            case '!':
                cI++;
                if (cLine[cI] == '='){
                    cI++;
                    cLexeme = "!=";
                    return NOTEQ;
                }
                cLexeme = '!';
                return NOT;

            case '+':
                cI++;
                switch (cLine[cI]){
                    case '+':
                        cI++;
                        cLexeme = "++";
                        return PLUSPLUS;
                    case '=':
                        cI++;
                        cLexeme = "+=";
                        return PLUSEQ;
                    default:
                        cLexeme = '+';
                        return PLUS;
                }

            case '-':
                cI++;
                switch (cLine[cI]){
                    case '-':
                        cI++;
                        cLexeme = "--";
                        return MINUSMINUS;
                    case '=':
                        cI++;
                        cLexeme = "-=";
                        return MINUSEQ;
                    default:
                        cLexeme = '-';
                        return MINUS;
                }

            case '*':
                cI++;
                switch (cLine[cI]){
                    case '=':
                        cI++;
                        cLexeme = "*=";
                        return MULTEQ;
                    default:
                        cLexeme = '*';
                        return MULT;
                }

            case '/':
                cI++;
                switch (cLine[cI]){
                    case '=':
                        cI++;
                        cLexeme = "/=";
                        return DIVEQ;
                    default:
                        cLexeme = '/';
                        return DIV;
                }

            case '=':
                cI++;
                switch (cLine[cI]){
                    case '=':
                        cI++;
                        cLexeme = "==";
                        return EQUALTO;
                    default:
                        cLexeme = '=';
                        return ASSIGN;
                }

            case '%':
                cI++;
                switch (cLine[cI]){
                    case '=':
                        cI++;
                        cLexeme = "%=";
                        return MODEQ;
                    default:
                        cLexeme = '%';
                        return MOD;
                }

            case '^':
                cI++;
                switch (cLine[cI]){
                    case '=':
                        cI++;
                        cLexeme = "^=";
                        return XOREQ;
                    default:
                        cLexeme = '^';
                        return XOR;
                }

            case '|':
                cI++;
                switch (cLine[cI]){
                    case '=':
                        cI++;
                        cLexeme = "|=";
                        return OREQ;
                    case '|':
                        cI++;
                        cLexeme = "||";
                        return LOGOR;
                    default:
                        cLexeme = '|';
                        return OR;
                }

            case '&':
                cI++;
                switch (cLine[cI]){
                    case '=':
                        cI++;
                        cLexeme = "&=";
                        return ANDEQ;
                    case '&':
                        cI++;
                        cLexeme = "&&";
                        return LOGAND;
                    default:
                        cLexeme = '&';
                        return AND;
                }

            case '<':
                cI++;
                switch (cLine[cI]){
                    case '<':
                        cI++;
                        if ((cI != cLine.length()) && (cLine[cI] == '=')){
                            cI++;
                            cLexeme = "<<=";
                            return SHIFTLEQ;
                        }
                        cLexeme = "<<";
                        return SHIFTL;
                    case '=':
                        cI++;
                        cLexeme = "<=";
                        return LTE;
                    default:
                        cLexeme = '<';
                        return LT;
                }

            case '>':
                cI++;
                switch (cLine[cI]){
                    case '>':
                        cI++;
                        if ((cI != cLine.length()) && (cLine[cI] == '=')){
                            cI++;
                            cLexeme = ">>=";
                            return SHIFTREQ;
                        }
                        cLexeme = ">>";
                        return SHIFTR;
                    case '=':
                        cI++;
                        cLexeme = ">=";
                        return GTE;
                    default:
                        cLexeme = '>';
                        return GT;
                }
        }

        string msg = "Invalid character found: ";
        cLexeme = cLine[cI];
        msg += cLine[cI];
        errorMessage(msg);

        cI++;
        return ERROR;
    }
}

int lex::getLine() {
    return lineNum;
}

int lex::getColumn() {
    return cI + 1;
}
