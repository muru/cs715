#include <stdio.h>
int *p, a, b;

void fn3 () {
	printf("%d", *p);	//PTA: p -> {a, b}
				//LFCPA: If called from fn1: p -> {a}
				//			fn2: p -> {b}
}

void fn2 () {
	p = &b;
	fn3();
	printf("%d", *p);	//PTA: p -> {a, b}
				//LFCPA: p -> {b}
}

void fn1 () {
	p = &a;
	fn3();
	printf("%d", *p);	//PTA: p -> {a, b}
				//LFCPA: p -> {a}
}


int main() {
	fn1();

	fn2();

	return 0;
}

