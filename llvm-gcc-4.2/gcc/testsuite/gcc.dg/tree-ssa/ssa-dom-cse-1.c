/* { dg-do compile } */ 
/* { dg-options "-O2 -fdump-tree-fre-details" } */
/* LLVM LOCAL test not applicable */
/* { dg-require-fdump "" } */
int t(int a) __attribute__ ((const));
void q (void);
void
threading(int a,int b)
{
	if (t(a))
	{
	  if (t(a))
	    q();
	}
}
/* We should thread the jump twice and eliminate it.  */
/* { dg-final { scan-tree-dump-times "Replaced.* t " 1 "fre"} } */
/* { dg-final { cleanup-tree-dump "fre" } } */
