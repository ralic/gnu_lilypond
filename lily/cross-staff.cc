#include "cross-staff.hh"
#include "item.hh"
#include "align-interface.hh"
#include "spanner.hh"
#include "warn.hh"

/*
  JUNKME
 */
Real
calc_interstaff_dist (Item const *item, Spanner const *span)
{
  Real interstaff = 0.0; 
  Score_element *common = item->common_refpoint (span, Y_AXIS);


  if (Align_interface::has_interface (common) && Align_interface::axis(common) == Y_AXIS)
    {
      SCM threshold = common->get_elt_property ("threshold");
      if (!gh_pair_p (threshold)
	  || !scm_equal_p (gh_car (threshold), gh_cdr (threshold)))
	warning (_ ("minVerticalAlign != maxVerticalAlign: cross staff spanners may be broken"));

      interstaff = 0.0;
      if (gh_pair_p (threshold))
	interstaff =  gh_scm2double (gh_car (threshold));

      Score_element const * span_refpoint = span;
      while (span_refpoint->parent_l  (Y_AXIS) != common)
	span_refpoint = span_refpoint->parent_l (Y_AXIS);

      Score_element const * note_refpoint = item;
      while (note_refpoint->parent_l (Y_AXIS) != common)
	note_refpoint = note_refpoint->parent_l (Y_AXIS);

      int span_prio =
	Align_interface::get_count (common,(Score_element*) dynamic_cast<Score_element const*> (span_refpoint));
      int item_prio =
	Align_interface::get_count (common,(Score_element*) dynamic_cast<Score_element  const *> (note_refpoint));

      /*
	our staff is lower -> interstaff *= -1
       */

      if (span_prio < item_prio)
	interstaff *= -1;
      return interstaff;
    }
  else return 0.0;
}

