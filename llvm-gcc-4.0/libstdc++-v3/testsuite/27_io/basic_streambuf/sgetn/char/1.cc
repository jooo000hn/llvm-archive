// 1999-10-11 bkoz

// Copyright (C) 1999, 2000, 2001, 2002, 2003, 2005
// Free Software Foundation, Inc.
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
// Software Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
// USA.

// As a special exception, you may use this file as part of a free software
// library without restriction.  Specifically, if other files instantiate
// templates or use macros or inline functions from this file, or you compile
// this file and link it with other files to produce an executable, this
// file does not by itself cause the resulting executable to be covered by
// the GNU General Public License.  This exception does not however
// invalidate any other reasons why the executable file might be covered by
// the GNU General Public License.

// 27.5.2 template class basic_streambuf

#include <streambuf>
#include <testsuite_hooks.h>

class testbuf : public std::streambuf
{
public:

  // Typedefs:
  typedef std::streambuf::traits_type traits_type;
  typedef std::streambuf::char_type char_type;

  testbuf(): std::streambuf() 
  { }

  bool
  check_pointers()
  { 
    bool test __attribute__((unused)) = true;
    VERIFY( this->eback() == NULL );
    VERIFY( this->gptr() == NULL );
    VERIFY( this->egptr() == NULL );
    VERIFY( this->pbase() == NULL );
    VERIFY( this->pptr() == NULL );
    VERIFY( this->epptr() == NULL );
    return test;
  }

  int_type 
  pub_uflow() 
  { return (this->uflow()); }

  int_type 
  pub_overflow(int_type __c = traits_type::eof()) 
  { return (this->overflow(__c)); }

  int_type 
  pub_pbackfail(int_type __c) 
  { return (this->pbackfail(__c)); }

  void 
  pub_setg(char* beg, char* cur, char *end) 
  { this->setg(beg, cur, end); }

  void 
  pub_setp(char* beg, char* end) 
  { this->setp(beg, end); }

protected:
  int_type 
  underflow() 
  { 
    int_type __retval = traits_type::eof();
    if (this->gptr() < this->egptr())
      __retval = traits_type::not_eof(0); 
    return __retval;
  }
};

void test02()
{
  typedef testbuf::traits_type traits_type;
  typedef testbuf::int_type int_type;

  bool test __attribute__((unused)) = true;

  char lit01[] = "chicago underground trio/possible cube on delmark";
  size_t i01 = traits_type::length(lit01);

  testbuf buf01;

  // 27.5.2.1 basic_streambuf ctors
  // default ctor initializes 
  // - all pointer members to null pointers
  // - locale to current global locale
  VERIFY( buf01.check_pointers() );
  VERIFY( buf01.getloc() == std::locale() );

  // 27.5.2.2.5 Put area

  char carray01[i01];
  std::memset(carray01, 0, i01);
  
  buf01.pub_setg(lit01, lit01, lit01 + i01);
  buf01.sgetn(carray01, 0);
  VERIFY( carray01[0] == 0 );
  buf01.sgetn(carray01, 1);
  VERIFY( carray01[0] == 'c' );
  buf01.sgetn(carray01 + 1, i01 - 1);
  VERIFY( carray01[0] == 'c' );
  VERIFY( carray01[1] == 'h' );
  VERIFY( carray01[i01 - 1] == 'k' );
}

int main() 
{
  test02();
  return 0;
}
