#include <cstring>
#include "syntactic.h"
#include "OpCodes.h"


using namespace std;

syntactic::syntactic (char * filename){
    string filePrefix;
    for (int i = 0; i < strlen(filename); i++){
        if (filename[i] != '.'){
            filePrefix += filename[i];
        } else {
            i = strlen(filename);
        }
    }

    string dbFileName = filePrefix;
    dbFileName += ".dbg";
    db.open(dbFileName.c_str());

    string lstFileName = filePrefix;
    lstFileName += ".lst";
    lst.open(lstFileName.c_str());
    lexObject = new lex(filename, &db, &lst);

    string symFileName = filePrefix;
    symFileName += ".sym";
    interpreterObject = new Interpreter(symFileName, lexObject);

    nextLex();

    currentType = ERROR;

    errorCount = 0;
}


syntactic::~syntactic() {

    db << endl << errorCount << " syntactic errors found in input file" << endl;
    db << lexObject->getErrorCount() << " lexical errors found in input file" << endl;

    lst << (lexObject->getErrorCount() + errorCount) << " total errors" << endl;

    delete lexObject;
    delete interpreterObject;
}

void syntactic::debug(string string1) {
    db << string1 << " cToken: " << lexObject->getTypeNames(cToken) << " cLex: " << cLex << endl;
}

void syntactic::errorMessage(string msg) {
    lst << "Error at " << lexObject->getLine() << ", " << lexObject->getColumn() << ": " <<  msg << endl;
    db << "ERROR at " << lexObject->getLine() << ", " << lexObject->getColumn() << ": " <<  msg << endl;
    errorCount++;
    nextLex();
}

void syntactic::nextLex() {
    cToken = lexObject->getToken();
    cLex = lexObject->getLexeme();
    db << '\t' << lexObject->getTypeNames(cToken) << ' ' << cLex << endl;
}


void syntactic::program (){
    debug("entered program");

    if (cToken == INTTYPE || cToken == DBLTYPE){
        decl();
    } else {
        stmt();
    }

    if (cToken == SEMI){
        nextLex();
    } else {
        errorMessage("expected semi");
        while (cToken != SEMI && cToken != EOFT){
            nextLex();
        }
        if (cToken == SEMI){
            nextLex();
        }
    }

    moreStmts();

    if (cToken == EOFT){
        //nothing
    } else {
        errorMessage("Expected end of file");
    }

    debug("exited program");
}



void syntactic::stmt() {
    debug("entered stmt");

    term();
    stmtTail();

    debug("exited stmt");
}

void syntactic::moreStmts() {
    debug("entered moreStmts");

    if (cToken == EOFT){
        debug("exited moreStmts");
        return;
    } else if (cToken == INTTYPE || cToken == DBLTYPE) {
        decl();
    } else {
        stmt();
    }

    if (cToken == SEMI){
        nextLex();
    } else {
        errorMessage("expected semicolon");
        while(cToken != SEMI && cToken != EOFT){
            nextLex();
        }
        if (cToken == SEMI){
            nextLex();
        }
    }

    moreStmts();
    debug("exited moreStmts");
}

void syntactic::decl() {
    debug("entered decl");

    ntype();

    if(cToken == IDENT){
        interpreterObject->NewDeclaration(cLex);
        nextLex();
    } else {
        errorMessage("expected IDENT");
    }

    declTail();
    moreDecls();

    currentType = ERROR;

    debug("exited decl");
}

void syntactic::ntype() {
    debug("entered ntype");

    if (cToken == INTTYPE){
        interpreterObject->SaveDeclType(INTTYPE);
        nextLex();
    } else if (cToken == DBLTYPE) {
        interpreterObject->SaveDeclType(DBLTYPE);
        nextLex();
    } else {
        errorMessage("expected INTTYPE or DBLTYPE");
    }

    debug("exited ntype");
}

void syntactic::declTail() {
    debug("entered declTail");

    if (cToken == ASSIGN){
        interpreterObject->binopFound(ASSIGN);
        nextLex();
        stmt();
    }

    debug("exited declTail");
}

void syntactic::moreDecls() {
    debug("entered moreDecls");

    if (cToken == SEMI){
        return;
    } else {
        if(cToken == COMMA){
            interpreterObject->otheropFound(COMMA);
            nextLex();
            if (cToken == IDENT){
                interpreterObject->NewDeclaration(cLex);
                nextLex();
            } else {
                errorMessage("expected ident");
            }
            declTail();
            moreDecls();
        } else {
            errorMessage("expected comma");
        }
    }


    debug("exited moreDecls");
}

void syntactic::term() {
    debug("entered term");
    if (cToken == LPAREN){
        nextLex();
        interpreterObject->otheropFound(LPAREN);
        stmt();
        if (cToken == RPAREN){
            nextLex();
            interpreterObject->otheropFound(RPAREN);
        } else {
            errorMessage("expected right parenthesis");
        }
    }else {
        pre();
        var();
        post();
    }
    debug("exited term");
}

void syntactic::pre() {
    debug("entered pre");

    if (cToken >= NOT && cToken <= MINUS) {
        interpreterObject->preopFound(cToken);
        nextLex();
        pre();
    } else {
        //nothing
    }

    debug("exited pre");
}

void syntactic::var() {
    debug("entered var");

    if (cToken == IDENT){
        interpreterObject->VariableFound(cLex);
        nextLex();
    } else if (cToken == NUMLIT){
        interpreterObject->NumLitFound(cLex);
        nextLex();
    } else {
        errorMessage("expected a variable");
    }


    debug("exited var");
}

void syntactic::post() {
    debug("entered post");

    if (cToken == PLUSPLUS || cToken == MINUSMINUS){
        interpreterObject->postopFound(cToken);
        nextLex();
    }

    debug("exited post");
}


void syntactic::stmtTail() {
    debug("entered stmtTail");
    if (cToken == SEMI){
    } else if(cToken == QUEST){
        nextLex();
        stmt();
        if (cToken == COLON){
            nextLex();
            stmt();
        } else {
            errorMessage("expected colon");
        }
    } else if (cToken >= PLUS && cToken <= GTE){
        binop();
        stmt();
    } else {}

    debug("exited stmtTail");
}

void syntactic::binop() {
    debug("entered binop");

    if (cToken >= PLUS && cToken <= GTE){
        interpreterObject->binopFound(cToken);
        nextLex();
    } else {
        errorMessage("expected a binop");
    }

    debug("exited binop");
}
