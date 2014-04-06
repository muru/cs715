#include <stdio.h>
/*
  Example from paper on Lazy Pointer Analysis
  Here points-to information differs for pta and lipta because of flow-sensitivity.
*/

int a, b, c, *e;
void p();

int main()
{
	if (a == b)
		e = &c;		/* statement n1 */	//LFCPA: e -> {c}
	else
		e = &b;		/* statement n2 */	//LFCPA: e -> {b}
							//LFCPA: e -> {c, b}
	e = &a;			/* statement n3 */	//LFCPA: e -> {a}
	p();
}

void p()
{
	printf ("%d", e);
}

//PTA: e -> {c, b, a} all the time

