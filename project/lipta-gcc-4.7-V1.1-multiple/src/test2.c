#include <stdio.h>
void funct(int *a, int*b)
{
	int *temp;
	temp = a;
	a = b;	
	b = temp;
}
/*nt main ()
{
               int *p, *q, s, t;
                 q = &s;
             p = &t;
                 while(*q > 0)
               *q--;
             p = q;
                 printf ("%d%d%d%d", p, q, s);
         } */
