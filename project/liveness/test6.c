#include<stdio.h>
int main()
{
int a,*b,r;
if(!r)
b=&a;
else
b=&r;
printf("%d",*b);
return 0;
}
