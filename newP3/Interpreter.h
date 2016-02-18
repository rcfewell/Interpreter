#ifndef INT_H
#define INT_H

#include <iostream>
#include <fstream>
#include <vector>
#include "lex.h"
#include "OpCodes.h"

using namespace std;

enum symbol_type {SYMBOL, OPERAND, VARIABLE, NUMBER, TEMPORARY, OPERATOR};

enum value_type {NONE, INTEGER, DOUBLE};

enum order_type {LEFT_TO_RIGHT, RIGHT_TO_LEFT};


class Interpreter 
{
    public:
		Interpreter (string, lex * LP);
		~Interpreter ();
		void SaveDeclType (tokenType T);
		void NewDeclaration (string L);
		void NumLitFound (string N);
		void VariableFound (string N);
		void preopFound (tokenType);
		void postopFound(tokenType);
		void binopFound(tokenType);
		void otheropFound (tokenType);
		void EvalQ();
		friend ostream & operator << (ostream & outs, const Interpreter & I);
		struct Symbol;
		struct Operand;
		struct Variable;
		struct Number;
		struct Temporary;
		struct Operator;

		//F (*function) (int, int);


    private:
		vector <Variable *> variables;
		vector <Number *> numbers;
		vector <Temporary *> temporaries;
		vector <Operator *> operators;
		vector <Symbol *> aLine;

		lex * lexObject;
		ofstream sym;

		bool error_found;
		bool typeSet;

		int currentTempNum;
		value_type currentType;
};

//F postpp (int, int);
//F postmm (int, int);
//F prepp (int, int);
//F premm (int, int);
//F uplus (int, int);
//F uminus (int, int);
//F negation (int, int);
//F multipul (int, int);
//F divide (int, int);
//F remainder (int, int);
//F bplus (int, int);
//F bminus (int, int);
//F shiftleft (int, int);
//F shiftright (int, int);
//F lessthan (int, int);
//F lessthaneq (int, int);
//F greaterthan (int, int);
//F greaterthaneq (int, int);
//F equal (int, int);
//F notequal (int, int);
//F bitandF (int, int);
//F bitxor (int, int);
//F bitoF (int, int);
//F logicaland (int, int);
//F logicalor (int, int);
//F assignment (int, int);
//F plusequal (int, int);
//F minusequal (int, int);
//F multequal (int, int);
//F divequal (int, int);
//F remequal (int, int);
//F shiftleftequal (int, int);
//F shiftrightequal (int, int);
//F bitandequal (int, int);
//F bitxorequal (int, int);
//F bitorequal (int, int);


#include "Symbol.h"
#endif
