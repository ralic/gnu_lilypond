/*
  item.hh -- declare Item

  source file of the GNU LilyPond music typesetter

  (c) 1997 Han-Wen Nienhuys <hanwen@stack.nl>
*/
#ifndef ITEM_HH
#define ITEM_HH

#include "glob.hh"
#include "boxes.hh"
#include "string.hh"
#include "score-elem.hh"

/**
 a horizontally fixed size element of the score

  Item is the datastructure for printables whose width is known
  before the spacing is calculated

  NB. This doesn't mean an Item has to initialize the output field before
  spacing calculation. 
  
*/
class Item : public virtual Score_elem {
public:
    /// indirection to the column it is in
    PCol * pcol_l_;

    virtual Item *item() { return this; }
    Item();
    Real hpos_f() const;
    NAME_MEMBERS(Item);
    virtual Line_of_score * line_l() const;
protected:
    virtual void do_print()const;

};



#endif
