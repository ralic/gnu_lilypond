/*
  rhythmic-head.cc -- implement Rhythmic_head

  source file of the GNU LilyPond music typesetter

  (c) 1997--2005 Han-Wen Nienhuys <hanwen@cs.uu.nl>
*/

#include "rhythmic-head.hh"

#include "warn.hh"
#include "rest.hh"
#include "stem.hh"
#include "staff-symbol-referencer.hh"
#include "item.hh"

Item *
Rhythmic_head::get_dots (Grob *me)
{
  SCM s = me->get_property ("dot");
  return unsmob_item (s);
}

Item *
Rhythmic_head::get_stem (Grob *me)
{
  SCM s = me->get_property ("stem");
  return unsmob_item (s);
}

int
Rhythmic_head::dot_count (Grob *me)
{
  return get_dots (me)
    ? scm_to_int (get_dots (me)->get_property ("dot-count")) : 0;
}

void
Rhythmic_head::set_dots (Grob *me, Item *dot)
{
  me->set_property ("dot", dot->self_scm ());
}

int
Rhythmic_head::duration_log (Grob *me)
{
  SCM s = me->get_property ("duration-log");
  return scm_is_number (s) ? scm_to_int (s) : 0;
}

ADD_INTERFACE (Rhythmic_head, "rhythmic-head-interface",
	       "Note head or rest",
	       "dot stem duration-log");

