/*
  lily-guile.cc -- implement assorted guile functions

  source file of the GNU LilyPond music typesetter

  (c) 1998--2000 Jan Nieuwenhuizen <janneke@gnu.org>

  Han-Wen Nienhuys <hanwen@cs.uu.nl>
*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>		// isinf

#include "libc-extension.hh"
#include "lily-guile.hh"
#include "main.hh"
#include "simple-file-storage.hh"
#include "file-path.hh"
#include "debug.hh"
#include "direction.hh"
#include "offset.hh"
#include "interval.hh"

SCM
ly_str02scm (char const*c)
{
  // this all really sucks, guile should take char const* arguments!
  return gh_str02scm ((char*)c);
}



/*
  Pass string to scm parser, evaluate one expression.
  Return result value and #chars read.

  Thanks to Gary Houston <ghouston@freewire.co.uk>

  Need guile-1.3.4 (>1.3 anyway) for ftell on str ports -- jcn
*/
SCM
ly_parse_scm (char const* s, int* n)
{
  SCM str = gh_str02scm ((char*)s);
  SCM port = scm_mkstrport (SCM_INUM0, str, SCM_OPN | SCM_RDNG,
                            "scm_eval_0str");
  SCM from = scm_ftell (port);

  SCM form;
  SCM answer = SCM_UNSPECIFIED;

  /* Read expression from port */
  if (!SCM_EOF_OBJECT_P (form = scm_read (port)))
    answer = scm_eval_x (form);

  /*
   After parsing

       (begin (foo 1 2))

   all seems fine, but after parsing

       (foo 1 2)

   read_buf has been advanced to read_pos - 1,
   so that scm_ftell returns 1, instead of #parsed chars
   */
  
  /*
    urg: reset read_buf for scm_ftell
    shouldn't scm_read () do this for us?
  */
  scm_fill_input (port);
  SCM to = scm_ftell (port);
  *n = gh_scm2int (to) - gh_scm2int (from);

  /* Don't close the port here; if we re-enter this function via a
     continuation, then the next time we enter it, we'll get an error.
     It's a string port anyway, so there's no advantage to closing it
     early.

     scm_close_port (port);
  */

  return answer;
}

SCM
ly_quote_scm (SCM s)
{
  return gh_list (ly_symbol2scm ("quote"), s, SCM_UNDEFINED);
}


SCM
ly_symbol2scm(const char *s)
{
  return gh_symbol2scm ((char *)s);
}

String
ly_symbol2string (SCM s)
{
  assert (gh_symbol_p (s));
  return String((Byte*)SCM_CHARS (s), (int) SCM_LENGTH(s));
}


/**
   Read a file, and shove it down GUILE.  GUILE also has file read
   functions, but you can't fiddle with the path of those.
 */
void
read_lily_scm_file (String fn)
{
  String s = global_path.find (fn);
  if (s == "")
    {
      String e = _f ("can't find file: `%s'", fn);
      e += " ";
      e += _f ("(load path: `%s')", global_path.str ());
      error (e);
    }
  else
    progress_indication ("[" + s);


  Simple_file_storage f(s);
  
  gh_eval_str ((char *) f.ch_C());
  progress_indication ("]");
}


SCM
ly_gulp_file (SCM name)
{
  String fn (ly_scm2string (name));
 String s = global_path.find (fn);
  if (s == "")
    {
      String e = _f ("can't find file: `%s'", fn);
      e += " ";
      e += _f ("(load path: `%s')", global_path.str ());
      error (e);
    }
  else
    progress_indication ("[" + s );


  Simple_file_storage f(s);
  SCM result = ly_str02scm (f.ch_C());
  progress_indication ("]");
  return result;
}

void
ly_display_scm (SCM s)
{
  gh_display (s);
  gh_newline ();
}

String
ly_scm2string (SCM s)
{
  assert (gh_string_p (s));
  int len; 
  char * p = gh_scm2newstr (s , &len);
  
  String r (p);

  free (p);
  return r;
}

SCM
index_cell (SCM s, Direction d)
{
  assert (d);
  return (d == LEFT) ? gh_car  (s) : gh_cdr (s);
}

SCM
index_set_cell (SCM s, Direction d, SCM v)
{
  if (d == LEFT)
    gh_set_car_x (s, v);
  else if (d == RIGHT)
    gh_set_cdr_x (s, v);
  return s;
}
  
SCM
ly_warning (SCM str)
{
  assert (gh_string_p (str));
  warning ("lily-guile: " + ly_scm2string (str));
  return SCM_BOOL_T;
}

SCM
ly_isdir_p (SCM s)
{
  if (gh_number_p (s))
    {
      int i = gh_scm2int (s);
      return (i>= -1 && i <= 1)  ? SCM_BOOL_T : SCM_BOOL_F; 
    }
  return SCM_BOOL_F;
}



typedef void (*Void_fptr)();
Array<Void_fptr> *scm_init_funcs_;

void add_scm_init_func (void (*f)())
{
  if (!scm_init_funcs_)
    scm_init_funcs_ = new Array<Void_fptr>;

  scm_init_funcs_->push (f);
}

void
init_lily_guile ()
{
  for (int i=scm_init_funcs_->size() ; i--;)
    (scm_init_funcs_->elem (i)) ();
}

unsigned int ly_scm_hash (SCM s)
{
  return scm_ihashv (s, ~1u);
}



bool
isdir_b (SCM s)
{
  if (gh_number_p (s))
    {
      int i = gh_scm2int (s);
      return i>= -1 && i <= 1; 
    }
  return false;
}

Direction
to_dir (SCM s)
{
  return (Direction) gh_scm2int (s);
}

Interval
ly_scm2interval (SCM p)
{
  return  Interval (gh_scm2double (gh_car (p)),
		    gh_scm2double (gh_cdr (p)));
}

SCM
ly_interval2scm (Interval i)
{
  return gh_cons (gh_double2scm (i[LEFT]),
		  gh_double2scm (i[RIGHT]));
}


bool
to_boolean (SCM s)
{
  return gh_boolean_p (s) && gh_scm2bool (s);
}

/*
  Appendable list L: the cdr contains the list, the car the last cons
  in the list.
 */
SCM
appendable_list ()
{
  SCM s = gh_cons (SCM_EOL, SCM_EOL);
  gh_set_car_x (s, s);
  
  return s;
}

void
appendable_list_append (SCM l, SCM elt)
{
  SCM newcons = gh_cons (elt, SCM_EOL);
  
  gh_set_cdr_x (gh_car (l), newcons);      
  gh_set_car_x (l, newcons);
}


SCM
ly_offset2scm (Offset o)
{
  return gh_cons (gh_double2scm (o[X_AXIS]), gh_double2scm(o[Y_AXIS]));
}

Offset
ly_scm2offset (SCM s)
{
  return Offset (gh_scm2double (gh_car (s)),
		 gh_scm2double (gh_cdr (s)));
}

SCM
ly_type (SCM exp)
{
  char const  * cp = "unknown";
  if (gh_number_p (exp))
    {
      cp = "number";
    }
  else if (gh_string_p (exp))
    {
      cp = "string";
    }
  else if (gh_procedure_p (exp))
    {
      cp = "procedure";
    }
  else if (gh_boolean_p (exp))
    {
      cp = "boolean";
    }
  else if (gh_pair_p (exp))
    {
      cp = "list";
    }

  return ly_str02scm (cp);
}

/*
  convert without too many decimals, and leave  a space at the end.
 */
   
   
SCM
ly_number2string (SCM s)
{
  assert (gh_number_p (s));

  char str[100];			// ugh.

  if (scm_integer_p (s))
    {
      Real r (gh_scm2double (s));

      if (isinf (r) || isnan (r))
	{
	  programming_error ("Infinity or NaN encountered while converting Real number; setting to zero.");
	  r = 0.0;
	}

      sprintf (str, "%8.4f ", r);
    }
  else
    {
      sprintf (str, "%d ", gh_scm2int (s));
    }

  return gh_str02scm (str);
}

#define TEST_GC

#ifdef TEST_GC
#include <libguile/gc.h>

static void *
greet_sweep (void *dummy1, void *dummy2, void *dummy3)
{
   fprintf(stderr, "entering sweep\n");
}

static void *
wave_sweep_goodbye (void *dummy1, void *dummy2, void *dummy3)
{
   fprintf(stderr, "leaving sweep\n");
}
#endif

static void
init_functions ()
{
  scm_make_gsubr ("ly-warn", 1, 0, 0, (SCM(*)(...))ly_warning);
  scm_make_gsubr ("ly-gulp-file", 1,0, 0, (SCM(*)(...))ly_gulp_file);
  scm_make_gsubr ("dir?", 1,0, 0, (SCM(*)(...))ly_isdir_p);
  scm_make_gsubr ("ly-number->string", 1, 0,0, (SCM(*)(...)) ly_number2string);


#ifdef TEST_GC 
  scm_c_hook_add (&scm_before_mark_c_hook, greet_sweep, 0, 0);
  scm_c_hook_add (&scm_before_sweep_c_hook, wave_sweep_goodbye, 0, 0);
#endif
  
}

ADD_SCM_INIT_FUNC(funcs, init_functions);
