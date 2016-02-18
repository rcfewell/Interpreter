#ifndef SYMBOL_H
#define SYMBOL_H

#include <iostream>
#include "OpCodes.h"

using namespace std;

struct Interpreter::Symbol
{
	Symbol ();
	virtual symbol_type IsA () const;
	virtual string ToString () const;
	friend ostream & operator << (ostream & outs, const Symbol & S);
	string name;
};	
struct Interpreter::Operand : public Symbol
{
	Operand ();
	symbol_type IsA () const;
	value_type vType;
	int iVal;
	double dVal;
};

struct Interpreter::Variable : public Operand
{
	Variable ();
	Variable (string N, value_type T);
	symbol_type IsA () const;
	string ToString () const;
};

struct Interpreter::Number : public Operand
{
	Number ();
	Number (string N);
	symbol_type IsA () const;
	string ToString () const;
};

struct Interpreter::Temporary : public Operand
{
	Temporary ();
	Temporary (int value);
	Temporary (double value);
	symbol_type IsA () const;
	string ToString () const;
};

struct Interpreter::Operator : public Symbol
{
	Operator ();
	Operator (const string & N, OpCode_type C, int A, int P, order_type O, value_type L, value_type R);
	symbol_type IsA () const;
	string ToString () const;
	OpCode_type opcode;
	int arguments;
	int priority;
	order_type order;
	value_type validLeft;
	value_type validRight;
};

#endif
