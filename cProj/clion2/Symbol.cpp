#include <iomanip>
#include <sstream>
#include <string>

#include "Symbol.h"
#include "Interpreter.h"

using namespace std;
Interpreter::Symbol::Symbol() {

}

symbol_type Interpreter::Symbol::IsA() const {
    return OPERAND;
}

string Interpreter::Symbol::ToString() const {
    return "";
}


bool Interpreter::Symbol::operator == (Interpreter::Symbol const &other) {
    if (this->name == other.name){
        return true;
    }
    return false;
}


bool Interpreter::Symbol::operator == (string const &aConst) {
    if (this->name == aConst)
        return true;
    return false;
}

Interpreter::Operand::Operand() {

}

symbol_type Interpreter::Operand::IsA() const {
    return OPERAND;
}

Interpreter::Variable::Variable() {

}

Interpreter::Variable::Variable(string N, value_type type) {
    name = N;
    vType = type;
    iVal = 0;
    dVal = 0.0;
}

symbol_type Interpreter::Variable::IsA() const {
    return VARIABLE;
}

string Interpreter::Variable::ToString() const {
    stringstream stream;
    stream << setw(3) << " ";
    stream << setw(10) << left <<  name;

    if (vType == INTEGER){
        stream << setw(10) << left << "INTEGER";
        stream << setw(10) << left << to_string(iVal);
    } else {
        stream << setw(10) << left << "DOUBLE";
        stream << setw(10) << left << to_string(dVal);
    }

    return stream.str();
}

Interpreter::Number::Number() {

}

Interpreter::Number::Number(string N) {
    name = N;

    if(N.find(".") != string::npos){
        vType = DOUBLE;
    } else {
        vType = INTEGER;
    }

    dVal = stod(N);
    iVal = stoi(N);
}

symbol_type Interpreter::Number::IsA() const {
    return NUMBER;
}

string Interpreter::Number::ToString() const {
    stringstream stream;
    stream << setw(3) << ' ';

    if (vType == INTEGER){
        stream << setw(10) << left << "INTEGER";
        stream << setw(10) << left << to_string(iVal);
    } else {
        stream << setw(10) << left << "DOUBLE";
        stream << setw(10) << left << to_string(dVal);
    }

    return stream.str();
}

Interpreter::Temporary::Temporary() {

}

Interpreter::Temporary::Temporary(int value, int & tNum) {
    stringstream stream;
    stream << '$' << tNum++;

    name = stream.str();
    vType = INTEGER;
    iVal = value;
    dVal = value;

}

Interpreter::Temporary::Temporary(double value, int & tNum) {
    stringstream stream;
    stream << '$' << tNum++;

    name = stream.str();
    vType = DOUBLE;
    iVal = value;
    dVal = value;

}

symbol_type Interpreter::Temporary::IsA() const {
    return TEMPORARY;
}

string Interpreter::Temporary::ToString() const {
    stringstream stream;
    stream << setw(3) << " ";
    stream << setw(10) << left <<  name;

    if (vType == INTEGER){
        stream << setw(10) << left << "INTEGER";
        stream << setw(10) << left << to_string(iVal);
    } else {
        stream << setw(10) << left << "DOUBLE";
        stream << setw(10) << left << to_string(dVal);
    }

    return stream.str();
}

Interpreter::Operator::Operator() {

}

Interpreter::Operator::Operator(const string &N, OpCode_type C, int A, int P, order_type O, value_type L, value_type R) {
    name = N;
    opcode = C;
    arguments = A;
    priority = P;
    order = O;
    validLeft = L;
    validRight = R;
}

symbol_type Interpreter::Operator::IsA() const {
    return OPERATOR;
}

string Interpreter::Operator::ToString() const {
    stringstream stream;

    string ret;

    stream << setw(4) << "" << setw(10)  << left << name
            << setw(22) << left << getOpCodeType(opcode)
            << setw(10) << left << to_string(arguments)
            << setw(10) << left << to_string(priority)
            << setw(20) << left << getOrderType(order)
            << setw(15) << left << getValueType(validLeft)
            << setw(15) << left << getValueType(validRight);


    return stream.str();
}

string Interpreter::Operator::getOpCodeType(OpCode_type type) const {
    string opCodeNames [] = {"POSTPP", "POSTMM", "PREPP", "PREMM", "UPLUS", "UMINUS", "NEGATION", "MULTIPLY", "DIVIDE",
            "REMAINDER", "BPLUS", "BMINUS", "SHIFTLEFT", "SHIFTRIGHT", "LESSTHAN", "LESSTHANEQ",
            "GREATERTHAN", "GREATERTHANEQ", "EQUAL", "NOTEQUAL", "BITAND", "BITXOR", "BITOR",
            "LOGICALAND", "LOGICALOR", "ASSIGNMENT", "PLUSEQUAL", "MINUSEQUAL", "MULTEQUAL",
            "DIVEQUAL", "REMEQUAL", "SHIFTLEFTEQUAL", "SHIFTRIGHTEQUAL", "BITANDEQUAL",
            "BITXOREQUAL", "BITOREQUAL", "LEFTP", "RIGHTP"};

    return opCodeNames[type];
}

string Interpreter::Operator::getSymbolType(symbol_type type) const {
    string symbolTypeNames [] = {
            "SYMBOL", "OPERAND", "VARIABLE", "NUMBER", "TEMPORARY", "OPERATOR"
    };

    return symbolTypeNames [type];
}

string Interpreter::Operator::getValueType(value_type type) const {
    std::string valueTypeNames[] = {
            "NONE", "INTEGER", "DOUBLE"
    };

    return valueTypeNames[type];
}

string Interpreter::Operator::getOrderType(order_type type) const {
    string orderTypeNames [] ={
            "LEFT_TO_RIGHT", "RIGHT_TO_LEFT"
    };

    return orderTypeNames[type];
}
