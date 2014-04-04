#include <stdio.h>
int *a, *b, c, *e;
void p()
{
	printf ("%d", e);
}
int main()
{
	//scanf("%d %d %d", &a, &b, &c);
	int i = 0, j  = 1, k[13];
	a     = &i; b = k + 6;
	if (a == b)
		e = &c;   /* statement n1 */
	else
		e = k + 7;   /* statement n2 */

	if (c - 5)
		a = b;
	else if (c)
		a = &j;
	else
		a = e;

	e = a;           /* statement n3 */
	p();
}
