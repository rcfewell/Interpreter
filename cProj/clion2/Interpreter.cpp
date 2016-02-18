#include "Interpreter.h"
#include "lex.h"

//evalOp::evalOp(F f (int, int), OpCode_type OC){
//    function = f;
//    opCode = OC;
//}

Interpreter::Interpreter(string filename, lex * LP) {
    lexObject = LP;
    error_found = false;
    currentType = NONE;
    sym.open(filename.c_str());
    currentTempNum = 0;

    operators.push_back(new Operator("++", POSTPP, 2, 1, LEFT_TO_RIGHT, INTEGER, NONE));
    operators.push_back(new Operator("--", POSTMM, 2, 1, LEFT_TO_RIGHT, INTEGER, NONE));
    operators.push_back(new Operator("++", PREPP, 2, 2, RIGHT_TO_LEFT, INTEGER, NONE));
    operators.push_back(new Operator("--", PREMM, 2, 2, RIGHT_TO_LEFT, INTEGER, NONE));
    operators.push_back(new Operator("+", UPLUS, 2, 2, RIGHT_TO_LEFT, DOUBLE, NONE));
    operators.push_back(new Operator("-", UMINUS, 2, 2, RIGHT_TO_LEFT, DOUBLE, NONE));
    operators.push_back(new Operator("!", NEGATION, 1, 2, RIGHT_TO_LEFT, DOUBLE, NONE));
    operators.push_back(new Operator("*", MULTIPLY, 2, 3, LEFT_TO_RIGHT, DOUBLE, DOUBLE));
    operators.push_back(new Operator("/", DIVIDE, 2, 3, LEFT_TO_RIGHT, DOUBLE, DOUBLE));
    operators.push_back(new Operator("%", REMAINDER, 2, 3, LEFT_TO_RIGHT, INTEGER, INTEGER));
    operators.push_back(new Operator("+", BPLUS, 2, 4, LEFT_TO_RIGHT, DOUBLE, DOUBLE));
    operators.push_back(new Operator("-", BMINUS, 2, 4, LEFT_TO_RIGHT, DOUBLE, DOUBLE));
    operators.push_back(new Operator("<<", SHIFTLEFT, 2, 5, LEFT_TO_RIGHT, DOUBLE, DOUBLE));
    operators.push_back(new Operator(">>", SHIFTRIGHT, 2, 5, LEFT_TO_RIGHT, DOUBLE, DOUBLE));
    operators.push_back(new Operator("<", LESSTHAN, 2, 6, LEFT_TO_RIGHT, DOUBLE, DOUBLE));
    operators.push_back(new Operator("<=", LESSTHANEQ, 2, 6, LEFT_TO_RIGHT, DOUBLE, DOUBLE));
    operators.push_back(new Operator(">", GREATERTHAN, 2, 6, LEFT_TO_RIGHT, DOUBLE, DOUBLE));
    operators.push_back(new Operator(">=", GREATERTHANEQ, 2, 6, LEFT_TO_RIGHT, DOUBLE, DOUBLE));
    operators.push_back(new Operator("=", EQUAL, 2, 7, LEFT_TO_RIGHT, DOUBLE, DOUBLE));
    operators.push_back(new Operator("!=", NOTEQUAL, 2, 7, LEFT_TO_RIGHT, DOUBLE, DOUBLE));
    operators.push_back(new Operator("&", BITAND, 2, 8, LEFT_TO_RIGHT, DOUBLE, DOUBLE));
    operators.push_back(new Operator("^", BITXOR, 2, 9, LEFT_TO_RIGHT, DOUBLE, DOUBLE));
    operators.push_back(new Operator("|", BITOR, 2, 10, LEFT_TO_RIGHT, DOUBLE, DOUBLE));
    operators.push_back(new Operator("&&", LOGICALAND, 2, 11, LEFT_TO_RIGHT, DOUBLE, DOUBLE));
    operators.push_back(new Operator("||", LOGICALOR, 2, 12, LEFT_TO_RIGHT, DOUBLE, DOUBLE));
    operators.push_back(new Operator("=", ASSIGNMENT, 2, 14, RIGHT_TO_LEFT, DOUBLE, DOUBLE));
    operators.push_back(new Operator("+=", PLUSEQUAL, 2, 14, RIGHT_TO_LEFT, DOUBLE, DOUBLE));
    operators.push_back(new Operator("-=", MINUSEQUAL, 2, 14, RIGHT_TO_LEFT, DOUBLE, DOUBLE));
    operators.push_back(new Operator("*=", MULTEQUAL, 2, 14, RIGHT_TO_LEFT, DOUBLE, DOUBLE));
    operators.push_back(new Operator("/=", DIVEQUAL, 2, 14, RIGHT_TO_LEFT, DOUBLE, DOUBLE));
    operators.push_back(new Operator("%=", REMEQUAL, 2, 14, RIGHT_TO_LEFT, INTEGER, INTEGER));
    operators.push_back(new Operator("<<=", SHIFTLEFTEQUAL, 2, 14, RIGHT_TO_LEFT, DOUBLE, DOUBLE));
    operators.push_back(new Operator(">>=", SHIFTRIGHTEQUAL, 2, 14, RIGHT_TO_LEFT, DOUBLE, DOUBLE));
    operators.push_back(new Operator("&=", BITANDEQUAL, 2, 14, RIGHT_TO_LEFT, DOUBLE, DOUBLE));
    operators.push_back(new Operator("^=", BITXOREQUAL, 2, 14, RIGHT_TO_LEFT, DOUBLE, DOUBLE));
    operators.push_back(new Operator("|=", BITOREQUAL, 2, 14, RIGHT_TO_LEFT, DOUBLE, DOUBLE));
    operators.push_back(new Operator("(", LEFTP, 2, 15, RIGHT_TO_LEFT, NONE, NONE));
    operators.push_back(new Operator(")", RIGHTP, 2, 15, RIGHT_TO_LEFT, NONE, NONE));



}

Interpreter::~Interpreter() {
    // TODO Sort lists

    sym << "Symbol Table Variables" << endl;
    for (int i = 0; i < variables.size(); i++){
        sym << '\t' << variables[i]->ToString() << endl;
    }
    sym << endl;

    sym << "Symbol Table Numbers" << endl;
    for (int i = 0; i < numbers.size(); i++){
        sym << '\t' << numbers[i]->ToString() << endl;
    }
    sym << endl;


    sym << "Symbol Table Temporaries" << endl;
        // TODO print temps
    sym << endl;



    sym << "Symbol Table Operands" << endl;
        for (int i = 0; i < operators.size(); i++){
            sym << '\t' << operators[i]->ToString() << endl;
        }
    sym << endl;



}

void Interpreter::SaveDeclType(tokenType type) {
    if (type == INTTYPE){
        currentType = INTEGER;
    } else if (type == DBLTYPE){
        currentType = DOUBLE;
    } else {
        // error
    }
}

void Interpreter::NewDeclaration(string L) {
    for (int i = 0; i < variables.size(); i++){
        if (*variables[i] == L){
            variables.erase(variables.begin() + i);
        }
    }
    Variable * newNew = new Variable(L, currentType);
    variables.push_back(newNew);
    aLine.push_back(newNew);
}

void Interpreter::NumLitFound(string N) {
    for (int i = 0; i < numbers.size(); i++){
        if (*numbers[i] == N){
            aLine.push_back(numbers[i]);
            return;
        }
    }

    Number * chee = new Number(N);
    numbers.push_back(chee);
    aLine.push_back(chee);
}


void Interpreter::VariableFound(string N) {
    for (int i = 0; i < variables.size(); i++){
        if (*variables[i] == N){
            aLine.push_back(variables[i]);
            return;
        }
    }

    //TODO: error var not found
}

void Interpreter::preopFound(tokenType type) {
    if (type == NOT) {
        aLine.push_back(operators[NEGATION]);
        return;
    } else if (type == PLUSPLUS) {
        aLine.push_back(operators[PREPP]);
        return;
    } else if (type == MINUSMINUS) {
        aLine.push_back(operators[PREMM]);
        return;
    } else if (type == PLUS) {
        aLine.push_back(operators[UPLUS]);
        return;
    } else {
        aLine.push_back(operators[UMINUS]);
        return;
    }
}



void Interpreter::postopFound(tokenType type) {
    if(type == PLUSPLUS){
        aLine.push_back(operators[POSTPP]);
    } else {
        aLine.push_back(operators[POSTMM]);
    }
}

void Interpreter::binopFound(tokenType type) {
    if (type == PLUS){
        aLine.push_back(operators[BPLUS]);
        return;
    } else if (type == MINUS){
        aLine.push_back(operators[BMINUS]);
        return;
    } else if (type == MULT){
        aLine.push_back(operators[MULTIPLY]);
        return;
    } else if (type == DIV){
        aLine.push_back(operators[DIVIDE]);
        return;
    } else if (type == MOD){
        aLine.push_back(operators[REMAINDER]);
        return;
    } else if (type == AND){
        aLine.push_back(operators[BITAND]);
        return;
    } else if (type == OR){
        aLine.push_back(operators[BITOR]);
        return;
    } else if (type == XOR){
        aLine.push_back(operators[BITXOR]);
        return;
    } else if (type == ASSIGN){
        aLine.push_back(operators[ASSIGNMENT]);
        return;
    } else if (type == LT){
        aLine.push_back(operators[LESSTHAN]);
        return;
    } else if (type == GT){
        aLine.push_back(operators[GREATERTHAN]);
        return;
    } else if (type == SHIFTL){
        aLine.push_back(operators[SHIFTLEFT]);
        return;
    } else if (type == SHIFTR){
        aLine.push_back(operators[SHIFTRIGHT]);
        return;
    } else if (type == PLUSEQ){
        aLine.push_back(operators[PLUSEQUAL]);
        return;
    } else if (type == MINUSEQ){
        aLine.push_back(operators[MINUSEQUAL]);
        return;
    } else if (type == MULTEQ){
        aLine.push_back(operators[MULTEQUAL]);
        return;
    } else if (type == DIVEQ){
        aLine.push_back(operators[DIVEQUAL]);
        return;
    } else if (type == NOTEQ){
        aLine.push_back(operators[NOTEQUAL]);
        return;
    } else if (type == LOGAND){
        aLine.push_back(operators[LOGICALAND]);
        return;
    } else if (type == ANDEQ){
        aLine.push_back(operators[BITANDEQUAL]);
        return;
    } else if (type == LOGOR){
        aLine.push_back(operators[LOGICALOR]);
        return;
    } else if (type == OREQ){
        aLine.push_back(operators[BITOREQUAL]);
        return;
    } else if (type == XOREQ){
        aLine.push_back(operators[BITXOREQUAL]);
        return;
    } else if (type == EQUALTO){
        aLine.push_back(operators[EQUAL]);
        return;
    } else if (type == SHIFTLEQ){
        aLine.push_back(operators[SHIFTLEFTEQUAL]);
        return;
    } else if (type == LTE){
        aLine.push_back(operators[LESSTHANEQ]);
        return;
    } else if (type == SHIFTREQ){
        aLine.push_back(operators[SHIFTRIGHTEQUAL]);
        return;
    } else if (type == GTE){
        aLine.push_back(operators[GREATERTHANEQ]);
        return;
    }


    // error

}


void Interpreter::otheropFound(tokenType type) {
    if (type == RPAREN){
        aLine.push_back(operators[RIGHTP]);
        return;
    } else if (type == LPAREN){
        aLine.push_back(operators[LEFTP]);
        return;
    } else if (type == COMMA){
        aLine.push_back(operators[BITCOMMA]);
        return;
    } else if (type == COLON){
        // colon
    } else if (type == QUEST){
        // something with a question mark
    } else if (type == SEMI) {
        // should never get here
    }

    // error

}

void Interpreter::EvalQ() {
    currentType = NONE;
}

/*
F postpp(int i, int i1) {
    return nullptr;
}

F postmm(int i, int i1) {
    return nullptr;
}

F prepp(int i, int i1) {
    return nullptr;
}

F premm(int i, int i1) {
    return nullptr;
}

F uplus(int i, int i1) {
    return nullptr;
}

F uminus(int i, int i1) {
    return nullptr;
}

F negation(int i, int i1) {
    return nullptr;
}

F multipul(int i, int i1) {
    return nullptr;
}

F divide(int i, int i1) {
    return nullptr;
}

F remainder(int i, int i1) {
    return nullptr;
}

F bplus(int i, int i1) {
    return nullptr;
}

F bminus(int i, int i1) {
    return nullptr;
}

F shiftleft(int i, int i1) {
    return nullptr;
}

F shiftright(int i, int i1) {
    return nullptr;
}

F lessthan(int i, int i1) {
    return nullptr;
}

F lessthaneq(int i, int i1) {
    return nullptr;
}

F greaterthan(int i, int i1) {
    return nullptr;
}

F greaterthaneq(int i, int i1) {
    return nullptr;
}

F equal(int i, int i1) {
    return nullptr;
}

F bitandF(int i, int i1) {
    return nullptr;
}

F bitxor(int i, int i1) {
    return nullptr;
}

F bitoF(int i, int i1) {
    return nullptr;
}

F logicaland(int i, int i1) {
    return nullptr;
}

F logicalor(int i, int i1) {
    return nullptr;
}

F assignment(int i, int i1) {
    return nullptr;
}

F plusequal(int i, int i1) {
    return nullptr;
}

F minusequal(int i, int i1) {
    return nullptr;
}

F divequal(int i, int i1) {
    return nullptr;
}

F remequal(int i, int i1) {
    return nullptr;
}

F shiftleftequal(int i, int i1) {
    return nullptr;
}

F bitandequal(int i, int i1) {
    return nullptr;
}

F bitxorequal(int i, int i1) {
    return nullptr;
}

F bitorequal(int i, int i1) {
    return nullptr;
}

F notequal(int i, int i1) {
    return nullptr;
}

F multequal(int i, int i1) {
    return nullptr;
}

F shiftrightequal(int i, int i1) {
    return nullptr;
}
*/
