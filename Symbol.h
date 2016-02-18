#ifndef SYMBOL_H
#define SYMBOL_H

#include <iostream>
#include "OpCodes.h"
#include "Interpreter.h"

using namespace std;

string toAString( int N );
int toAnInt( string S );
double toADouble( string S );

struct Interpreter::Symbol
{
	Symbol ();
	virtual symbol_type IsA () const;
	virtual string ToString () const;
	friend ostream & operator << ( ostream & outs, const Symbol & S );
	string name;
	bool operator == ( const Symbol& );
	bool operator == ( const string& );
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
	Variable (string N, value_type type);
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
	Temporary (int value, int & tNum);
	Temporary (double value, int & tNum);
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

	string getOpCodeType (OpCode_type) const;
	string getSymbolType (symbol_type) const;
	string getValueType (value_type) const;
	string getOrderType (order_type) const;
};

#endif

