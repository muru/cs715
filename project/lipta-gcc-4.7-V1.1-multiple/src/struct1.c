#include<stdio.h>
struct node
{
	int d;
	struct node *n;
};

int main()
{
	struct node x,z;
	x = z;
	struct node *y, **Y;
	struct node t1, t2, t3, t4;
	y = z.n;
	y = &t1;
	*y = t2;
	*Y = y;
	y->n = &t2;
	y->n->n = &t3;
	int a;
	while(a--)
	{
		y = y->n;
		y->n->n->n = &t4;
	}
	return y->n->d;
}	
