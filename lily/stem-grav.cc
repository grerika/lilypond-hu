/*
  stem-grav.cc -- implement Stem_engraver

  source file of the GNU LilyPond music typesetter

  (c) 1997 Han-Wen Nienhuys <hanwen@stack.nl>
*/

#include "stem-grav.hh"
#include "note-head.hh"
#include "stem.hh"
#include "musical-request.hh"
#include "duration-convert.hh"
#include "misc.hh"

Stem_engraver::Stem_engraver()
{
  abbrev_req_l_ = 0;
  stem_p_ = 0;
  dir_ = CENTER;
}

void
Stem_engraver::acknowledge_element(Score_elem_info i)
{
  if (i.elem_l_->is_type_b (Note_head::static_name()))
    {
      Note_head *h  = (Note_head*) i.elem_l_->item();
      if (!stem_p_) 
	{
	  Rhythmic_req * r = i.req_l_->musical()->rhythmic();
	  stem_p_ = new Stem;
	  int durlog_i = r->duration_.durlog_i_;
	  stem_p_->flag_i_ = Duration_convert::type2_i(durlog_i);
	  if (abbrev_req_l_)
	    stem_p_->abbrev_flag_i_ = intlog2 (abbrev_req_l_->type_i_) 
	      - (durlog_i>? 2);
	  announce_element (Score_elem_info (stem_p_, r));
	}
      stem_p_->add (h);
    }
}

void
Stem_engraver::do_pre_move_processing()
{
  if (stem_p_)
    {
      if (dir_)
	stem_p_->dir_ = dir_;

      typeset_element(stem_p_);
      stem_p_ = 0;
    }
  abbrev_req_l_ = 0;
}

bool
Stem_engraver::do_try_request (Request* r)
{
  Musical_req* mus_l = r->musical ();
  if (!mus_l)
    return false;
  
  Abbreviation_req* a = mus_l->abbrev ();
  if (!a)
    return false;

  abbrev_req_l_ = a;

  return true;
}

void
Stem_engraver::set_feature (Feature i)
{
  if (i.type_ == "vdir")	
    dir_ = (Direction) int(i.value_);
}

IMPLEMENT_IS_TYPE_B1(Stem_engraver, Engraver);
ADD_THIS_ENGRAVER(Stem_engraver);
