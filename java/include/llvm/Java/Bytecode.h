//===-- Bytecode.h - Java bytecode constants --------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file was developed by the LLVM research group and is distributed under
// the University of Illinois Open Source License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains Java bytecode constants.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_JAVA_BYTECODE_H
#define LLVM_JAVA_BYTECODE_H

#include <iosfwd>
#include <vector>

#include <stdint.h>

namespace llvm { namespace Java {

  enum JType {
    REFERENCE = 0, // this is not defined in the java spec
    BOOLEAN = 4,
    CHAR = 5,
    FLOAT = 6,
    DOUBLE = 7,
    BYTE = 8,
    SHORT = 9,
    INT = 10,
    LONG = 11,
  };

  namespace Opcode {

    enum {
      NOP = 0x00,
      ACONST_NULL = 0x01,
      ICONST_M1 = 0x02,
      ICONST_0 = 0x03,
      ICONST_1 = 0x04,
      ICONST_2 = 0x05,
      ICONST_3 = 0x06,
      ICONST_4 = 0x07,
      ICONST_5 = 0x08,
      LCONST_0 = 0x09,
      LCONST_1 = 0x0A,
      FCONST_0 = 0x0B,
      FCONST_1 = 0x0C,
      FCONST_2 = 0x0D,
      DCONST_0 = 0x0E,
      DCONST_1 = 0x0F,
      BIPUSH = 0x10,
      SIPUSH = 0x11,
      LDC = 0x12,
      LDC_W = 0x13,
      LDC2_W = 0x14,
      ILOAD = 0x15,
      LLOAD = 0x16,
      FLOAD = 0x17,
      DLOAD = 0x18,
      ALOAD = 0x19,
      ILOAD_0 = 0x1A,
      ILOAD_1 = 0x1B,
      ILOAD_2 = 0x1C,
      ILOAD_3 = 0x1D,
      LLOAD_0 = 0x1E,
      LLOAD_1 = 0x1F,
      LLOAD_2 = 0x20,
      LLOAD_3 = 0x21,
      FLOAD_0 = 0x22,
      FLOAD_1 = 0x23,
      FLOAD_2 = 0x24,
      FLOAD_3 = 0x25,
      DLOAD_0 = 0x26,
      DLOAD_1 = 0x27,
      DLOAD_2 = 0x28,
      DLOAD_3 = 0x29,
      ALOAD_0 = 0x2A,
      ALOAD_1 = 0x2B,
      ALOAD_2 = 0x2C,
      ALOAD_3 = 0x2D,
      IALOAD = 0x2E,
      LALOAD = 0x2F,
      FALOAD = 0x30,
      DALOAD = 0x31,
      AALOAD = 0x32,
      BALOAD = 0x33,
      CALOAD = 0x34,
      SALOAD = 0x35,
      ISTORE = 0x36,
      LSTORE = 0x37,
      FSTORE = 0x38,
      DSTORE = 0x39,
      ASTORE = 0x3A,
      ISTORE_0 = 0x3B,
      ISTORE_1 = 0x3C,
      ISTORE_2 = 0x3D,
      ISTORE_3 = 0x3E,
      LSTORE_0 = 0x3F,
      LSTORE_1 = 0x40,
      LSTORE_2 = 0x41,
      LSTORE_3 = 0x42,
      FSTORE_0 = 0x43,
      FSTORE_1 = 0x44,
      FSTORE_2 = 0x45,
      FSTORE_3 = 0x46,
      DSTORE_0 = 0x47,
      DSTORE_1 = 0x48,
      DSTORE_2 = 0x49,
      DSTORE_3 = 0x4A,
      ASTORE_0 = 0x4B,
      ASTORE_1 = 0x4C,
      ASTORE_2 = 0x4D,
      ASTORE_3 = 0x4E,
      IASTORE = 0x4F,
      LASTORE = 0x50,
      FASTORE = 0x51,
      DASTORE = 0x52,
      AASTORE = 0x53,
      BASTORE = 0x54,
      CASTORE = 0x55,
      SASTORE = 0x56,
      POP = 0x57,
      POP2 = 0x58,
      DUP = 0x59,
      DUP_X1 = 0x5A,
      DUP_X2 = 0x5B,
      DUP2 = 0x5C,
      DUP2_X1 = 0x5D,
      DUP2_X2 = 0x5E,
      SWAP = 0x5F,
      IADD = 0x60,
      LADD = 0x61,
      FADD = 0x62,
      DADD = 0x63,
      ISUB = 0x64,
      LSUB = 0x65,
      FSUB = 0x66,
      DSUB = 0x67,
      IMUL = 0x68,
      LMUL = 0x69,
      FMUL = 0x6A,
      DMUL = 0x6B,
      IDIV = 0x6C,
      LDIV = 0x6D,
      FDIV = 0x6E,
      DDIV = 0x6F,
      IREM = 0x70,
      LREM = 0x71,
      FREM = 0x72,
      DREM = 0x73,
      INEG = 0x74,
      LNEG = 0x75,
      FNEG = 0x76,
      DNEG = 0x77,
      ISHL = 0x78,
      LSHL = 0x79,
      ISHR = 0x7A,
      LSHR = 0x7B,
      IUSHR = 0x7C,
      LUSHR = 0x7D,
      IAND = 0x7E,
      LAND = 0x7F,
      IOR = 0x80,
      LOR = 0x81,
      IXOR = 0x82,
      LXOR = 0x83,
      IINC = 0x84,
      I2L = 0x85,
      I2F = 0x86,
      I2D = 0x87,
      L2I = 0x88,
      L2F = 0x89,
      L2D = 0x8A,
      F2I = 0x8B,
      F2L = 0x8C,
      F2D = 0x8D,
      D2I = 0x8E,
      D2L = 0x8F,
      D2F = 0x90,
      I2B = 0x91,
      I2C = 0x92,
      I2S = 0x93,
      LCMP = 0x94,
      FCMPL = 0x95,
      FCMPG = 0x96,
      DCMPL = 0x97,
      DCMPG = 0x98,
      IFEQ = 0x99,
      IFNE = 0x9A,
      IFLT = 0x9B,
      IFGE = 0x9C,
      IFGT = 0x9D,
      IFLE = 0x9E,
      IF_ICMPEQ = 0x9F,
      IF_ICMPNE = 0xA0,
      IF_ICMPLT = 0xA1,
      IF_ICMPGE = 0xA2,
      IF_ICMPGT = 0xA3,
      IF_ICMPLE = 0xA4,
      IF_IACMPEQ = 0xA5,
      IF_IACMPNE = 0xA6,
      GOTO = 0xA7,
      JSR = 0xA8,
      RET = 0xA9,
      TABLESWITCH = 0xAA,
      LOOKUPSWITCH = 0xAB,
      IRETURN = 0xAC,
      LRETURN = 0xAD,
      FRETURN = 0xAE,
      DRETURN = 0xAF,
      ARETURN = 0xB0,
      RETURN = 0xB1,
      GETSTATIC = 0xB2,
      PUTSTATIC = 0xB3,
      GETFIELD = 0xB4,
      PUTFIELD = 0xB5,
      INVOKEVIRTUAL = 0xB6,
      INVOKESPECIAL = 0xB7,
      INVOKESTATIC = 0xB8,
      INVOKEINTERFACE = 0xB9,
      XXXUNUSEDXXX = 0xBA,
      NEW = 0xBB,
      NEWARRAY = 0xBC,
      ANEWARRAY = 0xBD,
      ARRAYLENGTH = 0xBE,
      ATHROW = 0xBF,
      CHECKCAST = 0xC0,
      INSTANCEOF = 0xC1,
      MONITORENTER = 0xC2,
      MONITOREXIT = 0xC3,
      WIDE = 0xC4,
      MULTIANEWARRAY = 0xC5,
      IFNULL = 0xC6,
      IFNONNULL = 0xC7,
      GOTO_W = 0xC8,
      JSR_W = 0xC9,
      BREAKPOINT = 0xCA,
      IMPDEP1 = 0xFE,
      IMPDEP2 = 0xFF,
    };

  } // namespace Opcode

  inline int readSByte(const uint8_t* code, unsigned& i) {
    return ((int8_t*)code)[++i];
  }

  inline unsigned readUByte(const uint8_t* code, unsigned& i) {
    return code[++i];
  }

  inline int readSShort(const uint8_t* code, unsigned& i) {
    return (readSByte(code, i) << 8) | readUByte(code, i);
  }

  inline unsigned readUShort(const uint8_t* code, unsigned& i) {
    return (readUByte(code, i) << 8) | readUByte(code, i);
  }

  inline int readSInt(const uint8_t* code, unsigned& i) {
    return (readUShort(code, i) << 16) | readUShort(code, i);
  }

  inline unsigned readUInt(const uint8_t* code, unsigned& i) {
    return readSInt(code, i);
  }

  inline void skipPadBytes(unsigned& i) {
    unsigned lastTwoBits = i & 0x3;
    i += (4 - lastTwoBits) & 0x3;
    --i;
  }

} } // namespace llvm::Java

#endif//LLVM_JAVA_BYTECODE_H
