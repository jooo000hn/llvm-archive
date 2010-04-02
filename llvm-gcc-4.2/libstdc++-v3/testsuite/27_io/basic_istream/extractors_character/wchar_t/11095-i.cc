// Copyright (C) 2004 Free Software Foundation
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 2, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING.  If not, write to the Free
// Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,
// USA.

// 27.6.1.2.3 character extractors

#include <istream>
#include <sstream>
#include <testsuite_hooks.h>

// libstdc++/11095
// operator>>(basic_istream&, _CharT*)
void test01() 
{
  bool test __attribute__((unused)) = true;
  const std::wstring str_01(L"Consoli ");

  std::wstringbuf isbuf_01(str_01, std::ios_base::in);
  std::wistream is_01(&isbuf_01);

  std::ios_base::iostate state1, state2;

  wchar_t array1[10];
  typedef std::wios::traits_type ctraits_type;

  is_01.width(-60);
  state1 = is_01.rdstate();
  is_01 >> array1;
  state2 = is_01.rdstate();
  VERIFY( state1 == state2 );
  VERIFY( !ctraits_type::compare(array1, L"Consoli", 7) );
}

int main()
{
  test01();
  return 0;
}

