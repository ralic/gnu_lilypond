/*
  repeat-engraver.cc -- implement Repeat_engraver

  source file of the GNU LilyPond music typesetter

  (c) 1998--2000 Jan Nieuwenhuizen <janneke@gnu.org>
*/

#include "engraver.hh"
#include "cons.hh"
#include "bar.hh"
#include "bar-engraver.hh"
#include "musical-request.hh"
#include "multi-measure-rest.hh"
#include "command-request.hh"
#include "timing-translator.hh"
#include "engraver-group-engraver.hh"
#include "repeated-music.hh"
#include "timing-translator.hh"
#include "volta-spanner.hh"
#include "note-column.hh"
#include "paper-def.hh"
#include "music-list.hh"
#include "side-position-interface.hh"
#include "spanner.hh"
#include "note-column.hh"

struct Bar_create_event
{
  Moment when_;
  bool bar_b_;
  bool last_b_;
  String type_;
  Bar_create_event();
  Bar_create_event (Moment w, String s);
  Bar_create_event (Moment w, int i, int j);
};

int compare (Bar_create_event const & c1, Bar_create_event const &c2)
{
  return (c1.when_ - c2.when_).sign();
}

/**
  Generate repeat-bars |: :| for repeated-music
  */
class Repeat_engraver : public Engraver 
{
public:
  VIRTUAL_COPY_CONS(Translator);
  Repeat_engraver ();
protected:
  virtual void acknowledge_element (Score_element_info i);
  virtual void do_removal_processing ();
  virtual bool do_try_music (Music *req_l);
  virtual void do_process_music();
  virtual void do_pre_move_processing();
  virtual void do_post_move_processing ();
  void queue_events ();

private:
  Repeated_music *repeated_music_l_;
  bool done_this_one_b_;

  /*
    Royal_brackla_create_queue is only two Whiskies away. :-)
   */
  Cons<Bar_create_event> *create_barmoments_queue_;

  Spanner * volta_span_p_;
  Spanner* end_volta_span_p_;
};




ADD_THIS_TRANSLATOR (Repeat_engraver);

bool
Repeat_engraver::do_try_music (Music* m)
{
  if (Repeated_music* r = dynamic_cast<Repeated_music *> (m))
    {
      if (repeated_music_l_)
	return false;
      
      if (r->volta_fold_b_)
	{
	  repeated_music_l_ = r;
	}

      /*
	We acknowledge other types of unfolded music as well, to
	get auto context selection right.
       */
      if (r->type_ == "volta" || r->type_ == "unfolded")
	return true;

    }
  return false;
}

/**
 Walk through repeat music, and generate events for appropriate times.

 UGH. Should use Music_iteration for this.

 Should also queue some event to get timing information reset during
 2nd and following voltas.
*/
void
Repeat_engraver::queue_events ()
{
  Music_sequence* alt = repeated_music_l_->alternatives_p_;
  Moment walk_mom = now_mom () + repeated_music_l_->repeat_body_p_->length_mom ();

  SCM novolta = get_property ("noVoltaBraces");
  bool create_volta = !to_boolean (novolta);

  Cons_list<Bar_create_event> becel;
  becel.append (new Bar_create_event (now_mom (), "|:"));

  if (!alt)
    {
      becel.append  (new Bar_create_event (walk_mom, ":|"));
      becel.append  (new Bar_create_event (walk_mom, "stop"));
   }
  else
    {
      int last_number = 0;
      int volta_number = repeated_music_l_->repeats_i_ - alt->length_i () + 1;

      /*
	all repeat alternatives, and generate events with
	appropriate timestamps. The volta spanner event (a number string)
	happens at the begin of the alt. The :| bar event at the ending.
      */
      for (Cons<Music> *i = alt->music_p_list_p_->head_; i; i = i->next_)
	{

	  /*
	    some idiot might typeset a repeat not starting on a
	    barline.  Make sure there is one.

	    (todo: should try to avoid line breaks?)
	  */
	  if (last_number == 0)
	    {
	      becel.append (new Bar_create_event (walk_mom, ""));
	    }

	  
	  if (create_volta)
	    {
	      Bar_create_event * c = new Bar_create_event (walk_mom, last_number+ 1,
							   volta_number);
	      
	      if (!i->next_)
		c->last_b_ = true;
	      
	      becel.append (c);
	      last_number = volta_number;
	      volta_number ++;
              SCM l (get_property ("voltaSpannerDuration"));
              if (SMOB_IS_TYPE_B (Moment, l))
		{
		  Moment vSD_mom = *SMOB_TO_TYPE (Moment,l);
		  if ( vSD_mom < i->car_->length_mom() ) // terminate volta early ?
		    {
		      vSD_mom += walk_mom;
		      c->last_b_ = true;
		      becel.append (new Bar_create_event (vSD_mom, "stop"));
		    }
		}
	    }
	  walk_mom += i->car_->length_mom();

	  if (i->next_)
	    becel.append (new Bar_create_event (walk_mom, ":|"));
	  else
	    becel.append (new Bar_create_event (walk_mom, "stop"));
	}
    }

  Cons<Bar_create_event> *&tail = create_barmoments_queue_
    ? last_cons (create_barmoments_queue_)->next_
    : create_barmoments_queue_;

  tail = becel.head_ ;
  becel.head_ = 0;
}

void
Repeat_engraver::do_process_music ()
{
  if (repeated_music_l_ && !done_this_one_b_)
    { 
      queue_events ();
      done_this_one_b_ = true;
    }
  
  
  Cons<Bar_create_event> * head =   create_barmoments_queue_;
  if (!head)
    return;

  Bar_engraver* bar_engraver_l = dynamic_cast <Bar_engraver*>
    (daddy_grav_l ()->get_simple_translator ("Bar_engraver"));

  /*
    Do all the events that need to be done now.
  */
  while (head && now_mom () == head->car_->when_)
    {
      create_barmoments_queue_ = create_barmoments_queue_->next_;
      head->next_ =0;
      if (bar_engraver_l)
	{
	  String t = head->car_->type_;
	  if (head->car_->bar_b_)
	    {
	      if (t == "stop" || t == ":|")
		{
		  end_volta_span_p_ = volta_span_p_;
		  volta_span_p_ =0;
		}

	      if (t != "stop")
		bar_engraver_l->request_bar (t);
	      else
		bar_engraver_l->request_bar (""); 
	    }
	  else
	    {
	      assert (!volta_span_p_);
	      volta_span_p_ = new Spanner (get_property ("basicVoltaSpannerProperties"));
	      Volta_spanner::set_interface (volta_span_p_);
	      announce_element (Score_element_info (volta_span_p_,0));
	      volta_span_p_->set_elt_property ("text",
					       ly_str02scm (t.ch_C()));
	      volta_span_p_->set_elt_property ("last-volta",
					       gh_bool2scm (head->car_->last_b_));
	      // voltaSpannerDuration stuff here.
	      // other property stuff here.
	    }
	  
	}
      else
	{
	  warning (_ ("No bar engraver found.  Ignoring repeats."));
	}

      delete head->car_;
      delete head;

      head = create_barmoments_queue_;
    }

  assert (!head || head->car_->when_ > now_mom ());
}  


void
Repeat_engraver::acknowledge_element (Score_element_info i)
{
  if (Item* item = dynamic_cast<Item*> (i.elem_l_))
    {
      if (Note_column::has_interface (item))
	{
	  if (volta_span_p_)
	    Volta_spanner::add_column (volta_span_p_,item);
	  if (end_volta_span_p_)
	    Volta_spanner::add_column (end_volta_span_p_,item);      
	}
      if (Bar::has_interface (item))
	{
	  if (volta_span_p_)
	    Volta_spanner::add_bar (volta_span_p_, item);
	  if (end_volta_span_p_)
	    Volta_spanner::add_bar(end_volta_span_p_ , item);
	}
    }
}

void
Repeat_engraver::do_removal_processing ()
{
  if (volta_span_p_)
    {
      typeset_element(volta_span_p_);
    }
  if (end_volta_span_p_)
    {
      typeset_element (end_volta_span_p_);
    }
  // todo: the paranoid may also delete create_barmoments_queue_
}

void
Repeat_engraver::do_post_move_processing ()
{
  for (Cons<Bar_create_event> *p = create_barmoments_queue_;
       p && p->car_->when_ == now_mom (); p = p->next_)
    if (p->car_->type_ == "stop")
      {
	repeated_music_l_ = 0;
	done_this_one_b_ = false;
      }
}

void 
Repeat_engraver::do_pre_move_processing ()
{
  if (end_volta_span_p_)
    {
      Side_position::add_staff_support (end_volta_span_p_);
      
      typeset_element (end_volta_span_p_ );
      end_volta_span_p_ =0;
    }
    
}


Repeat_engraver::Repeat_engraver()
{
  repeated_music_l_ =0;
  end_volta_span_p_ =0;
  volta_span_p_ =0;
  done_this_one_b_ = false;
  create_barmoments_queue_ =0;
}
				 
/* ************** */
Bar_create_event::Bar_create_event()
{
  last_b_ =false;
  bar_b_ = true;
}

Bar_create_event::Bar_create_event (Moment w, String s)
{
  last_b_ =false;
  when_ = w;
  type_ = s;
  bar_b_ = true;
}

Bar_create_event::Bar_create_event (Moment w, int i, int j)
{
  last_b_ =false;
  when_ = w ;
  bar_b_ = false;

  if (i!=j)
    type_ = to_str (i) + ".-" ;

  type_ += to_str(j) + ".";
}
