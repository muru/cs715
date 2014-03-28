#include <stdio.h>
int a, b, c, *e;
void p()
{
        printf ("%d", e);
}
int main()
{

        if (a == b)
                e = &c;   /* statement n1 */
        else
                e = &b;   /* statement n2 */
        e = &a;           /* statement n3 */
        p();
}
