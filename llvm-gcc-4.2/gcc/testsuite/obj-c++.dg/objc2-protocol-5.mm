/* APPLE LOCAL file 4695109 - modified for radar 6255913 */
/* { dg-options "-mmacosx-version-min=10.5 -fobjc-abi-version=2" { target powerpc*-*-darwin* i?86*-*-darwin* } } */
/* { dg-do compile { target *-*-darwin* } } */

@protocol Proto1
+classMethod;
@end

@protocol Proto2
+classMethod2;
@end

int main() {
	return (long) @protocol(Proto1);
}
/* LLVM LOCAL llvm syntax */
/* { dg-final { scan-assembler "l_.*OBJC_PROTOCOL_\\\$_Proto1:" } } */
