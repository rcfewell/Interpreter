#include <iostream>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstdlib>
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
