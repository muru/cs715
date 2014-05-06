#include<stdio.h>
int main()
{
	int *p, a;

	if (a)
		p = &a;

	printf("%u %u", p, &p);		//PTA: p → { a }
					//LFCPA: p → {}, as 'a' is not live
	return 0;
}

