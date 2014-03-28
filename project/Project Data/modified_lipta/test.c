void f(int *b, int *c)
{
	int *t = b;
	*b    = *c;
	*c    = *t;
}


int main()
{
	int a  = 10;
	int c  = 20;
	int *b = &a;
	int *d = &c;
	f(b, d);
	return 0;
}
