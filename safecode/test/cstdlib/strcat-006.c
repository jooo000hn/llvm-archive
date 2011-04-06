/*
 * RUN: sh test.sh %s
 * XFAIL: *
 */

/* strcat() onto the end of an object that is not nul terminated,
 * but the front of the object is. */

#include <string.h>
#include <stdio.h>

int main()
{
  char dst[100] = "";
  memset(&dst[1], 'a', 99);
  strcat(&dst[1], "string");
  return 0;
}