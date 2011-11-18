/* APPLE LOCAL file radar 6217257 */
/* __block cannot be used on dynamic array declaration due to block's runtime
   limitations. This test detects and flags as error such cases. */
/* { dg-options "-mmacosx-version-min=10.6 -ObjC" { target *-*-darwin* } } */
/* { dg-do compile } */

int size=5;

int main() {
  __block int array[size];	/* { dg-error "__block attribute not allowed on declaration with a variably" } */

  __block id aid1[size];	/* { dg-error "__block attribute not allowed on declaration with a variably" } */

 __block id aid2[2][size];	/* { dg-error "__block attribute not allowed on declaration with a variably" } */

 __block id aid3[][size];	/* { dg-error "__block attribute not allowed on declaration with a variably" } */
				/* { dg-error "needs an explicit size" "" { target *-*-* } 16 } */


 __block id oid1 [1];
 __block id oid2 [2][3];
 __block id oid4 [][1];	/* { dg-error "needs an explicit size" } */
 return 0;
}

