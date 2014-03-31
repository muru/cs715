#include <stdio.h>
int a, b, c, *e;
void p()
{
	printf ("%d", e);
}
int main()
{
	scanf("%d %d %d", &a, &b, &c);

	if (a == b)
		e = &c;   /* statement n1 */
	else
		e = &b;   /* statement n2 */

	if (c - 5)
		a = b;
	else if (c)
		a = b - 5;
	else
		a = b + 5;

	e = &a;           /* statement n3 */
	p();
}
