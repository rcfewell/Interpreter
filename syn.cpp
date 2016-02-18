#include <iostream>
#include <fstream>
#include "Syn.h"


using namespace std;

Syn::Syn (char * filename)
{
	indent = 10;
	lex = new Lex (filename);
	interpreter = new Interpreter( filename, lex );
	token = lex->get_token ();
	program ();
}

Syn::~Syn ()
{
	delete lex;
}

void Syn::program()
{
/*
1. <program> -> <stmt> SEMI <more_stmts> EOFT
2. <program> -> <decl> SEMI <more_stmts> EOFT
*/
	lex->debug << string(2 * ++indent, ' ') << "Entering <program>\n";
	if (token == INTTYPE || token == DBLTYPE)
		decl ();
	else
		stmt ();
	if (token == SEMI)
		token = lex->get_token();
	else
	{
		lex->error_message ("Semi colon expected; " + lex->get_lexeme() + " found");
		while (token != SEMI && token != EOFT)
			token = lex->get_token();
	}
	more_stmts ();
	if (token != EOFT)
	{
		lex->error_message ("End of File expected; " + lex->get_lexeme() + " found");
		while (token != SEMI && token != EOFT)
			token = lex->get_token();
	}
	lex->debug << string(2 * indent--, ' ') << "Exiting <program>\n";
}

void Syn::more_stmts()
{
/*
3. <more_stmts> -> <stmt> SEMI <more_stmts>
4. <more_stmts> -> <decl> SEMI <more_stmts>
5. <more_stmts> -> {}
*/
	lex->debug << string(2 * ++indent, ' ') << "Entering <more_stmts>\n";
	if (token != EOFT)
	{
		if (token == INTTYPE || token == DBLTYPE)
			decl ();
		else
			stmt ();
		if (token == SEMI)
			token = lex->get_token();
		else
		{
			lex->error_message ("Semi colon expected; " + lex->get_lexeme() + " found");
			while (token != SEMI && token != EOFT)
				token = lex->get_token();
		}
		more_stmts ();
	}
	lex->debug << string(2 * indent--, ' ') << "Exiting <more_stmts>\n";
}

void Syn::stmt()
{
/*
6. <stmt> -> <term> <stmt_tail>
*/
	lex->debug << string(2 * ++indent, ' ') << "Entering <stmts>\n";
	term ();
	stmt_tail ();
	lex->debug << string(2 * indent--, ' ') << "Exiting <stmts>\n";
}

void Syn::decl()
{
/*
7. <decl> -> <ntype> IDENT <decl_tail> <more_decls>
*/
	lex->debug << string(2 * ++indent, ' ') << "Entering <decl>\n";
	ntype ();
	if (token == IDENT)
	{
		//Symbol * sem->AddVariable (lex->get_lexeme());
		token = lex->get_token();
	}
	else
	{
		lex->error_message ("Identifier expected; " + lex->get_lexeme() + " found");
		while (token != SEMI && token != EOFT)
			token = lex->get_token();
	}
	decl_tail ();
	// more_decls ();
	lex->debug << string(2 * indent--, ' ') << "Exiting <decl>\n";
}

void Syn::ntype()
{
/*
8. <ntype> -> INTTYPE
9. <ntype> -> DBLTYPE
*/
	lex->debug << string(2 * ++indent, ' ') << "Entering <ntype>\n";
	if (token == INTTYPE || token == DBLTYPE)
		token = lex->get_token();
	lex->debug << string(2 * indent--, ' ') << "Exiting <ntype>\n";
}

void Syn::decl_tail()
{
/*
10. <decl_tail> -> ASSIGN <stmt>
11. <decl_tail> -> {}
*/
	lex->debug << string(2 * ++indent, ' ') << "Entering <decl_tail>\n";
	if (token != SEMI)
	{
		if (token == ASSIGN)
			token = lex->get_token();
		else
		{
			lex->error_message ("= expected; " + lex->get_lexeme() + " found");
			while (token != SEMI && token != EOFT)
				token = lex->get_token();
		}
		stmt ();
	}
	lex->debug << string(2 * indent--, ' ') << "Exiting <decl_tail>\n";
}

void Syn::more_decls()
{
/*
12. <more_decls> -> COMMA IDENT <decl_tail> <more_decls>
13. <more_decls> -> {}
*/
// Not implemented
}

void Syn::term()
{
/*
14. <term> -> <pre> <var> <post>
15. <term> -> LPAREN <stmt> RPAREN
*/
	lex->debug << string(2 * ++indent, ' ') << "Entering <term>\n";
	if (token == LPAREN)
	{
		token = lex->get_token();
		stmt ();
		if (token == RPAREN)
			token = lex->get_token();
		else
		{
			lex->error_message (") expected; " + lex->get_lexeme() + " found");
			while (token != SEMI && token != EOFT)
				token = lex->get_token();
		}
	}
	else
	{
		pre ();
		var ();
		post ();
	}
	lex->debug << string(2 * indent--, ' ') << "Exiting <term>\n";
}

void Syn::pre()
{
/*
16. <pre> -> <uoppre> <pre>
17. <pre> -> {}
*/
	lex->debug << string(2 * ++indent, ' ') << "Entering <pre>\n";
	if (token == PLUS || token == MINUS || token == NOT ||
	    token == TILDA || token == PLUSPLUS || token == MINUSMINUS) 
	//if (token != IDENT && token != NUMLIT)
	{
		uoppre ();
		pre ();
	}
	lex->debug << string(2 * indent--, ' ') << "Exiting <pre>\n";
}

void Syn::uoppre()
{
/*
18. <uoppre> -> PLUS
19. <uoppre> -> MINUS
20. <uoppre> -> NOT
21. <uoppre> -> TILDA
22. <uoppre> -> PLUSPLUS
23. <uoppre> -> MINUSMINUS
*/
	lex->debug << string(2 * ++indent, ' ') << "Entering <uoppre>\n";
	if (token == PLUS || token == MINUS || token == NOT ||
	    token == TILDA || token == PLUSPLUS || token == MINUSMINUS) 
		token = lex->get_token();
	else
	{
		lex->error_message ("+, -, !, ~, ++ or -- expected; " + lex->get_lexeme() + " found");
		while (token != IDENT && token != NUMLIT)
			token = lex->get_token();
	}
	lex->debug << string(2 * indent--, ' ') << "Exiting <uoppre>\n";
}

void Syn::var()
{
/*
24. <var> -> IDENT
25. <var> -> NUMLIT
*/
	lex->debug << string(2 * ++indent, ' ') << "Entering <var>\n";
	if (token == IDENT)
	{
		//Symbol * sem->GetVariable (lex->get_lexeme());
		token = lex->get_token();
	}
	else if (token == NUMLIT)
	{
		//Symbol * sem->AddNumber (lex->get_lexeme());
		token = lex->get_token();
	}
	else
	{
		lex->error_message ("Identifier or numeric literal expected; " + lex->get_lexeme() + " found");
		while (token != SEMI && token != EOFT)
			token = lex->get_token();
	}
	lex->debug << string(2 * indent--, ' ') << "Exiting <var>\n";
}

void Syn::post()
{
/*
26. <post> -> PLUSPLUS
27. <post> -> MINUSMINUS
28. <post> -> {}
*/
	lex->debug << string(2 * ++indent, ' ') << "Entering <post>\n";
	if (token == PLUSPLUS || token == MINUSMINUS)
		token = lex->get_token();
	lex->debug << string(2 * indent--, ' ') << "Exiting <post>\n";
}

void Syn::stmt_tail()
{
/*
29. <stmt_tail> -> <binop> <stmt>
30. <stmt_tail> -> QUEST <stmt> COLON <stmt>
31. <stmt_tail> -> {}
*/
	lex->debug << string(2 * ++indent, ' ') << "Entering <stmt_tail>\n";
	if (token != RPAREN && token != SEMI && token != EOFT)
	{
		if (token == QUEST)
		{
		}
		else
		{
			binop ();
			stmt ();
		}
	}
	lex->debug << string(2 * indent--, ' ') << "Exiting <stmt_tail>\n";
}

void Syn::binop()
{
/*
32. <binop> -> PLUS
33. <binop> -> MINUS
34. <binop> -> MULT
35. <binop> -> DIV
36. <binop> -> MOD
37. <binop> -> AND
38. <binop> -> OR
39. <binop> -> XOR
40. <binop> -> ASSIGN
41. <binop> -> LT
42. <binop> -> GT
43. <binop> -> SHIFTL
44. <binop> -> SHIFTR
45. <binop> -> PLUSEQ
46. <binop> -> MINUSEQ
47. <binop> -> MULTEQ
48. <binop> -> DIVEQ
49. <binop> -> MODEQ
50. <binop> -> NOTEQ
51. <binop> -> LOGAND
52. <binop> -> ANDEQ
53. <binop> -> LOGOR
54. <binop> -> OREQ
55. <binop> -> XOREQ
56. <binop> -> EQUALTO
57. <binop> -> SHIFTLEQ
58. <binop> -> LTE
59. <binop> -> SHIFTREQ
60. <binop> -> GTE
61. <binop> -> COMMA
62. <binop> -> EXP
63. <binop> -> EXPEQ
*/
	lex->debug << string(2 * ++indent, ' ') << "Entering <binop>\n";
	if (token == PLUS || token == MINUS || token == MULT ||
	    token == DIV || token == MOD || token == AND ||
	    token == OR || token == XOR || token == ASSIGN ||
	    token == LT || token == GT || token == SHIFTL ||
	    token == SHIFTR || token == PLUSEQ || token == MINUSEQ ||
	    token == MULTEQ || token == DIVEQ || token == MODEQ ||
	    token == NOTEQ || token == LOGAND || token == ANDEQ ||
	    token == LOGOR || token == OREQ || token == XOREQ ||
	    token == EQUALTO || token == SHIFTLEQ || token == LTE ||
	    token == SHIFTREQ || token == GTE)
		token = lex->get_token();
	else
	{
		lex->error_message ("Binary operator expected; " + lex->get_lexeme() + " found");
		while (token != SEMI && token != EOFT)
			token = lex->get_token();
	}
	lex->debug << string(2 * indent--, ' ') << "Exiting <binop>\n";
}
 
