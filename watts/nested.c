#include <stdio.h>

int main ()
{
int a = 3;
int b = 12;
int c = 2 * (a + b);
int d = c *(a - b) / (a + b);
int e = (c - (a + b) * 0.5);
printf ("a = %d\n", a);
printf ("b = %d\n", b);
printf ("c = %d\n", c);
printf ("d = %d\n", d);
printf ("e = %d\n", e);
return 0;
}
