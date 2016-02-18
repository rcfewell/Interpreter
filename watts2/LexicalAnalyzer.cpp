#include <iomanip>
#include <cstdlib>
#include "LexicalAnalyzer.h"

using namespace std;

static string token_names[] = {"ERROR", "IDENT", "INTTYPE", "DBLTYPE", "NUMLIT",
	"PLUS", "MINUS", "MULT", "DIV", "MOD", "EXP", "NOT", "AND", "OR",
	"XOR", "ASSIGN", "LT", "GT", "SHIFTL", "SHIFTR", "PLUSPLUS", "PLUSEQ",
	"MINUSMINUS", "MINUSEQ", "MULTEQ", "DIVEQ", "MODEQ", "EXPEQ", "NOTEQ",
	"LOGAND", "ANDEQ", "LOGOR", "OREQ", "XOREQ", "EQUALTO", "SHIFTLEQ",
	"LTE", "SHIFTREQ", "GTE", "TILDA", "RPAREN", "LPAREN", "SEMI", "QUEST",
	"COLON", "COMMA", "EOFT"};

static int table [][25] = 
//	  a   _   0   .   ;   +   -   /   *   %   =   <   >   &   ^   |   !   ~   (   )   ;   ?   :   ,  other
       {{ 1,  1,  2,  3, 13,  5,  6, 13, 13, 13, 13,  7,  9, 11, 13, 12, 13, 23, 23, 23, 23, 23, 23, 23, 32},  // 0 starting
	{ 1,  1,  1, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21},  // 1 alpha or _ (or  number)
	{22, 22,  2,  4, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22},  // 2 number
	{31, 31,  4, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31},  // 3 period
	{22, 22,  4, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22},  // 4 number for decimal
	{24, 24, 24, 24, 24, 23, 24, 24, 24, 24, 23, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24},  // 5 plus
	{24, 24, 24, 24, 24, 24, 23, 24, 24, 24, 23, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24},  // 6 minus
	{24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 23,  8, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24},  // 7 less than
	{24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 23, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24},  // 8 second less than
	{24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 23, 24, 10, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24},  // 9 greater than
	{24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 23, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24},  // 10 second greater than
	{24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 23, 24, 24, 23, 24, 23, 24, 24, 24, 24, 24, 24, 24, 24, 24},  // 11 ampersand
	{24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 23, 24, 24, 24, 24, 23, 24, 24, 24, 24, 24, 24, 24, 24, 24},  // 12 vertical bar
	{24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 23, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24}}; // 13 any other symbol

LexicalAnalyzer::LexicalAnalyzer (char * filename)
{
	cout << "Input file: " << filename << endl;
	string name = filename;
	string extension;
	if (name.length() > 3)
		extension = name.substr (name.length()-3, 3);
	if (extension != ".in")
	{
		cout << "Invalid file extension; must be '.in'\n";
		exit (1);
	}
	input.open (filename);
	if (input.fail())
	{
		cout << "File " << filename << " not found\n";
		exit (2);
	}
	string listingname = name.substr (0, name.length()-3) + ".lst"; 
	listing.open (listingname.c_str());
	listing << "Input file: " << filename << endl;
	string debugname = name.substr (0, name.length()-3) + ".dbg"; 
	debug.open (debugname.c_str());
	debug << "Input file: " << filename << endl;
	line = " ";
	linenum = 0;
	pos = 0;
	lexeme = "";
	errors = 0;
}

LexicalAnalyzer::~LexicalAnalyzer ()
{
	input.close ();
	cout << errors << " errors found in input file\n";
	listing << errors << " errors found in input file\n";
	listing.close();
	debug << errors << " errors found in input file\n";
	debug.close();
}

token_type LexicalAnalyzer::get_token ()
{
	token_type t = find_token ();;
	debug << '\t' << left << setw (10) << get_token_name(t)
	     << '\t' << get_lexeme() << endl << right;
	return t;
}

token_type LexicalAnalyzer::find_token ()
{
	static string valid = "a_0.;+-/*%=<>&^|!~();?:,";
	while (isspace(line[pos]))
		if (++pos >= line.length())
		{
			getline (input, line);
			if (input.fail())
			{
				lexeme = "eof";
				return EOFT;
			}
			linenum++;
			listing << setw(4) << linenum << ": " << line << endl;	
			debug << setw(4) << linenum << ": " << line << endl;	
			line += ' ';
			pos = 0;
		}
	lexeme = "";
	int state = 0;
	while (true)
	{
		char c = line[pos++];
		lexeme += c;
		if (isalpha(c)) 
			c = 'a';
		else if (isdigit(c))
			c = '0';
		int col = 0;
		while (col < valid.length() && valid[col] != c)
			col++;
		state = table[state][col];
		switch (state)
		{
		    case 21: // identifier, int, or double 
			pos--;
			lexeme = lexeme.erase(lexeme.length()-1,1);
			if (lexeme == "int") return INTTYPE;
			if (lexeme == "double") return DBLTYPE;
			return IDENT;
		    case 22: // numeric literal
			pos--;
			lexeme = lexeme.erase(lexeme.length()-1,1);
			return NUMLIT;
		    case 24: // symbol with backup
			pos--;
			lexeme = lexeme.erase(lexeme.length()-1,1);
		    case 23: // symbol
			if (lexeme == "+") return PLUS;
			if (lexeme == "-") return MINUS;
			if (lexeme == "/") return DIV;
			if (lexeme == "*") return MULT;
			if (lexeme == "%") return MOD;
			if (lexeme == "=") return ASSIGN;
			if (lexeme == "<") return LT;
			if (lexeme == ">") return GT;
			if (lexeme == "!") return NOT;
			if (lexeme == "&") return AND;
			if (lexeme == "|") return OR;
			if (lexeme == "^") return XOR;
			if (lexeme == "~") return TILDA;
			if (lexeme == "(") return LPAREN;
			if (lexeme == ")") return RPAREN;
			if (lexeme == ";") return SEMI;
			if (lexeme == "?") return QUEST;
			if (lexeme == ":") return COLON;
			if (lexeme == ",") return COMMA;
			if (lexeme == "++") return PLUSPLUS;
			if (lexeme == "--") return MINUSMINUS;
			if (lexeme == "+=") return PLUSEQ;
			if (lexeme == "-=") return MINUSEQ;
			if (lexeme == "/=") return DIVEQ;
			if (lexeme == "*=") return MULTEQ;
			if (lexeme == "%=") return MODEQ;
			if (lexeme == "==") return EQUALTO;
			if (lexeme == "<=") return LTE;
			if (lexeme == ">=") return GTE;
			if (lexeme == "!=") return NOTEQ;
			if (lexeme == "&=") return ANDEQ;
			if (lexeme == "|=") return OREQ;
			if (lexeme == "^=") return XOREQ;
			if (lexeme == "&&") return LOGAND;
			if (lexeme == "||") return LOGOR;
			if (lexeme == "<<") return SHIFTL;
			if (lexeme == ">>") return SHIFTR;
			if (lexeme == "<<=") return SHIFTLEQ;
			if (lexeme == ">>=") return SHIFTREQ;
			return ERROR;
		    case 31: // error with backup
			pos--;
			lexeme = lexeme.erase(lexeme.length()-1,1);
		    case 32: // error
			error_message (string("Invalid character found: ") + lexeme);
			return ERROR;
		}
	}
}

string LexicalAnalyzer::get_token_name (token_type t) const
{
	return token_names [t];
}

string LexicalAnalyzer::get_lexeme () const
{
	return lexeme;
}

void LexicalAnalyzer::error_message (const string & msg)
{
	listing << "Error at " << linenum << ',' << pos << ": " << msg << endl;
	debug << "Error at " << linenum << ',' << pos << ": " << msg << endl;
	errors++;
}
