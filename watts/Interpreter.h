#ifndef INT_H
#define INT_H

#include <iostream>
#include <fstream>
#include <vector>
#include "LexicalAnalyzer.h"

using namespace std;

enum symbol_type {SYMBOL, OPERAND, VARIABLE, NUMBER, TEMPORARY, OPERATOR};
enum value_type {NONE, INTEGER, DOUBLE};
enum order_type {LEFT_TO_RIGHT, RIGHT_TO_LEFT};

class Interpreter 
{
    public:
	Interpreter (char * filename, LexicalAnalyzer * LP);
	~Interpreter ();
	void SaveDeclType (token_type T);
	void NewDeclaration (string L);
	void NumLitFound (string N);
	void VariableFound (string N);
	friend ostream & operator << (ostream & outs, const Interpreter & I);
	struct Symbol;
	struct Operand;
	struct Variable;
	struct Number;
	struct Temporary;
	struct Operator;
    private:
	vector <Variable *> variables;
	vector <Number *> numbers;
	vector <Temporary *> temporaries;
	vector <Operator *> operators;
	LexicalAnalyzer * lex;
	ofstream symbollist;
	bool error_found;
	value_type current_type;
};
	
#include "Symbol.h"
#endif
