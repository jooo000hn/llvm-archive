/* Test that compiling for the GNU runtime works (regardless of
   the system runtime used).  */
/* Author: Ziemowit Laski <zlaski@apple.com>  */
/* { dg-do compile } */
/* { dg-options "-fgnu-runtime" } */
/* APPLE LOCAL radar 4895746 */
/* { dg-skip-if "" { *-*-darwin* } { "-m64" } { "" } } */

#include <objc/Object.h>

@interface FooBar: Object
- (void)boo;
@end

int main ()
{
  id fooBarInst = [[FooBar alloc] init];
  [fooBarInst boo];
  return 0;
}

