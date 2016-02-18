#ifndef LEX_H
#define LEX_H

#include <iostream>
#include <fstream>

using namespace std;

enum token_type {ERROR, IDENT, INTTYPE, DBLTYPE, NUMLIT, PLUS, MINUS, MULT,
		DIV, MOD, EXP, NOT, AND, OR, XOR, ASSIGN, LT, GT, SHIFTL,
		SHIFTR, PLUSPLUS, PLUSEQ, MINUSMINUS, MINUSEQ, MULTEQ,
		DIVEQ, MODEQ, EXPEQ, NOTEQ, LOGAND, ANDEQ, LOGOR, OREQ,
		XOREQ, EQUALTO, SHIFTLEQ, LTE, SHIFTREQ, GTE, TILDA, RPAREN,
		LPAREN, SEMI, QUEST, COLON, COMMA, EOFT};

class LexicalAnalyzer 
{
    public:
	LexicalAnalyzer (char * filename);
	~LexicalAnalyzer ();
	token_type get_token ();
	token_type find_token ();
	string get_token_name (token_type t) const;
	string get_lexeme () const;
	void error_message (const string & msg);
	ofstream debug;
    private:
	ifstream input;
	ofstream listing;
	string line;
	int linenum;
	int pos;
	string lexeme;
	int errors;
};
	
#endif
