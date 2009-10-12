/* APPLE LOCAL file v7 merge */
/* Test the `vst2s16' ARM Neon intrinsic.  */
/* This file was autogenerated by neon-testgen.  */

/* { dg-do assemble } */
/* { dg-require-effective-target arm_neon_ok } */
/* { dg-options "-save-temps -O0 -mfpu=neon -mfloat-abi=softfp" } */

#include "arm_neon.h"

void test_vst2s16 (void)
{
  int16_t *arg0_int16_t;
  int16x4x2_t arg1_int16x4x2_t;

  vst2_s16 (arg0_int16_t, arg1_int16x4x2_t);
}

/* { dg-final { scan-assembler "vst2\.16\[ 	\]+\\\{((\[dD\]\[0-9\]+-\[dD\]\[0-9\]+)|(\[dD\]\[0-9\]+, \[dD\]\[0-9\]+))\\\}, \\\[\[rR\]\[0-9\]+\\\]!?\(\[ 	\]+@\[a-zA-Z0-9 \]+\)?\n" } } */
/* { dg-final { cleanup-saved-temps } } */
