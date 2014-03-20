//int M, N;
#include<stdio.h>
void f(int* a, int* b)
{
	int M, N;
	scanf("%d %d", &M, &N);
	int *c = &M, *d= &N;
	if(M < N) {
		a = c;
		b = d;
	}
	if(M == 0) {
		f(b, a);
	}
}

int main()
{
	int *a, *b;
	f(a, b);
	return *a + *b;
}
