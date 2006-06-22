//===-- AST SymbolTable Class -----------------------------------*- C++ -*-===//
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
/// @file hlvm/AST/SymbolTable.cpp
/// @author Reid Spencer <reid@hlvm.org> (original author)
/// @date 2006/05/04
/// @since 0.1.0
/// @brief Implements the functions of class hlvm::AST::SymbolTable.
//===----------------------------------------------------------------------===//

#include "hlvm/AST/SymbolTable.h"
#include "hlvm/AST/Type.h"
#include "hlvm/AST/Linkables.h"
#include "hlvm/Base/Assert.h"
#include "llvm/ADT/StringExtras.h"
#include <iostream>
#include <algorithm>

namespace hlvm {

template<class ElemType>
std::string 
SymbolTable<ElemType>::getUniqueName(const std::string &base_name) const {
  std::string try_name = base_name;
  const_iterator end = map_.end();

  // See if the name exists. Loop until we find a free name in the symbol table
  // by incrementing the last_unique_ counter.
  while (map_.find(try_name) != end)
    try_name = base_name + 
      llvm::utostr(++last_unique_);
  return try_name;
}

// lookup a node by name - returns null on failure
template<class ElemType>
ElemType* SymbolTable<ElemType>::lookup(const std::string& name) const {
  const_iterator TI = map_.find(name);
  if (TI != map_.end())
    return const_cast<ElemType*>(TI->second);
  return 0;
}

// Erase a specific type from the symbol table
template<class ElemType>
bool SymbolTable<ElemType>::erase(ElemType *N) {
  for (iterator TI = map_.begin(), TE = map_.end(); TI != TE; ++TI) {
    if (TI->second == N) {
      this->erase(TI);
      return true;
    }
  }
  return false;
}

// remove - Remove a node from the symbol table...
template<class ElemType>
ElemType* SymbolTable<ElemType>::erase(iterator Entry) {
  hlvmAssert(Entry != map_.end() && "Invalid entry to remove!");
  const ElemType* Result = Entry->second;
  map_.erase(Entry);
  return const_cast<ElemType*>(Result);
}

// insert - Insert a node into the symbol table with the specified name...
template<class ElemType>
void SymbolTable<ElemType>::insert(const std::string& Name, const ElemType* N) {
  hlvmAssert(N && "Can't insert null node into symbol table!");

  // Check to see if there is a naming conflict.  If so, rename this type!
  std::string unique_name = Name;
  if (lookup(Name))
    unique_name = getUniqueName(Name);

  // Insert the map entry
  map_.insert(make_pair(unique_name, N));
}

/// rename - Given a value with a non-empty name, remove its existing entry
/// from the symbol table and insert a new one for Name.  This is equivalent to
/// doing "remove(V), V->Name = Name, insert(V)", but is faster, and will not
/// temporarily remove the symbol table plane if V is the last value in the
/// symtab with that name (which could invalidate iterators to that plane).
template<class ElemType>
bool SymbolTable<ElemType>::rename(ElemType *N, const std::string &name) {
  for (iterator NI = map_.begin(), NE = map_.end(); NI != NE; ++NI) {
    if (NI->second == N) {
      // Remove the old entry.
      map_.erase(NI);
      // Add the new entry.
      this->insert(name,N);
      return true;
    }
  }
  return false;
}

// instantiate for Types and Linkabes
template class SymbolTable<Type>;
template class SymbolTable<ConstantValue>;
template class SymbolTable<Linkable>;

}
