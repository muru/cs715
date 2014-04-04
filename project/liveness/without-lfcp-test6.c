num_ssa_name=13


points-to information

b_1, points-to vars: { a r }
-----------------------------------------------------

b_1 = PHI <&a(2), &r(3)>
b_1 : --> single use.
D.2746_5 = *b_1;


PTA query stats:
  pt_solution_includes: 0 disambiguations, 0 queries
  pt_solutions_intersect: 0 disambiguations, 0 queries
main ()
{
  int r;
  int * b;
  int a;
  int D.2746;
  int r.0;

<bb 2>:
  r.0_2 = r;
  if (r.0_2 == 0)
    goto <bb 4>;
  else
    goto <bb 3>;

<bb 3>:

<bb 4>:
  # b_1 = PHI <&a(2), &r(3)>
  D.2746_5 = *b_1;
  printf ("%d", D.2746_5);
  a ={v} {CLOBBER};
  r ={v} {CLOBBER};
  return 0;

}


