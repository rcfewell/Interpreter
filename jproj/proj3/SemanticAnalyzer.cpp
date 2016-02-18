#include "SemanticAnalyzer.h"

void SemanticAnalyzer::checkSymbol(string t)
{
	for (int i = 0; i < symbolTable.size(); i++) {
		if (symbolTable[i].var == t) {
			if (evaluating) {
				foundNumLit(symbolTable[i].valI);
			}
			return;
		}
	}

	if (lastType != 0) {
		addSymbol(t);
	}
	else {
		cout << "UNDECLARED VARIABLE: " << t << endl;
	}

	lastType = ERROR;
	if (!evaluating)
		lastVar = t;
}

void SemanticAnalyzer::addSymbol(string t) 
{
	Symbol s;
	s.type = lastType;
	s.var = t;
	symbolTable.push_back(s);
}

void SemanticAnalyzer::foundNumLit(int n)
{
	operands.push_back(n);
}

void SemanticAnalyzer::foundOperator(token_type op)
{
	if (!evaluating)
		return;

	if (operators.size() > 0) {
		if (precedence(operators[operators.size() - 1]) <= precedence(op)) {
			operands[operands.size() - 2] = evalArith(operands[operands.size() - 1], operands[operands.size() - 2], operators[operators.size() - 1]);
			operands.pop_back();
			operators.pop_back();
		}
	}

	operators.push_back(op);
}

int SemanticAnalyzer::evalArith(int a, int b, token_type t)
{
	switch (t) {
	case PLUS: return a + b;
	case MULT: return a * b;
	}

	return 0;
}

void SemanticAnalyzer::startEval()
{
	evaluating = true;
}

void SemanticAnalyzer::endEval()
{
	if (!evaluating)
		return;

	evaluating = false;

	while (operands.size() > 1) {
		operands[operands.size() - 2] = evalArith(operands[operands.size() - 1], operands[operands.size() - 2], operators[operators.size() - 1]);
		operands.pop_back();
		operators.pop_back();
	}

	setVar(operands[0]);

	operands.clear();
}

void SemanticAnalyzer::setVar(int n)
{
	for (int i = 0; i < symbolTable.size(); i++) {
		if (symbolTable[i].var == lastVar)
			symbolTable[i].valI = n;
	}
}

void SemanticAnalyzer::printTable()
{
	for (int i = 0; i < symbolTable.size(); i++) {
		cout << type(symbolTable[i].type) << " " << symbolTable[i].var << " " << symbolTable[i].valI << endl;
	}
}

string SemanticAnalyzer::type(token_type t)
{
	if (t == INTTYPE) return "int";
	if (t == DBLTYPE) return "double";
}

int SemanticAnalyzer::precedence(token_type o)
{
	if (o == PLUSPLUS || o == MINUSMINUS) return 1;
	if (o == MULT || o == DIV || o == MOD) return 5;
	if (o == PLUS || o == MINUS) return 6;

	return -1;
}