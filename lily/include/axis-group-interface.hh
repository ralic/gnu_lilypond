/*   
  axis-group-interface.hh -- declare Axis_group_interface
  
  source file of the GNU LilyPond music typesetter
  
  (c) 2000 Han-Wen Nienhuys <hanwen@cs.uu.nl>
  
 */

#ifndef AXIS_GROUP_INTERFACE_HH
#define AXIS_GROUP_INTERFACE_HH

#include "group-interface.hh"

/**
   Treat a group of elements as a union. This sets the parent of any S
   added to ELT_L_ to ELT_L_.

   Properties:

   axes -- list of axis (number) in which this group works

   transparent -- an Axis_group is transparent by default

   elements -- contains list of pointers to other elements

   interfaces -- Axis_group is added to this list.
*/
struct Axis_group_interface 
{
  static Interval group_extent_callback (Score_element *,Axis);
  static Interval relative_group_extent (Axis, Score_element * common, SCM list);

  static void add_element (Score_element* me, Score_element*);
  static void set_axes (Score_element*,Axis,Axis);
  static bool axis_b (Score_element*,Axis);
  static Link_array<Score_element> get_children (Score_element*);
  static bool has_interface (Score_element*);
  static void set_interface (Score_element*);
  
};

#endif /* AXIS_GROUP_INTERFACE_HH */

