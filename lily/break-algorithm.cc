/*
  break.cc -- implement Break_algorithm

  source file of the GNU LilyPond music typesetter

  (c) 1996--2005 Han-Wen Nienhuys <hanwen@cs.uu.nl>
*/

#include "break-algorithm.hh"
#include "paper-column.hh"
#include "output-def.hh"
#include "system.hh"
#include "paper-score.hh"
#include "paper-column.hh"
#include "cpu-timer.hh"
#include "simple-spacer.hh"
#include "group-interface.hh"

Array<int>
Break_algorithm::find_break_indices () const
{
  Link_array<Grob> all = pscore_->system_->columns ();
  Array<int> retval;

  for (int i= 0; i < all.size (); i++)
    if (Item::is_breakable (all[i]))
      retval.push (i);

  if (linewidth_ <= 0)
    while (retval.size () > 2)
      retval.del (1);

  return retval;
}

Link_array<Grob>
Break_algorithm::find_breaks () const
{
  Link_array<Grob> all = pscore_->system_->columns ();
  Link_array<Grob> retval;

  for (int i= 0; i < all.size (); i++)
    if (Item::is_breakable (all[i]))
      retval.push (all[i]);

  if (linewidth_ <= 0)
    while (retval.size () >2)
      retval.del (1);

  return retval;
}

Simple_spacer_wrapper *
Break_algorithm::generate_spacing_problem (Link_array<Grob> const &curline,
					   Interval line) const
{
  Simple_spacer_wrapper *spw = new Simple_spacer_wrapper;
  Simple_spacer *sp = spw->spacer_;

  /*
    this is hardcoded, but this shouldn't happen anyway.
    used to be get_dimension (ly_symbol2scm ("loose_column_distance"));
  */
  sp->default_space_ = 1.0;
  sp->indent_ = line[LEFT];

  /*
    sort out how interfacing this should work;
  */
  if (line.is_empty ())
    {
      sp->line_len_ = -1;
    }
  else
    sp->line_len_ = line.length ();

  spw->add_columns (curline);
  return spw;
}

Break_algorithm::Break_algorithm ()
{
  pscore_ = 0;
  linewidth_ = 0;
}

void
Break_algorithm::set_pscore (Paper_score *s)
{
  pscore_ = s;
  linewidth_ = s->layout_->get_dimension (ly_symbol2scm ("linewidth"));
}

Array<Column_x_positions>
Break_algorithm::solve () const
{
  Array<Column_x_positions> h= do_solve ();

  return h;
}

