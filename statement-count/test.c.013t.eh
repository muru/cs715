
;; Function int main() (main, funcdef_no=2, decl_uid=2704, cgraph_uid=2)

int main() ()
{
  int i;
  int a;
  int j;
  int n;
  int D.2721;
  int D.2717;
  bool retval.0;
  bool D.2714;

  n = 10;
  j = 3;
  i = 0;
  goto <D.2715>;
  <D.2713>:
  j = foo (j);
  D.2717 = is_odd (j);
  retval.0 = D.2717 != 0;
  if (retval.0 != 0) goto <D.2718>; else goto <D.2719>;
  <D.2718>:
  a = j;
  goto <D.2720>;
  <D.2719>:
  a = j % 3;
  <D.2720>:
  i = i + 1;
  <D.2715>:
  D.2714 = i < n;
  if (D.2714 != 0) goto <D.2713>; else goto <D.2711>;
  <D.2711>:
  D.2721 = a;
  goto <D.2722>;
  D.2721 = 0;
  goto <D.2722>;
  <D.2722>:
  return D.2721;
}



;; Function int is_odd(int) (_Z6is_oddi, funcdef_no=1, decl_uid=2702, cgraph_uid=1)

int is_odd(int) (int b)
{
  int D.2723;

  D.2723 = b % 2;
  goto <D.2724>;
  <D.2724>:
  return D.2723;
}



;; Function int foo(int) (_Z3fooi, funcdef_no=0, decl_uid=2698, cgraph_uid=0)

int foo(int) (int a)
{
  static int i = 0;
  int i.3;
  int D.2727;
  int i.2;
  int i.1;

  i.1 = i;
  i.2 = i.1 + 1;
  i = i.2;
  i.3 = i;
  D.2727 = a + i.3;
  goto <D.2729>;
  <D.2729>:
  return D.2727;
}


