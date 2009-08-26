------------------------------------------------------------------------------
--                                                                          --
--                         GNAT COMPILER COMPONENTS                         --
--                                                                          --
--                             G E T _ T A R G                              --
--                                                                          --
--                                 B o d y                                  --
--                                                                          --
--          Copyright (C) 1992-2003 Free Software Foundation, Inc.          --
--                                                                          --
-- GNAT is free software;  you can  redistribute it  and/or modify it under --
-- terms of the  GNU General Public License as published  by the Free Soft- --
-- ware  Foundation;  either version 2,  or (at your option) any later ver- --
-- sion.  GNAT is distributed in the hope that it will be useful, but WITH- --
-- OUT ANY WARRANTY;  without even the  implied warranty of MERCHANTABILITY --
-- or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License --
-- for  more details.  You should have  received  a copy of the GNU General --
-- Public License  distributed with GNAT;  see file COPYING.  If not, write --
-- to  the  Free Software Foundation,  51  Franklin  Street,  Fifth  Floor, --
-- Boston, MA 02110-1301, USA.                                              --
--                                                                          --
-- GNAT was originally developed  by the GNAT team at  New York University. --
-- Extensive contributions were provided by Ada Core Technologies Inc.      --
--                                                                          --
------------------------------------------------------------------------------

package body Get_Targ is

   ----------------------
   -- Digits_From_Size --
   ----------------------

   function Digits_From_Size (Size : Pos) return Pos is
   begin
      --  LLVM local begin
      if    Size =  32 then
         return  6;
      elsif Size =  48 then
         return  9;
      elsif Size =  64 then
         return 15;
      elsif Size =  96 then
         return 18;
      elsif Size = 128 then
         return 18;
      --  LLVM local end
      else
         raise Program_Error;
      end if;
   end Digits_From_Size;

   ---------------------
   -- Width_From_Size --
   ---------------------

   function Width_From_Size  (Size : Pos) return Pos is
   begin
      --  LLVM local begin
      if    Size =  8 then
         return  4;
      elsif Size = 16 then
         return  6;
      elsif Size = 32 then
         return 11;
      elsif Size = 64 then
         return 21;
      --  LLVM local end
      else
         raise Program_Error;
      end if;
   end Width_From_Size;

   -----------------------------
   -- Get_Max_Unaligned_Field --
   -----------------------------

   function Get_Max_Unaligned_Field return Pos is
   begin
      return 64;  -- Can be different on some targets (e.g., AAMP)
   end Get_Max_Unaligned_Field;

end Get_Targ;
