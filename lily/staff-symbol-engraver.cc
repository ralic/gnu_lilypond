/*
  staff-symbol-engraver.cc -- implement Staff_symbol_engraver

  source file of the GNU LilyPond music typesetter

  (c) 1997--2005 Han-Wen Nienhuys <hanwen@cs.uu.nl>
*/

#include "score.hh"
#include "paper-column.hh"
#include "output-def.hh"
#include "side-position-interface.hh"
#include "engraver.hh"
#include "moment.hh"

/**
   Manage the staff symbol.
*/
class Staff_symbol_engraver : public Engraver
{
public:
  TRANSLATOR_DECLARATIONS (Staff_symbol_engraver);

protected:
  Spanner *span_;

  virtual ~Staff_symbol_engraver ();
  virtual void acknowledge_grob (Grob_info);
  virtual void finalize ();
  virtual void process_music ();
};

Staff_symbol_engraver::~Staff_symbol_engraver ()
{
  assert (!span_);
}

Staff_symbol_engraver::Staff_symbol_engraver ()
{
  span_ = 0;
}

void
Staff_symbol_engraver::process_music ()
{
  if (!span_)
    {
      span_ = make_spanner ("StaffSymbol", SCM_EOL);

      span_->set_bound (LEFT, unsmob_grob (get_property ("currentCommandColumn")));

    }
}

void
Staff_symbol_engraver::finalize ()
{
  if (span_)
    {
      span_->set_bound (RIGHT, unsmob_grob (get_property ("currentCommandColumn")));
    }
  span_ = 0;
}

void
Staff_symbol_engraver::acknowledge_grob (Grob_info s)
{
  s.grob_->set_property ("staff-symbol", span_->self_scm ());
}


ADD_TRANSLATOR (Staff_symbol_engraver,
		/* descr */ "Create the constellation of five (default) "
		"staff lines.",
		/* creats*/ "StaffSymbol",
		/* accepts */ "",
		/* acks  */ "grob-interface",
		/* reads */ "",
		/* write */ "");

/****************************************************************/

class Tab_staff_symbol_engraver : public Staff_symbol_engraver
{
public:
  TRANSLATOR_DECLARATIONS (Tab_staff_symbol_engraver);
protected:
  virtual void process_music ();
};

void
Tab_staff_symbol_engraver::process_music ()
{
  bool init = !span_;
  Staff_symbol_engraver::process_music ();
  if (init)
    {
      int k = scm_ilength (get_property ("stringTunings"));
      if (k>= 0)
	span_->set_property ("line-count", scm_int2num (k));
    }
}

Tab_staff_symbol_engraver::Tab_staff_symbol_engraver ()
{
}

ADD_TRANSLATOR (Tab_staff_symbol_engraver,
		/* descr */ "Create a staff-symbol, but look at stringTunings for the number of lines."
		"staff lines.",
		/* creats*/ "StaffSymbol",
		/* accepts */ "",
		/* acks  */ "grob-interface",
		/* reads */ "stringTunings",
		/* write */ "");
