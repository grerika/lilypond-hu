/*   
  music-wrapper-iterator.cc --  implement Music_wrapper_iterator
  
  source file of the GNU LilyPond music typesetter
  
  (c) 1998--2000 Han-Wen Nienhuys <hanwen@cs.uu.nl>
  
 */


#include "music-wrapper-iterator.hh"
#include "music-wrapper.hh"

Music_wrapper_iterator::Music_wrapper_iterator ()
{
  child_iter_p_ =0;
}



void
Music_wrapper_iterator::do_print () const
{
  child_iter_p_->print ();
}

void
Music_wrapper_iterator::construct_children ()
{
  child_iter_p_ =
    get_iterator_p (dynamic_cast<Music_wrapper const*> (music_l_)->element_l ());
}

Music_wrapper_iterator::~Music_wrapper_iterator ()
{
  delete child_iter_p_;
}


bool
Music_wrapper_iterator::ok () const
{
  return child_iter_p_ && child_iter_p_->ok ();
}

void
Music_wrapper_iterator::do_process_and_next (Moment m)
{
  child_iter_p_->process_and_next (m);
  Music_iterator::do_process_and_next (m);
}


Moment
Music_wrapper_iterator::next_moment () const
{
  return child_iter_p_->next_moment ();
}


Music_iterator*
Music_wrapper_iterator::try_music_in_children (Music const *m) const
{
  return child_iter_p_->try_music (m);
}
