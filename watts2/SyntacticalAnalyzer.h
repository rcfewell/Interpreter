#ifndef SYN_H
#define SYN_H

#include <iostream>
#include <fstream>
#include "LexicalAnalyzer.h"

using namespace std;

class SyntacticalAnalyzer 
{
    public:
	SyntacticalAnalyzer (char * filename);
	~SyntacticalAnalyzer ();
	void program();
	void more_stmts();
	void stmt();
	void decl();
	void ntype();
	void decl_tail();
	void more_decls();
	void term();
	void pre();
	void uoppre();
	void var();
	void post();
	void stmt_tail();
	void binop();
    private:
	LexicalAnalyzer * lex;
	int errors;
	token_type token;
	int indent;
};
	
#endif
