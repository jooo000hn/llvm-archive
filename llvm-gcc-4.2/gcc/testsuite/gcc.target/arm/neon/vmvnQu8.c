/* APPLE LOCAL file v7 merge */
/* Test the `vmvnQu8' ARM Neon intrinsic.  */
/* This file was autogenerated by neon-testgen.  */

/* { dg-do assemble } */
/* { dg-require-effective-target arm_neon_ok } */
/* { dg-options "-save-temps -O0 -mfpu=neon -mfloat-abi=softfp" } */

#include "arm_neon.h"

void test_vmvnQu8 (void)
{
  uint8x16_t out_uint8x16_t;
  uint8x16_t arg0_uint8x16_t;

  out_uint8x16_t = vmvnq_u8 (arg0_uint8x16_t);
}

/* { dg-final { scan-assembler "vmvn\[ 	\]+\[qQ\]\[0-9\]+, \[qQ\]\[0-9\]+!?\(\[ 	\]+@\[a-zA-Z0-9 \]+\)?\n" } } */
/* { dg-final { cleanup-saved-temps } } */
