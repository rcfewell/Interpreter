#include <iostream>
#include <fstream>
#include "Interpreter.h" 
#include "Symbol.h"
 
using namespace std;

Interpreter::Interpreter( char * filename, Lex * LP )
{
	lex = LP;

	error_found = false;
	current_type = NONE;

	cout << "Input file: " << filename << endl;
	string name = filename;
	string extension;

	if (name.length() > 3)
		extension = name.substr (name.length()-3, 3);
	

	string listingname = name.substr (0, name.length()-3) + ".sym"; 
	symbollist.open (listingname.c_str());
	symbollist << "Input file: " << filename << endl;


	operators.push_back( new Operator( "++", POSTPP, 2, 1, LEFT_TO_RIGHT, INTEGER, NONE ) );
	operators.push_back( new Operator( "--", POSTMM, 2, 1, LEFT_TO_RIGHT, INTEGER, NONE ) );
    operators.push_back( new Operator( "++", PREPP, 2, 2, RIGHT_TO_LEFT, INTEGER, NONE ) );
    operators.push_back( new Operator( "--", PREMM, 2, 2, RIGHT_TO_LEFT, INTEGER, NONE ) );
    operators.push_back( new Operator( "+", UPLUS, 2, 2, RIGHT_TO_LEFT, DOUBLE, NONE ) );
    operators.push_back( new Operator( "-", UMINUS, 2, 2, RIGHT_TO_LEFT, DOUBLE, NONE ) );
    operators.push_back( new Operator( "!", NEGATION, 1, 2, RIGHT_TO_LEFT, DOUBLE, NONE ) );
    operators.push_back( new Operator( "*", MULTIPLY, 2, 3, LEFT_TO_RIGHT, DOUBLE, DOUBLE ) );
    operators.push_back( new Operator( "/", DIVIDE, 2, 3, LEFT_TO_RIGHT, DOUBLE, DOUBLE ) );
    operators.push_back( new Operator( "%", REMAINDER, 2, 3, LEFT_TO_RIGHT, INTEGER, INTEGER ) );
    operators.push_back( new Operator( "+", BPLUS, 2, 4, LEFT_TO_RIGHT, DOUBLE, DOUBLE ) );
    operators.push_back( new Operator( "-", BMINUS, 2, 4, LEFT_TO_RIGHT, DOUBLE, DOUBLE ) );
    operators.push_back( new Operator( "<<", SHIFTLEFT, 2, 5, LEFT_TO_RIGHT, DOUBLE, DOUBLE ) );
    operators.push_back( new Operator( ">>", SHIFTRIGHT, 2, 5, LEFT_TO_RIGHT, DOUBLE, DOUBLE ) );
    operators.push_back( new Operator( "<", LESSTHAN, 2, 6, LEFT_TO_RIGHT, DOUBLE, DOUBLE ) );
    operators.push_back( new Operator( "<=", LESSTHANEQ, 2, 6, LEFT_TO_RIGHT, DOUBLE, DOUBLE ) );
    operators.push_back( new Operator( ">", GREATERTHAN, 2, 6, LEFT_TO_RIGHT, DOUBLE, DOUBLE ) );
    operators.push_back( new Operator( ">=", GREATERTHANEQ, 2, 6, LEFT_TO_RIGHT, DOUBLE, DOUBLE ) );
    operators.push_back( new Operator( "=", EQUAL, 2, 7, LEFT_TO_RIGHT, DOUBLE, DOUBLE ) );
    operators.push_back( new Operator( "!=", NOTEQUAL, 2, 7, LEFT_TO_RIGHT, DOUBLE, DOUBLE ) );
    operators.push_back( new Operator( "&", BITAND, 2, 8, LEFT_TO_RIGHT, DOUBLE, DOUBLE ) );
    operators.push_back( new Operator( "^", BITXOR, 2, 9, LEFT_TO_RIGHT, DOUBLE, DOUBLE ) );
    operators.push_back( new Operator( "|", BITOR, 2, 10, LEFT_TO_RIGHT, DOUBLE, DOUBLE ) );
    operators.push_back( new Operator( "&&", LOGICALAND, 2, 11, LEFT_TO_RIGHT, DOUBLE, DOUBLE ) );
    operators.push_back( new Operator( "||", LOGICALOR, 2, 12, LEFT_TO_RIGHT, DOUBLE, DOUBLE ) );
    operators.push_back( new Operator( "=", ASSIGNMENT, 2, 14, RIGHT_TO_LEFT, DOUBLE, DOUBLE ) );
    operators.push_back( new Operator( "+=", PLUSEQUAL, 2, 14, RIGHT_TO_LEFT, DOUBLE, DOUBLE ) );
    operators.push_back( new Operator( "-=", MINUSEQUAL, 2, 14, RIGHT_TO_LEFT, DOUBLE, DOUBLE ) );
    operators.push_back( new Operator( "*=", MULTEQUAL, 2, 14, RIGHT_TO_LEFT, DOUBLE, DOUBLE ) );
    operators.push_back( new Operator( "/=", DIVEQUAL, 2, 14, RIGHT_TO_LEFT, DOUBLE, DOUBLE ) );
    operators.push_back( new Operator( "%=", REMEQUAL, 2, 14, RIGHT_TO_LEFT, INTEGER, INTEGER ) );
    operators.push_back( new Operator( "<<=", SHIFTLEFTEQUAL, 2, 14, RIGHT_TO_LEFT, DOUBLE, DOUBLE ) );
    operators.push_back( new Operator( ">>=", SHIFTRIGHTEQUAL, 2, 14, RIGHT_TO_LEFT, DOUBLE, DOUBLE ) );
    operators.push_back( new Operator( "&=", BITANDEQUAL, 2, 14, RIGHT_TO_LEFT, DOUBLE, DOUBLE ) );
    operators.push_back( new Operator( "^=", BITXOREQUAL, 2, 14, RIGHT_TO_LEFT, DOUBLE, DOUBLE ) );
    operators.push_back( new Operator( "|=", BITOREQUAL, 2, 14, RIGHT_TO_LEFT, DOUBLE, DOUBLE ) );
    operators.push_back( new Operator( "(", LEFTP, 2, 15, RIGHT_TO_LEFT, NONE, NONE ) );
    operators.push_back( new Operator( ")", RIGHTP, 2, 15, RIGHT_TO_LEFT, NONE, NONE ) );

    // printTables ();
	
}

void Interpreter::printTables ()
{
	// cout << "made it into the deconstructor\n";

	symbollist << "Symbol Table Variables:" << endl;
	for( int i = 0; i < variables.size(); i++ )
		symbollist << '\t' << variables[i]->ToString () << endl;
	symbollist << endl;

	symbollist << "Symbol Table Numbers:" << endl;
	for( int i = 0; i < numbers.size(); i++ )
		symbollist << '\t' << numbers[i]->ToString () << endl;
	symbollist << endl;

	symbollist << "Symbol Table Temporaries:" << endl;
	symbollist << endl;

	symbollist << "Symbol Table Operators:" << endl;
	for( int i = 0; i < operators.size(); i++ )
		symbollist << '\t' << operators[i]->ToString () << endl;
	symbollist << endl;
}

Interpreter::~Interpreter ()
{
	symbollist << "Symbol Table Variables:" << endl;
	for( int i = 0; i < variables.size(); i++ )
		symbollist << '\t' << variables[i]->ToString () << endl;
	symbollist << endl;

	symbollist << "Symbol Table Numbers:" << endl;
	for( int i = 0; i < numbers.size(); i++ )
		symbollist << '\t' << numbers[i]->ToString () << endl;
	symbollist << endl;

	symbollist << "Symbol Table Temporaries:" << endl;
	symbollist << endl;

	symbollist << "Symbol Table Operators:" << endl;
	for( int i = 0; i < operators.size(); i++ )
		symbollist << '\t' << operators[i]->ToString () << endl;
	symbollist << endl;
	
	symbollist.close();
}

void Interpreter::SaveDeclType (token_type T)
{
	if( T == INTTYPE )
		current_type = INTEGER;
	else if( T == DBLTYPE )
		current_type = DOUBLE;
	else{}

}

void Interpreter::NewDeclaration (string L)		
{
	for( int i = 0; i < variables.size(); i++ )
	{
		if( *variables[i] == L )
			variables.erase( variables.begin () + i );
	}

	Variable * newVar = new Variable( L, current_type );
	variables.push_back( newVar );
	Q.push_back( newVar );
}

void Interpreter::NumLitFound (string N)
{
	for (int i = 0; i < numbers.size(); i++){
        if (*numbers[i] == N){
            Q.push_back(numbers[i]);
            return;
        }
    }

    Number * newNumLit = new Number(N);
    numbers.push_back(newNumLit);
    Q.push_back(newNumLit);

}


void Interpreter::VariableFound (string N)
{
	for( int i = 0; i < variables.size(); i++ )
	{
		if( *variables[i] == N )
		{
			Q.push_back( variables[i] );
			return;
		}
	}
}

void Interpreter::foundPreop(token_type t) 
{
    
    if (t == NOT) 
    {
        Q.push_back(operators[NEGATION]);
        return;
    } 
    else if (t == PLUSPLUS) 
    {
        Q.push_back(operators[PREPP]);
        return;
    } 
    else if (t == MINUSMINUS) 
    {
        Q.push_back(operators[PREMM]);
        return;
    } 
    else if (t == PLUS) 
    {
        Q.push_back(operators[UPLUS]);
        return;
    } 
    else 
    {
        Q.push_back(operators[UMINUS]);
        return;
    }
}

void Interpreter::foundPostop(token_type t) 
{
    if(t == PLUSPLUS)
    	Q.push_back(operators[POSTPP]);
    else 
    	Q.push_back(operators[POSTMM]);
}
    
void Interpreter::foundBinop(token_type t) 
{
    if (t == PLUS)
    {
        Q.push_back(operators[BPLUS]);
        return;
    } 
    else if (t == MINUS)
    {
        Q.push_back(operators[BMINUS]);
        return;
    } 
    else if (t == MULT)
    {
        Q.push_back(operators[MULTIPLY]);
        return;
    } 
    else if (t == DIV)
    {
        Q.push_back(operators[DIVIDE]);
        return;
    } 
    else if (t == MOD)
    {
        Q.push_back(operators[REMAINDER]);
        return;
    } 
    else if (t == AND)
    {
        Q.push_back(operators[BITAND]);
        return;
    } 
    else if (t == OR)
    {
        Q.push_back(operators[BITOR]);
        return;
    } 
    else if (t == XOR)
    {
        Q.push_back(operators[BITXOR]);
        return;
    } 
    else if (t == ASSIGN)
    {
        Q.push_back(operators[ASSIGNMENT]);
        return;
    } 
    else if (t == LT)
    {
        Q.push_back(operators[LESSTHAN]);
        return;
    } 
    else if (t == GT)
    {
        Q.push_back(operators[GREATERTHAN]);
        return;
    } 
    else if (t == SHIFTL)
    {
        Q.push_back(operators[SHIFTLEFT]);
        return;
    } 
    else if (t == SHIFTR)
    {
        Q.push_back(operators[SHIFTRIGHT]);
        return;
    } 
    else if (t == PLUSEQ)
    {
        Q.push_back(operators[PLUSEQUAL]);
        return;
    } 
    else if (t == MINUSEQ)
    {
        Q.push_back(operators[MINUSEQUAL]);
        return;
    } 
    else if (t == MULTEQ)
    {
        Q.push_back(operators[MULTEQUAL]);
        return;
    } 
    else if (t == DIVEQ)
    {
        Q.push_back(operators[DIVEQUAL]);
        return;
    } 
    else if (t == NOTEQ)
    {
        Q.push_back(operators[NOTEQUAL]);
        return;
    } 
    else if (t == LOGAND)
    {
        Q.push_back(operators[LOGICALAND]);
        return;
    } 
    else if (t == ANDEQ)
    {
        Q.push_back(operators[BITANDEQUAL]);
        return;
    } 
    else if (t == LOGOR)
    {
        Q.push_back(operators[LOGICALOR]);
        return;
    } 
    else if (t == OREQ)
    {
        Q.push_back(operators[BITOREQUAL]);
        return;
    } 
    else if (t == XOREQ)
    {
        Q.push_back(operators[BITXOREQUAL]);
        return;
    } 
    else if (t == EQUALTO)
    {
        Q.push_back(operators[EQUAL]);
        return;
    } 
    else if (t == SHIFTLEQ)
    {
        Q.push_back(operators[SHIFTLEFTEQUAL]);
        return;
    } 
    else if (t == LTE)
    {
        Q.push_back(operators[LESSTHANEQ]);
        return;
    } 
    else if (t == SHIFTREQ)
    {
        Q.push_back(operators[SHIFTRIGHTEQUAL]);
        return;
    } 
    else if (t == GTE)
    {
        Q.push_back(operators[GREATERTHANEQ]);
        return;
    }

}

void Interpreter::foundOtherOp( token_type t)
{
	if (t == RPAREN)
	{
        Q.push_back(operators[RIGHTP]);
        return;
    }
    else if (t == LPAREN)
    {
        Q.push_back(operators[LEFTP]);
        return;
    } 
    else if (t == COMMA)
    {
        Q.push_back(operators[BITCOMMA]);
        return;
    } 
    else if (t == COLON)
    {
        // colon
    } 
    else if (t == QUEST)
    {
        // something with a question mark
    } 
    else if (t == SEMI) 
    {
        // should never get here
   	}	
    // error

}








