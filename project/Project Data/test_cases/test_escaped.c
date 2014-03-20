int *fn();

int main()
{
	int *p, a, b, c, d;

	if (c)
		p = &a;
	printf("%u %u",p, &p);

	p = fn();

	printf("%u %u",p, &p);

	return 0;
}
