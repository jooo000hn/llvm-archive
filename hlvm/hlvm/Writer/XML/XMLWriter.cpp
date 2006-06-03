//===-- hlvm/Writer/XML/XMLWriter.cpp - AST XML Writer Class ----*- C++ -*-===//
//
//                      High Level Virtual Machine (HLVM)
//
// Copyright (C) 2006 Reid Spencer. All Rights Reserved.
//
// This software is free software; you can redistribute it and/or modify it 
// under the terms of the GNU Lesser General Public License as published by 
// the Free Software Foundation; either version 2.1 of the License, or (at 
// your option) any later version.
//
// This software is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for 
// more details.
//
// You should have received a copy of the GNU Lesser General Public License 
// along with this library in the file named LICENSE.txt; if not, write to the 
// Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, 
// MA 02110-1301 USA
//
//===----------------------------------------------------------------------===//
/// @file hlvm/Writer/XML/XMLWriter.cpp
/// @author Reid Spencer <rspencer@x10sys.com>
/// @date 2006/05/12
/// @since 0.1.0
/// @brief Provides the interface to hlvm::XMLWriter
//===----------------------------------------------------------------------===//

#include <hlvm/Writer/XML/XMLWriter.h>
#include <hlvm/AST/AST.h>
#include <hlvm/AST/Bundle.h>
#include <hlvm/AST/Import.h>
#include <hlvm/AST/Documentation.h>
#include <hlvm/AST/ContainerType.h>
#include <hlvm/AST/Variable.h>
#include <hlvm/AST/Program.h>
#include <hlvm/AST/Block.h>
#include <hlvm/AST/ControlFlow.h>
#include <hlvm/AST/Constants.h>
#include <hlvm/Base/Assert.h>
#include <hlvm/Pass/Pass.h>
#include <llvm/ADT/StringExtras.h>
#include <libxml/xmlwriter.h>
#include <iostream>

using namespace hlvm;
using namespace llvm;

namespace {

class XMLWriterImpl : public XMLWriter {
public:
  XMLWriterImpl(const char* fname) : pass(fname) { }
  virtual ~XMLWriterImpl() { }
  virtual void write(AST* node);

private:
  class WriterPass : public Pass
  {
    public:
      WriterPass(const char* fname) : Pass(0,Pass::PreAndPostOrderTraversal) {
        writer = xmlNewTextWriterFilename(fname,0);
        hlvmAssert(writer && "Can't allocate writer");
        xmlTextWriterSetIndent(writer,1);
        xmlTextWriterSetIndentString(writer,
            reinterpret_cast<const xmlChar*>("  "));
      }
      ~WriterPass() {
        xmlFreeTextWriter(writer);
      }
    inline void writeComment(const char* cmt)
      { xmlTextWriterWriteComment(writer,
          reinterpret_cast<const xmlChar*>(cmt)); }
    inline void startElement(const char* elem) 
      { xmlTextWriterStartElement(writer, 
          reinterpret_cast<const xmlChar*>(elem)); }
    inline void endElement() 
      { xmlTextWriterEndElement(writer); }
    inline void writeAttribute(const char*name, const char*val)
      { xmlTextWriterWriteAttribute(writer, 
          reinterpret_cast<const xmlChar*>(name), 
          reinterpret_cast<const xmlChar*>(val)); }
    inline void writeAttribute(const char* name, const std::string& val) 
      { writeAttribute(name, val.c_str()); }
    inline void writeAttribute(const char* name, const Type* t)
      { writeAttribute(name, t->getName()); }
    inline void writeAttribute(const char* name, uint64_t val)
      { writeAttribute(name, llvm::utostr(val)); }
    inline void writeElement(const char* elem, const char* body)
      { xmlTextWriterWriteElement(writer,
          reinterpret_cast<const xmlChar*>(elem),
          reinterpret_cast<const xmlChar*>(body)); }
    inline void writeString(const std::string& text)
      { xmlTextWriterWriteString(writer,
          reinterpret_cast<const xmlChar*>(text.c_str())); }

    inline void putHeader();
    inline void putFooter();
    inline void putDoc(Documentable* node);
    inline void put(Bundle* b);
    inline void put(Documentation* b);
    inline void put(AliasType* t);
    inline void put(AnyType* t);
    inline void put(BooleanType* t);
    inline void put(CharacterType* t);
    inline void put(IntegerType* t);
    inline void put(RangeType* t);
    inline void put(EnumerationType* t);
    inline void put(RealType* t);
    inline void put(OctetType* t);
    inline void put(VoidType* t);
    inline void put(PointerType* t);
    inline void put(ArrayType* t);
    inline void put(VectorType* t);
    inline void put(StructureType* t);
    inline void put(SignatureType* t);
    inline void put(ConstLiteralInteger* t);
    inline void put(Variable* v);
    inline void put(Function* f);
    inline void put(Program* p);
    inline void put(Block* f);
    inline void put(ReturnOp* f);

    virtual void handle(Node* n,Pass::TraversalKinds mode);

  private:
    xmlTextWriterPtr writer;
    AST* node;
    friend class XMLWriterImpl;
  };
private:
  WriterPass pass;
};

inline const char* 
getLinkageKind(LinkageKinds lk)
{
  switch (lk) {
    case WeakLinkage:        return "weak";
    case ExternalLinkage:    return "external";
    case InternalLinkage:    return "internal";
    case LinkOnceLinkage:    return "linkonce";
    case AppendingLinkage:   return "appending";
    default:
      hlvmDeadCode("Bad LinkageKinds");
      break;
  }
  return "error";
}

inline void
XMLWriterImpl::WriterPass::putDoc(Documentable* node)
{
  Documentation* theDoc = node->getDoc();
  if (theDoc) {
    this->put(theDoc);
  }
}

void
XMLWriterImpl::WriterPass::putHeader() 
{
  xmlTextWriterStartDocument(writer,0,"UTF-8",0);
  startElement("hlvm");
  writeAttribute("xmlns","http://hlvm.org/src/hlvm/Reader/XML/HLVM.rng");
  writeAttribute("pubid",node->getPublicID());
}

void
XMLWriterImpl::WriterPass::putFooter()
{
  endElement();
  xmlTextWriterEndDocument(writer);
}
void 
XMLWriterImpl::WriterPass::put(Documentation* b)
{
  startElement("doc");
  const std::string& data = b->getDoc();
  xmlTextWriterWriteRawLen(writer,
    reinterpret_cast<const xmlChar*>(data.c_str()),data.length());
  endElement();
}

void 
XMLWriterImpl::WriterPass::put(AliasType* t)
{
  startElement("alias");
  writeAttribute("id",t->getName());
  writeAttribute("renames",t->getType());
  putDoc(t);
}
void 
XMLWriterImpl::WriterPass::put(AnyType* t)
{
  startElement("atom");
  writeAttribute("id",t->getName());
  putDoc(t);
  startElement("intrinsic");
  writeAttribute("is","any");
  endElement();
}

void
XMLWriterImpl::WriterPass::put(BooleanType* t)
{
  startElement("atom");
  writeAttribute("id",t->getName());
  putDoc(t);
  startElement("intrinsic");
  writeAttribute("is","bool");
  endElement();
}

void
XMLWriterImpl::WriterPass::put(CharacterType* t)
{
  startElement("atom");
  writeAttribute("id",t->getName());
  putDoc(t);
  startElement("intrinsic");
  writeAttribute("is","char");
  endElement();
}

void
XMLWriterImpl::WriterPass::put(IntegerType* t)
{
  startElement("atom");
  writeAttribute("id",t->getName());
  putDoc(t);
  const char* primName = t->getPrimitiveName();
  if (primName) {
    startElement("intrinsic");
    writeAttribute("is",primName);
  } else if (t->isSigned()) {
    startElement("signed");
    writeAttribute("bits", llvm::utostr(t->getBits()));
  } else {
    startElement("unsigned");
    writeAttribute("bits", llvm::utostr(t->getBits()));
  }
  endElement();
}

void
XMLWriterImpl::WriterPass::put(RangeType* t)
{
  startElement("range");
  writeAttribute("id",t->getName());
  writeAttribute("min",t->getMin());
  writeAttribute("max",t->getMax());
  putDoc(t);
}

void 
XMLWriterImpl::WriterPass::put(EnumerationType* t)
{
  startElement("enumeration");
  writeAttribute("id",t->getName());
  putDoc(t);
  for (EnumerationType::const_iterator I = t->begin(), E = t->end(); 
       I != E; ++I)
  {
    startElement("enumerator");
    writeAttribute("id",*I);
    endElement();
  }
}

void
XMLWriterImpl::WriterPass::put(RealType* t)
{
  startElement("atom");
  writeAttribute("id",t->getName());
  putDoc(t);
  const char* primName = t->getPrimitiveName();
  if (primName) {
    startElement("intrinsic");
    writeAttribute("is",primName);
    endElement();
  } else {
    startElement("real");
    writeAttribute("mantissa", llvm::utostr(t->getMantissa()));
    writeAttribute("exponent", llvm::utostr(t->getExponent()));
    endElement();
  }
}

void
XMLWriterImpl::WriterPass::put(OctetType* t)
{
  startElement("atom");
  writeAttribute("id",t->getName());
  putDoc(t);
  startElement("intrinsic");
  writeAttribute("is","octet");
  endElement();
}

void
XMLWriterImpl::WriterPass::put(VoidType* t)
{
  startElement("atom");
  writeAttribute("id",t->getName());
  putDoc(t);
  startElement("intrinsic");
  writeAttribute("is","void");
  endElement();
}

void 
XMLWriterImpl::WriterPass::put(PointerType* t)
{
  startElement("pointer");
  writeAttribute("id", t->getName());
  writeAttribute("to", t->getTargetType());
  putDoc(t);
}

void 
XMLWriterImpl::WriterPass::put(ArrayType* t)
{
  startElement("array");
  writeAttribute("id", t->getName());
  writeAttribute("of", t->getElementType());
  writeAttribute("length", t->getMaxSize());
  putDoc(t);
}

void 
XMLWriterImpl::WriterPass::put(VectorType* t)
{
  startElement("vector");
  writeAttribute("id", t->getName());
  writeAttribute("of", t->getElementType());
  writeAttribute("length", t->getSize());
  putDoc(t);
}

void 
XMLWriterImpl::WriterPass::put(StructureType* t)
{
  startElement("structure");
  writeAttribute("id",t->getName());
  putDoc(t);
  for (StructureType::iterator I = t->begin(), E = t->end(); I != E; ++I) {
    startElement("field");
    AliasType* alias = cast<AliasType>(*I);
    writeAttribute("id",alias->getName());
    writeAttribute("type",alias->getType());
    putDoc(alias);
    endElement();
  }
}

void 
XMLWriterImpl::WriterPass::put(SignatureType* t)
{
  startElement("signature");
  writeAttribute("id",t->getName());
  writeAttribute("result",t->getResultType());
  writeAttribute("varargs",t->isVarArgs() ? "true" : "false");
  putDoc(t);
  for (SignatureType::iterator I = t->begin(), E = t->end(); I != E; ++I) {
    startElement("arg");
    AliasType* alias = cast<AliasType>(*I);
    writeAttribute("id",alias->getName());
    writeAttribute("type",alias->getType());
    putDoc(alias);
    endElement();
  }
}

void 
XMLWriterImpl::WriterPass::put(ConstLiteralInteger* i)
{
  startElement("dec");
  if (cast<IntegerType>(i->getType())->isSigned())
    writeString(llvm::itostr(i->getValue()));
  else
    writeString(llvm::utostr(i->getValue(0)));
}

void
XMLWriterImpl::WriterPass::put(Variable* v)
{
  startElement("variable");
  writeAttribute("id",v->getName());
  writeAttribute("type",v->getType()->getName());
  putDoc(v);
}

void
XMLWriterImpl::WriterPass::put(Function* f)
{
  startElement("function");
  writeAttribute("id",f->getName());
  writeAttribute("type",f->getSignature()->getName());
  writeAttribute("linkage",getLinkageKind(f->getLinkageKind()));
  putDoc(f);
}

void 
XMLWriterImpl::WriterPass::put(Program* p)
{
  startElement("program");
  writeAttribute("id",p->getName());
  putDoc(p);
}

void 
XMLWriterImpl::WriterPass::put(Block* b)
{
  startElement("block");
  if (!b->getLabel().empty())
    writeAttribute("label",b->getLabel());
  putDoc(b);
}

void 
XMLWriterImpl::WriterPass::put(ReturnOp* r)
{
  startElement("ret");
  putDoc(r);
}

void 
XMLWriterImpl::WriterPass::put(Bundle* b)
{
  startElement("bundle");
  writeAttribute("id",b->getName());
  putDoc(b);
}

void
XMLWriterImpl::WriterPass::handle(Node* n,Pass::TraversalKinds mode)
{
  if (mode & Pass::PreOrderTraversal) {
    switch (n->getID()) 
    {
      case AliasTypeID:               put(cast<AliasType>(n)); break;
      case AnyTypeID:                 put(cast<AnyType>(n)); break;
      case BooleanTypeID:             put(cast<BooleanType>(n)); break;
      case BundleID:                  put(cast<Bundle>(n)); break;
      case CharacterTypeID:           put(cast<CharacterType>(n)); break;
      case IntegerTypeID:             put(cast<IntegerType>(n)); break;
      case RangeTypeID:               put(cast<RangeType>(n)); break;
      case EnumerationTypeID:         put(cast<EnumerationType>(n)); break;
      case RealTypeID:                put(cast<RealType>(n)); break;
      case OctetTypeID:               put(cast<OctetType>(n)); break;
      case VoidTypeID:                put(cast<VoidType>(n)); break;
      case PointerTypeID:             put(cast<PointerType>(n)); break;
      case ArrayTypeID:               put(cast<ArrayType>(n)); break;
      case VectorTypeID:              put(cast<VectorType>(n)); break;
      case StructureTypeID:           put(cast<StructureType>(n)); break;
      case SignatureTypeID:           put(cast<SignatureType>(n)); break;
      case ConstLiteralIntegerOpID:   put(cast<ConstLiteralInteger>(n)); break;
      case VariableID:                put(cast<Variable>(n)); break;
      case FunctionID:                put(cast<Function>(n)); break;
      case ProgramID:                 put(cast<Program>(n)); break;
      case BlockID:                   put(cast<Block>(n)); break;
      case ReturnOpID:                put(cast<ReturnOp>(n)); break;
      default:
        hlvmDeadCode("Unknown Type");
        break;
    }
  }
  if (mode & Pass::PostOrderTraversal) {
    endElement();
  }
}

void
XMLWriterImpl::write(AST* ast) 
{
  pass.node = ast;
  pass.putHeader();
  PassManager* PM = PassManager::create();
  PM->addPass(&pass);
  PM->runOn(ast);
  pass.putFooter();
}

}

XMLWriter* 
hlvm::XMLWriter::create(const char* fname)
{
  return new XMLWriterImpl(fname);
}
