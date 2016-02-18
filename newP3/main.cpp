#include <iostream>
#include "SetLimits.h"

#include "syntactic.h"

using namespace std;

int main (int argc, char * argv[])
{
    SetLimits();

    int t;
    if (argc < 2)
    {
        cout << "format: proj1 <filename>\n";
        exit (1);
    }

    syntactic syntactic (argv[1]);

    syntactic.program();

    return 0;
}
