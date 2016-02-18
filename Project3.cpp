#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <vector>
#include "SetLimits.h"
#include "Lex.h"
#include "Syn.h"
#include "Interpreter.h"
#include "Symbol.h"

int main (int argc, char * argv[])
{
	if (argc < 2)
	{
		printf ("format: proj1 <filename>\n");
		exit (1);
	}
	SetLimits ();

	Syn syn( argv[1] );

	return 0;
}
 
