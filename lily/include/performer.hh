/*
  performer.hh -- declare Performer

  (c) 1996, 1997 Han-Wen Nienhuys <hanwen@stack.nl>
                 Jan Nieuwenhuizen <jan@digicash.com>
 */

#ifndef PERFORMER_HH
#define PERFORMER_HH

#include "lily-proto.hh"
#include "varray.hh"
#include "request.hh"
#include "score-elem-info.hh"
#include "staff-info.hh"

class Performer {
    bool init_b_;
public:
    NAME_MEMBERS();
    Performer_group_performer* daddy_perf_l_;

    Performer();
    virtual ~Performer();

    
    void print() const;
    virtual void process_requests();

    virtual void set( Moment mom );
    virtual bool try_request( Request* req_l );

    virtual void do_removal_processing();
    void creation_processing();
protected:
    virtual void do_creation_processing();

    virtual bool do_try_request(Request*);
    virtual int get_tempo_i() const;
    virtual void do_print() const;
    virtual Moment get_mom() const;
    virtual void play_event( Midi_item* l );

};

#include "global-performers.hh"

#endif // PERFORMER_HH
