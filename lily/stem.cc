/*
  stem.cc -- implement Stem

  source file of the GNU LilyPond music typesetter

  (c) 1996, 1997--1999 Han-Wen Nienhuys <hanwen@cs.uu.nl>
    Jan Nieuwenhuizen <janneke@gnu.org>

  TODO: This is way too hairy
*/
#include "directional-element-interface.hh"
#include "dimension-cache.hh"
#include "stem.hh"
#include "debug.hh"
#include "paper-def.hh"
#include "note-head.hh"
#include "lookup.hh"
#include "molecule.hh"
#include "paper-column.hh"
#include "misc.hh"
#include "beam.hh"
#include "rest.hh"
#include "group-interface.hh"
#include "cross-staff.hh"
#include "staff-symbol-referencer.hh"
#include "lily-guile.icc"


void
Stem::set_beaming (int i,  Direction d )
{
  SCM pair = get_elt_property ("beaming");
  
  if (!gh_pair_p (pair))
    {
      pair = gh_cons (gh_int2scm (0),gh_int2scm (0));
      set_elt_property ("beaming", pair);
    }
  index_set_cell (pair, d, gh_int2scm (i));
}

int
Stem::beam_count (Direction d) const
{
  SCM p=get_elt_property ("beaming");
  if (gh_pair_p (p))
    return gh_scm2int (index_cell (p,d));
  else
    return 0;
}

Interval
Stem::head_positions () const
{
  if (!heads_i ())
    {
      Interval iv;
      return iv;
    }

  
  Drul_array<Note_head*> e (extremal_heads ());

  return Interval (staff_symbol_referencer_interface (e[DOWN]).position_f (),
		   staff_symbol_referencer_interface( e[UP]).position_f ()); 
}


Real
Stem::chord_start_f () const
{
  return head_positions()[get_direction ()]
    * Staff_symbol_referencer_interface (this).staff_space ()/2.0;
}

Real
Stem::stem_end_position () const
{
  SCM p =get_elt_property ("stem-end-position");
  Real pos;
  if (!gh_number_p (p))
    {
      Stem * me = (Stem*) this;
      pos = get_default_stem_end_position ();
      me->set_elt_property ("stem-end-position", gh_double2scm (pos));
    }
  else
    pos = gh_scm2double (p);

  return pos;
}

Direction
Stem::get_direction () const
{
  Direction d = directional_element (this).get ();

  if (!d)
    {
       Stem * me = (Stem*) this;
       d = get_default_dir ();
       directional_element (me).set (d);
    }
  return d ;
}


void
Stem::set_stemend (Real se)
{
  // todo: margins
  Direction d= get_direction ();
  
  if (d && d * head_positions()[get_direction ()] >= se*d)
    warning (_ ("Weird stem size; check for narrow beams"));

  set_elt_property ("stem-end-position", gh_double2scm (se));
}

int
Stem::type_i () const
{
  return first_head () ?  first_head ()->balltype_i () : 2;
}

/*
  Note head that determines hshift for upstems
 */ 
Score_element*
Stem::support_head ()const
{
  SCM h = get_elt_property ("support-head");
  Score_element * nh = unsmob_element (h);
  if (nh)
    return nh;
  else if (heads_i () == 1)
    {
      /*
	UGH.
       */
      
      return unsmob_element (gh_car (get_elt_property ("heads")));
    }
  else
    return first_head ();
}


int
Stem::heads_i ()const
{
  Group_interface gi (this, "heads");
  return gi.count ();
}

/*
  The note head which forms one end of the stem.  
 */
Note_head*
Stem::first_head () const
{
  return extremal_heads ()[-get_direction ()];
}

/*
  START is part where stem reaches `last' head. 
 */
Drul_array<Note_head*>
Stem::extremal_heads () const
{
  const int inf = 1000000;
  Drul_array<int> extpos;
  extpos[DOWN] = inf;
  extpos[UP] = -inf;  
  
  Drul_array<Note_head *> exthead;
  exthead[LEFT] = exthead[RIGHT] =0;
  
  for (SCM s = get_elt_property ("heads"); gh_pair_p (s); s = gh_cdr (s))
    {
      Note_head * n = dynamic_cast<Note_head*> (unsmob_element (gh_car (s)));
      Staff_symbol_referencer_interface si (n);
      
      int p = int(si.position_f ());

      Direction d = LEFT;
      do {
      if (d* p > d* extpos[d])
	{
	  exthead[d] = n;
	  extpos[d] = p;
	}
      } while (flip (&d) != DOWN);
    }

  return exthead;
}

void
Stem::add_head (Rhythmic_head *n)
{
  n->set_elt_property ("stem", this->self_scm_);
  n->add_dependency (this);

  Group_interface gi (this);
  if (Note_head *nh = dynamic_cast<Note_head *> (n))
    gi.name_ = "heads";
  else
    gi.name_ = "rests";

  gi.add_element (n);
}

Stem::Stem ()
{
  set_elt_property ("heads", SCM_EOL);
  set_elt_property ("rests", SCM_EOL);
}

bool
Stem::invisible_b () const
{
  /*
    UGH. Who determines balltype for stem?
   */
  Note_head * nh = dynamic_cast<Note_head*> (support_head ());
  return !(heads_i () && nh->balltype_i () >= 1);
}

int
Stem::get_center_distance (Direction d) const
{
  int staff_center = 0;
  int distance = (int) (d*(head_positions()[d] - staff_center));
  return distance >? 0;
}

Direction
Stem::get_default_dir () const
{
  int du = get_center_distance (UP);
  int dd = get_center_distance (DOWN);

  if (sign (dd - du))
    return Direction (sign (dd -du));

  return Direction (int(paper_l ()->get_var ("stem_default_neutral_direction")));
}

Real
Stem::get_default_stem_end_position () const
{
  bool grace_b = get_elt_property ("grace") != SCM_UNDEFINED;
  String type_str = grace_b ? "grace-" : "";
  SCM s;
  Array<Real> a;

  Real length_f = 0.;
  SCM scm_len = get_elt_property("length");
  if (gh_number_p (scm_len))
    {
      length_f = gh_scm2double (scm_len);
    }
  else
    {
      s = ly_eval_str (type_str + "stem-length");
      scm_to_array (s, &a);
      // stem uses half-spaces
      length_f = a[((flag_i () - 2) >? 0) <? (a.size () - 1)] * 2;
    }


  s = ly_eval_str (type_str + "stem-shorten");
  scm_to_array (s, &a);

  // stem uses half-spaces
  Real shorten_f = a[((flag_i () - 2) >? 0) <? (a.size () - 1)] * 2;

  /* URGURGURG
     'set-default-stemlen' sets direction too
   */
  Direction dir = get_direction ();
  if (!dir)
    {
      dir = get_default_dir ();
      directional_element (this).set (dir);
    }
  
  /* 
    stems in unnatural (forced) direction should be shortened, 
    according to [Roush & Gourlay]
   */
  if (((int)chord_start_f ())
      && (get_direction () != get_default_dir ()))
    length_f -= shorten_f;


   Real st = head_positions()[dir] + dir * length_f;
  
   bool no_extend_b = to_boolean (get_elt_property ("no-stem-extend"));
    if (!grace_b && !no_extend_b && dir * st < 0)
      st = 0.0;

  return st;
}

/*
  FIXME: wrong name
 */
int
Stem::flag_i () const
{
  SCM s = get_elt_property ("duration-log");
  return  (gh_number_p (s)) ? gh_scm2int (s) : 2;
}

void
Stem::position_noteheads ()
{
  if (!heads_i ())
    return;
  
  Link_array<Score_element> heads =
    Group_interface__extract_elements (this, (Score_element*)0, "heads");

  heads.sort (compare_position);
  Direction dir =get_direction ();
  
  if (dir < 0)
    heads.reverse ();


  Real w = support_head ()->extent (X_AXIS)[dir];
  for (int i=0; i < heads.size (); i++)
    {
      heads[i]->translate_axis (w - heads[i]->extent (X_AXIS)[dir], X_AXIS);
    }
  
  bool parity= true;		// todo: make this settable.
  int lastpos = int (Staff_symbol_referencer_interface (heads[0]).position_f ());
  for (int i=1; i < heads.size (); i ++)
    {
      Real p = Staff_symbol_referencer_interface (heads[i]).position_f ();
      int dy =abs (lastpos- (int)p);

      if (dy <= 1)
	{
	  if (parity)
	    {
	      Real l  = heads[i]->extent (X_AXIS).length ();
	      heads[i]->translate_axis (l * get_direction (), X_AXIS);
	    }
	  parity = !parity;
	}
      else
	parity = true;
      
      lastpos = int (p);
    }
}

void
Stem::do_pre_processing ()
{
  stem_end_position ();	// ugh. Trigger direction calc.
  position_noteheads ();

  if (invisible_b ())
    {
      set_elt_property ("transparent", SCM_BOOL_T);
      set_empty (Y_AXIS);      
      set_empty (X_AXIS);      
    }

  set_spacing_hints ();
}



/**
   set stem directions for hinting the optical spacing correction.

   Modifies DIR_LIST property of the Stem's Paper_column

   TODO: more advanced: supply height of noteheads as well, for more advanced spacing possibilities
 */
void
Stem::set_spacing_hints () 
{
  if (!invisible_b ())
    {
      SCM scmdir  = gh_int2scm (get_direction ());
      SCM dirlist = column_l ()->get_elt_property ("dir-list");
      if (dirlist == SCM_UNDEFINED)
	dirlist = SCM_EOL;

      if (scm_sloppy_memq (scmdir, dirlist) == SCM_EOL)
	{
	  dirlist = gh_cons (scmdir, dirlist);
	  column_l ()->set_elt_property ("dir-list", dirlist);
	}
    }
}

Molecule
Stem::flag () const
{
  String style;
  SCM st = get_elt_property ("style");
  if ( st != SCM_UNDEFINED)
    {
      style = ly_scm2string (st);
    }

  char c = (get_direction () == UP) ? 'u' : 'd';
  Molecule m = lookup_l ()->afm_find (String ("flags-") + to_str (c) + 
				      to_str (flag_i ()));
  if (!style.empty_b ())
    m.add_molecule(lookup_l ()->afm_find (String ("flags-") + to_str (c) + style));
  return m;
}

Interval
Stem::dim_callback (Dimension_cache const* c) 
{
  Stem * s = dynamic_cast<Stem*> (c->element_l ());
  
  Interval r (0, 0);
  if (s->get_elt_property ("beam") != SCM_UNDEFINED || abs (s->flag_i ()) <= 2)
    ;	// TODO!
  else
    {
      r = s->flag ().dim_.x ();
      r += s->note_delta_f ();
    }
  return r;
}


const Real ANGLE = 20* (2.0*M_PI/360.0); // ugh!

Molecule*
Stem::do_brew_molecule_p () const
{
  Molecule *mol_p =new Molecule;

  Staff_symbol_referencer_interface si (first_head ());
  
  Real y1 = si.position_f();
  Real y2 = stem_end_position ();
  
  Interval stem_y(y1,y2);
  stem_y.unite (Interval (y2,y1));

  Real dy = staff_symbol_referencer_interface (this)
    .staff_space ()/2.0;

  Real head_wid = 0;
  if (support_head ())
    head_wid = support_head ()->extent (X_AXIS).length ();
  stem_y[Direction(-get_direction ())] += get_direction () * head_wid * tan(ANGLE)/(2*dy);
  
  if (!invisible_b ())
    {
      Real stem_width = paper_l ()->get_var ("stemthickness");
      Molecule ss =lookup_l ()->filledbox (Box (Interval (-stem_width/2, stem_width/2),
						 Interval (stem_y[DOWN]*dy, stem_y[UP]*dy)));
      mol_p->add_molecule (ss);
    }

  if (!beam_l () && abs (flag_i ()) > 2)
    {
      Molecule fl = flag ();
      fl.translate_axis(stem_y[get_direction ()]*dy, Y_AXIS);
      mol_p->add_molecule (fl);
    }

  if (first_head ())
    {
      mol_p->translate_axis (note_delta_f (), X_AXIS);
    }
  return mol_p;
}

Real
Stem::note_delta_f () const
{
  Real r=0;
  if (first_head ())
    {
      Interval head_wid(0,  first_head()->extent (X_AXIS).length ());
         Real rule_thick = paper_l ()->get_var ("stemthickness");

      Interval stem_wid(-rule_thick/2, rule_thick/2);
      if (get_direction () == CENTER)
	r = head_wid.center ();
      else
	r = head_wid[get_direction ()] - stem_wid[get_direction ()];
    }
  return r;
}

Real
Stem::hpos_f () const
{
  return note_delta_f () + Item::hpos_f ();
}


Beam*
Stem::beam_l ()const
{
  SCM b=  get_elt_property ("beam");
  return dynamic_cast<Beam*> (unsmob_element (b));
}


// ugh still very long.
Stem_info
Stem::calc_stem_info () const
{
  assert (beam_l ());

  Direction beam_dir = directional_element (beam_l ()).get ();
  if (!beam_dir)
    {
      programming_error ("Beam dir not set.");
      beam_dir = UP;
    }
    
  Staff_symbol_referencer_interface st (this);
  Real staff_space = st.staff_space ();
  Real half_space = staff_space / 2;
  Real interbeam_f = paper_l ()->interbeam_f (beam_l ()->get_multiplicity ());
  Real thick = gh_scm2double (beam_l ()->get_elt_property ("beam-thickness"));
  int multiplicity = beam_l ()->get_multiplicity ();

  Stem_info info; 
  info.idealy_f_ = chord_start_f ();

  // for simplicity, we calculate as if dir == UP
  info.idealy_f_ *= beam_dir;
  SCM grace_prop = get_elt_property ("grace");

  bool grace_b = to_boolean (grace_prop);
  
  Array<Real> a;
  SCM s;
  String type_str = grace_b ? "grace-" : "";
  
  s = ly_eval_str (type_str + "beamed-stem-minimum-length");
  scm_to_array (s, &a);
  Real minimum_length = a[multiplicity <? (a.size () - 1)] * staff_space;

  s = ly_eval_str (type_str + "beamed-stem-length");
  scm_to_array (s, &a);
  Real stem_length =  a[multiplicity <? (a.size () - 1)] * staff_space;

  if (!beam_dir || (beam_dir == directional_element (this).get ()))
    /* normal beamed stem */
    {
      if (multiplicity)
	{
	  info.idealy_f_ += thick + (multiplicity - 1) * interbeam_f;
	}
      info.miny_f_ = info.idealy_f_;
      info.maxy_f_ = INT_MAX;

      info.idealy_f_ += stem_length;
      info.miny_f_ += minimum_length;

      /*
	lowest beam of (UP) beam must never be lower than second staffline

	Hmm, reference (Wanske?)

	Although this (additional) rule is probably correct,
	I expect that highest beam (UP) should also never be lower
	than middle staffline, just as normal stems.
	
      */
      bool no_extend_b = to_boolean (get_elt_property ("no-stem-extend"));
      if (!grace_b && !no_extend_b)
	{
	  /* highest beam of (UP) beam must never be lower than middle
	     staffline
	     lowest beam of (UP) beam must never be lower than second staffline
	   */
	  info.miny_f_ =
	    info.miny_f_ >? 0
	    >? (- 2 * half_space - thick
		+ (multiplicity > 0) * thick
		+ interbeam_f * (multiplicity - 1));
	}
    }
  else
    /* knee */
    {
      info.idealy_f_ -= thick;
      info.maxy_f_ = info.idealy_f_;
      info.miny_f_ = -INT_MAX;

      info.idealy_f_ -= stem_length;
      info.maxy_f_ -= minimum_length;
    }
  
  info.idealy_f_ = (info.maxy_f_ <? info.idealy_f_) >? info.miny_f_;

  s = beam_l ()->get_elt_property ("shorten");
  if (gh_number_p (s))
    info.idealy_f_ -= gh_double2scm (s);

  Real interstaff_f = -beam_dir* calc_interstaff_dist (this, beam_l ());

  info.idealy_f_ += interstaff_f;
  info.miny_f_ += interstaff_f;
  info.maxy_f_ += interstaff_f ;

  return info;
}

