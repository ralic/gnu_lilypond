/*
  paper-column.hh -- declare  Paper_column

  source file of the GNU LilyPond music typesetter

  (c)  1997--2000 Han-Wen Nienhuys <hanwen@cs.uu.nl>
*/


#ifndef P_COL_HH
#define P_COL_HH

#include "item.hh"
#include "rod.hh"
#include "spring.hh"

/**
   bounded-by-me -- list of elts.

   shortest-starter-duration -- rational signifying shortest moment that starts here


   Interfaces:

   axis-group, spaceable-element.
  */

class Paper_column : public Item
{ 
public:
  VIRTUAL_COPY_CONS(Score_element);

  /*
    Not (yet) in scm, because of messy effects when a column commits suicide.
   */
  int  rank_i_;
  virtual void do_break_processing ();
  virtual Paper_column *column_l () const;
  virtual Line_of_score *line_l () const;

  /// if lines are broken then this column is in #line#
  Line_of_score *line_l_;

  /// which  one (left =0)
  static int rank_i(Score_element*);

  Paper_column (SCM);
  bool musical_b () const;
  static Moment when_mom (Score_element*);

  static bool used_b (Score_element*) ;
  void set_rank (int);
};
     
#endif // P_COL_HH

