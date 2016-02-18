#ifndef SYNTACTIC_H
#define SYNTACTIC_H

#include <iostream>
#include <fstream>
#include "lex.h"
#include "Interpreter.h"

class syntactic {
public:
    syntactic(char *);

     ~syntactic();

    void program();

private:
    lex * lexObject;
    Interpreter * interpreterObject;


    int errorCount;
    tokenType cToken;
    string cLex;
    ofstream db;
    ofstream lst;
    tokenType currentType;

    void nextLex();
    void errorMessage(string);

    void stmt();
    void moreStmts();
    void decl();
    void ntype();
    void declTail();
    void moreDecls();
    void term();
    void pre();
    void var();
    void post();
//    void uoppre();
    void stmtTail();
    void binop();

    void debug(string);



};


#endif