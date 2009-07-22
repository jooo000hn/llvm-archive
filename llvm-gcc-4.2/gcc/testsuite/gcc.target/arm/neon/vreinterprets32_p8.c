/* APPLE LOCAL file v7 merge */
/* Test the `vreinterprets32_p8' ARM Neon intrinsic.  */
/* This file was autogenerated by neon-testgen.  */

/* { dg-do assemble } */
/* { dg-require-effective-target arm_neon_ok } */
/* { dg-options "-save-temps -O0 -mfpu=neon -mfloat-abi=softfp" } */

#include "arm_neon.h"

void test_vreinterprets32_p8 (void)
{
  int32x2_t out_int32x2_t;
  poly8x8_t arg0_poly8x8_t;

  out_int32x2_t = vreinterpret_s32_p8 (arg0_poly8x8_t);
}

/* { dg-final { cleanup-saved-temps } } */
