/*
  chord-name-engraver.cc -- implement Chord_name_engraver

  source file of the GNU LilyPond music typesetter

  (c) 1998--2000 Jan Nieuwenhuizen <janneke@gnu.org>
*/

#include "engraver.hh"
#include "chord-name.hh"
#include "chord.hh"
#include "musical-request.hh"
#include "paper-def.hh"
#include "font-interface.hh"
#include "paper-def.hh"
#include "main.hh"
#include "dimensions.hh"
#include "item.hh"
#include "pitch.hh"
#include "protected-scm.hh"

class Chord_name_engraver : public Engraver 
{
public:
  Chord_name_engraver ();
  VIRTUAL_COPY_CONS (Translator);

protected:
  virtual void do_pre_move_processing ();
  virtual void acknowledge_element (Score_element_info i);
  virtual void do_process_music ();
  virtual bool do_try_music (Music *);

private:
  void add_note (Note_req *);
  
  Item* chord_name_p_;
  Protected_scm chord_;
  Protected_scm last_chord_;
};

ADD_THIS_TRANSLATOR (Chord_name_engraver);

Chord_name_engraver::Chord_name_engraver ()
{
  chord_name_p_ = 0;
  chord_ = gh_cons (SCM_EOL, gh_cons (SCM_EOL, SCM_EOL));
  last_chord_ = gh_cons (SCM_EOL, gh_cons (SCM_EOL, SCM_EOL));
}

void
Chord_name_engraver::add_note (Note_req* n)
{
  SCM pitches = gh_car (chord_);
  SCM modifiers = gh_cdr (chord_);
  SCM inversion = modifiers == SCM_EOL ? SCM_EOL : gh_car (modifiers);
  SCM bass = modifiers == SCM_EOL ? SCM_EOL : gh_cdr (modifiers);
  
  if (n->get_mus_property ("inversion") == SCM_BOOL_T)
    inversion = n->get_mus_property ("pitch");
  else if (n->get_mus_property ("bass") == SCM_BOOL_T)
    bass = n->get_mus_property ("pitch");
  else
    pitches = scm_sort_list (gh_cons (n->get_mus_property ("pitch"), pitches),
			     Pitch::less_p_proc);
  chord_ = gh_cons (pitches, gh_cons (inversion, bass));
}

bool
Chord_name_engraver::do_try_music (Music* m)
{
  if (Note_req* n = dynamic_cast<Note_req*> (m))
    {
      add_note (n);
      return true;
    }
  return false;
}

/* Uh, if we do acknowledge_element, shouldn't we postpone
   do_process_music until do_process_acknowlegded?

   Sigh, I can *never* remember how this works, can't we
   possibly-please just number these functions:

     do_creation0
     
     post_move1
     do_try_music2
     do_process_music3 (or is it acknowledge_element3 ?)
     acknowledge_element4
  
     do_pre_move9
     
     do_removal99

  and what was the deal with this ``do'' prefix again? */
void
Chord_name_engraver::acknowledge_element (Score_element_info i)
{
  if (Note_req* n = dynamic_cast<Note_req*> (i.req_l_))
    add_note (n);
}

void
Chord_name_engraver::do_process_music ()
{
  if (!chord_name_p_ && gh_car (chord_) != SCM_EOL)
    {
#if 0
      bool find_inversion_b = false;
      SCM chord_inversion = get_property ("chordInversion");
      if (gh_boolean_p (chord_inversion))
	find_inversion_b = gh_scm2bool (chord_inversion);

      chord_ = Chord::pitches_and_requests_to_chord (pitches_,
						     inversion_,
						     bass_,
						     find_inversion_b);

#endif
      
      chord_name_p_ = new Item (get_property ("ChordName"));
      chord_name_p_->set_elt_property ("chord", chord_);
      announce_element (chord_name_p_, 0);
      SCM s = get_property ("drarnChords"); //FIXME!
      if (to_boolean (s) && last_chord_ != SCM_EOL &&
	  gh_equal_p (chord_, last_chord_))
	chord_name_p_->set_elt_property ("begin-of-line-visible", SCM_BOOL_T);
    }
}

void
Chord_name_engraver::do_pre_move_processing ()
{
  if (chord_name_p_)
    {
      typeset_element (chord_name_p_);
    }
  chord_name_p_ = 0;

  last_chord_ = chord_;
  chord_ = gh_cons (SCM_EOL, gh_cons (SCM_EOL, SCM_EOL));
}

