#include "test.h"

int foo(int a)
{
	static int i = 0;
	i++;
	return is_odd(a) + i;
}

int is_odd (int b)
{
	return (b) % 2;
}
