#ifndef SEM_H
#define SEM_H

#include <vector>
#include "Lex.h"

struct Symbol
{
	void reset() 
	{
		var = "";
		type = ERROR;
		valI = 0;
		valD = 0.0;
	}

	string var;
	token_type type;
	int valI;
	double valD;
};

class SemanticAnalyzer
{
public:

	token_type lastType;
	bool evaluating;

	void checkSymbol(string t);
	void addSymbol(string t);
	void foundNumLit(int n);
	void foundOperator(token_type op);
	int evalArith(int a, int b, token_type t);
	void startEval();
	void endEval();
	void setVar(int n);
	void printTable();
	string type(token_type t);
	int precedence(token_type o);

private:
	vector<Symbol> symbolTable;
	vector<int> operands;
	vector<token_type> operators;
	string lastVar;
};

#endif
