int main() {
	int *p1, *p2, *p3, a, b, c, d;

	if (c)
		p2 = &a;
	else
		p2 = &b;		//p2 -> {a, b}

	if (a)
		p3 = &c;
	else
		p3 = &d;		//p3 -> {c, d}

	p1 = p2 + 1;			//PTA: p1_9 -> {a, b}
					//LIPTA: p1 -> {a, b}
	printf("%d %u",*p1, p1);

	p1 = p3 + 1;			//PTA: p1_11 -> {c, d}
					//LIPTA: p1 -> {c, d}, but fortunately in a different bb (of LIPTA)
	printf("%d %u",*p1, p1);

	return 0;
}
