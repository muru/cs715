#include<stdio.h>
struct node
{
	struct node *n;
	int data;
};

int main()
{
	int *z, w;
	struct node p, q, r;
	struct node *x;
	x = &p;
	x->n = &q;
	x->n->n = &r;
	z = &w;
	int a;

	while(a--)
		x = x->n;
	z = &(x->data);
	printf("%d", z);
	return x->data;
}
