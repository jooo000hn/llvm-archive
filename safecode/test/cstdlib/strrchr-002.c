/*
 * RUN: sh test.sh %s
 * XFAIL: *
 */

/* strrchr() on an unterminated string, with the character being searched
 * for not found in the string. */

#include <stdio.h>
#include <string.h>

int main()
{
  char a[1000];
  memset(a, 'a', 1000);
  printf("%p\n", strrchr(a, 'b'));
  return 0;
}