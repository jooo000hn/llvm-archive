/* Copyright (C) 2001 Free Software Foundation, Inc.  */

/* { dg-do compile } */
/* { dg-options -Wparentheses } */

/* Source: Neil Booth, 1 Nov 2001.  PR 3170, 3422 - bogus warnings
   about suggesting parentheses.  */

int foo (int a, int b)
{
  int c = (a && b) || 0;	
  c = a && b || 0;

  return (a && b && 1) || 0;
}
