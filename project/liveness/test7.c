#include <stdio.h>
typedef struct node
{
	int v;
	struct node * n;
	struct node * p;
} list_node;

int f(int i, int j, int k)
{
	list_node a, b, c, *q;
	q = &a;
	a.v = 1;
	a.n = &b;
	a.p = &c;
	b.v = 2;
	b.n = &c;
	b.p = &a;
	c.v = 3;
	c.n = NULL;
	c.p = &b;
	while (q->v < 4)
	{
		q = q->n;
	}
	return q->v;
}

int main()
{
	int i,j,k;
	scanf ("%d %d %d", &i, &j, &k);
	return f(i, j, k);
}
