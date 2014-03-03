#include "test.h"

int foo(int a)
{
	static int i = 0;
	i++;
	return (a) + i;
}

int is_odd (int b)
{
	return foo(b) % 2;
}
