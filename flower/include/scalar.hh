/*
  scalar.hh -- declare Scalar

  source file of the Flower Library

  (c)  1997--1998 Han-Wen Nienhuys <hanwen@cs.uu.nl>
*/


#ifndef SCALAR_HH
#define SCALAR_HH

#include "string.hh"
#include "real.hh"
#include "matrix.hh"

/// Perl -like scalar type.
struct Scalar : public String 
{
  Scalar (Real r) { *this = to_str (r); }
  Scalar (int i) { *this = to_str (i); }
  Scalar (char c) { *this = to_str (c); }
  Scalar (char const *c) : String (c) {}    
  Scalar (String s) : String (s) {}
  Scalar (Rational);
  operator Rational();
  Scalar() {}
  bool isnum_b() const;
  operator Real();
  operator int();
  bool to_bool () const;

  /*
    urg, these are bit silly; perhaps should make "Print_string" class
    (derive from Scalar?)
   */
  Scalar (Vector v) { *this = v.str (); }
  Scalar (Matrix m) { *this = m.str (); }

  /**   perl -like string to bool conversion.
   */
  operator bool() const;
};

#endif // SCALAR_HH

