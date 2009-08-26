/* APPLE LOCAL file v7 merge */
/* Test the `vcltQs32' ARM Neon intrinsic.  */
/* This file was autogenerated by neon-testgen.  */

/* { dg-do assemble } */
/* { dg-require-effective-target arm_neon_ok } */
/* { dg-options "-save-temps -O0 -mfpu=neon -mfloat-abi=softfp" } */

#include "arm_neon.h"

void test_vcltQs32 (void)
{
  uint32x4_t out_uint32x4_t;
  int32x4_t arg0_int32x4_t;
  int32x4_t arg1_int32x4_t;

  out_uint32x4_t = vcltq_s32 (arg0_int32x4_t, arg1_int32x4_t);
}

/* { dg-final { scan-assembler "vcgt\.s32\[ 	\]+\[qQ\]\[0-9\]+, \[qQ\]\[0-9\]+, \[qQ\]\[0-9\]+!?\(\[ 	\]+@\[a-zA-Z0-9 \]+\)?\n" } } */
/* { dg-final { cleanup-saved-temps } } */
