/*
  translator-scheme.cc --  implement Scheme context functions

  source file of the GNU LilyPond music typesetter

  (c) 2002--2004 Han-Wen Nienhuys <hanwen@cs.uu.nl>

 */

#include "translator.hh"
#include "context-def.hh"
#include "translator-group.hh"
#include "lily-guile.hh"


LY_DEFINE (ly_translator_name, "ly:translator-name",
	   1, 0, 0, (SCM trans),
	  "Return the type name of the translator object @var{trans}. "
	   "The name is a symbol.")
{
  Translator *tr = unsmob_translator (trans);
  SCM_ASSERT_TYPE (tr, trans, SCM_ARG1, __FUNCTION__, "Translator");
  char const *nm = classname (tr);
  return ly_symbol2scm (nm);
}

LY_DEFINE (ly_translator_description, "ly:translator-description",
	  1,0,0, (SCM me),
	  "Return an alist of properties of  translator @var{me}.")
{
  Translator *tr = unsmob_translator (me);
  SCM_ASSERT_TYPE (tr, me, SCM_ARG1, __FUNCTION__, "Translator");
  return tr->translator_description ();
}

int
Translator::print_smob (SCM s, SCM port, scm_print_state *)
{
  Translator *me = (Translator*) ly_cdr (s);
  scm_puts ("#<Translator ", port);
  scm_puts (classname (me), port);
  scm_display (me->simple_trans_list_, port);
  
  scm_puts (" >", port);
  return 1;
}

