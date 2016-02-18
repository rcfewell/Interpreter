#include <stdio.h>

int main ()
{
int a = 5;
double b = 3.2;
a++;
++a;
b++;
++b;
b = b << a;
printf ("a = %d\n", a);
printf ("b = %f\n", b);
}
