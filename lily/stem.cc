/*
  stem.cc -- implement Stem

  source file of the GNU LilyPond music typesetter

  (c) 1996,1997 Han-Wen Nienhuys <hanwen@stack.nl>
*/

#include "stem.hh"
#include "dimen.hh" 
#include "debug.hh"
#include "paper-def.hh"
#include "note-head.hh"
#include "lookup.hh"
#include "molecule.hh"
#include "p-col.hh"
#include "misc.hh"
#include "beam.hh"

const int STEMLEN=7;

IMPLEMENT_IS_TYPE_B1 (Stem,Item);

Stem::Stem () 
{
  /*
    TODO: staff-size
   */
  abbrev_flag_i_ = 0;
  beam_l_ = 0;
  beams_left_i_ = 0;
  beams_right_i_ = 0;

  stem_bottom_f_ = stem_top_f_ = 0;
  flag_i_ = 4;
  dir_ =CENTER;
  staff_size_i_ = 8;

  stem_xoffset_f_ =0;
}

int
Stem::min_head_i () const
{
  int m = 1000;
  for (int i =0; i < head_l_arr_.size (); i++)
    m = m <? head_l_arr_[i]->position_i_;
  return m;
}

int
Stem::max_head_i () const
{
  int m = -1000;
  for (int i =0; i < head_l_arr_.size (); i++)
    m = m >? head_l_arr_[i]->position_i_;
  return m;
  
}

void
Stem::do_print () const
{
#ifndef NPRINT
  DOUT << "flag "<< flag_i_ << " print_flag? " << !(bool)beam_l_ << "abbrev_flag_i_" << abbrev_flag_i_;
#endif
}

Real 
Stem::stem_length_f () const
{
  return stem_top_f_-stem_bottom_f_ ;
}

Real
Stem::stem_start_f () const
{
  return (dir_ < 0)? stem_top_f_ : stem_bottom_f_;
}

Real
Stem::stem_end_f () const
{
  return (dir_ < 0)? stem_bottom_f_ : stem_top_f_;
}


void
Stem::set_stemend (Real se)
{
  // todo: margins
  if (!  ((dir_ > 0 && se >= max_head_i ()) || 
	  (se <= min_head_i () && dir_ <0)))	
    warning ("Weird stem size; check for narrow beams");

  stem_top_f_  = (dir_ < 0) ? max_head_i () : se;
  stem_bottom_f_  = (dir_ < 0) ? se  : min_head_i ();
}

void
Stem::add (Note_head *n)
{
  n->add_dependency (this);
  if (n->rest_b_) 
      rest_l_arr_.push (n);
  else 
      head_l_arr_.push (n);

#if 0
  else if (1) //why different? (n->balltype_i_) 
      head_l_arr_.push (n);
  else
      whole_l_arr_.push (n);
#endif
}

bool
Stem::invisible_b () const
{
  return !head_l_arr_.size();
}

// if dir_ is set we return fake values.

int
Stem::get_center_distance_from_top ()
{
  if (dir_)
    return (dir_ > 0) ? 0 : 1;

  int staff_center = staff_size_i_ / 2;
  int max = max_head_i () - staff_center;
  return max >? 0;
}

// if dir_ is set we return fake values.
int
Stem::get_center_distance_from_bottom ()
{
  if (dir_)
    return (dir_ > 0) ? 1 : 0;

  int staff_center = staff_size_i_ / 2;
  int min = staff_center - min_head_i ();
  return min >? 0;
}

Direction
Stem::get_default_dir ()
{
  if (dir_)
    return dir_;
  return (get_center_distance_from_top () >=
    get_center_distance_from_bottom ()) ? 
    (Direction)-1 : (Direction)1;
}


void
Stem::set_default_dir ()
{
  dir_ = get_default_dir ();
}

void
Stem::set_default_stemlen ()
{
  if (!dir_)
    set_default_dir ();

  // ugh... how about non 5-line staffs?
  bool on_ledger_line_b = ((max_head_i () < -2 && dir_ == 1)
//    || (min_head_i () > staff_size_i_ && dir_ == -1));
    || (min_head_i () > staff_size_i_ + 3 && dir_ == -1));
  if (on_ledger_line_b)
    {
      set_stemend (staff_size_i_ / 2 - 1);
    }
  else 
    {
      Real dy = paper ()->interbeam_f ();
      Real len = STEMLEN;
      // ugh, should get nice *rule* for this
      if (abbrev_flag_i_ > 1)
	len += (abbrev_flag_i_ - 1)* dy / 2;
      set_stemend ((dir_ > 0) ? max_head_i () + len :
		   min_head_i () - len);
    }
}

void
Stem::set_default_extents ()
{
  if (!stem_length_f ())
    set_default_stemlen ();

  set_stemend ((dir_< 0) ? 
	       max_head_i ()-stem_length_f (): min_head_i () +stem_length_f ());
  if (dir_ > 0){	
    stem_xoffset_f_ = paper ()->note_width ()-paper ()->rule_thickness ();
  }
  else
    stem_xoffset_f_ = 0;
}

/*
  TODO
  
  move into note_column.cc

  */
void
Stem::set_noteheads ()
{
  if (!head_l_arr_.size ())
    return;
  head_l_arr_.sort (Note_head::compare);
  if (dir_ < 0) 
    head_l_arr_.reverse ();
  
  head_l_arr_[0]->extremal_i_ = -1;
  head_l_arr_.top ()->extremal_i_ = 1;
  int parity=1;
  int lastpos = head_l_arr_[0]->position_i_;
  for (int i=1; i < head_l_arr_.size (); i ++) 
    {
      int dy =abs (lastpos- head_l_arr_[i]->position_i_);
	
      if (dy <= 1) 
	{
	  if (parity)
	    head_l_arr_[i]->x_dir_ = (stem_xoffset_f_>0) ? UP:DOWN;
	  parity = !parity;
	}
      else
	parity = 0;
      lastpos = head_l_arr_[i]->position_i_;
    }
}

void
Stem::do_pre_processing ()
{
  if (stem_bottom_f_== stem_top_f_)
    set_default_extents ();
  set_noteheads ();
  flag_i_ = dir_*abs (flag_i_);
  transparent_b_ = invisible_b ();
  empty_b_ = invisible_b ();
}

Interval
Stem::do_width () const
{
  if (beam_l_ || abs (flag_i_) <= 4)
    return Interval (0,0);	// TODO!
  Paper_def*p= paper ();
  Interval r (p->lookup_l ()->flag (flag_i_).dim.x ());
  r+= stem_xoffset_f_;
  return r;
}

Molecule
Stem::abbrev_mol () const
{
  Real dy = paper ()->interbeam_f ();
  Real w = 1.5 * paper ()->lookup_l ()->ball (2).dim.x ().length ();
  Real beamdy = paper ()->interline_f () / 2;

  int beams_i = 0;
  Real slope = paper ()->internote_f () / 4;

  if (beam_l_) {
    // huh?
    slope = 2 * beam_l_->slope;
    // ugh, rather calc from Abbreviation_req
    beams_i = beams_right_i_ >? beams_left_i_; 
  }
  paper()->lookup_l ()->beam (slope, 20 PT);

  Molecule beams;
  Atom a (paper ()->lookup_l ()->beam (slope, w));
  a.translate (Offset(- w / 2, stem_end_f () - (w / 2 * slope)));
  // ugh
  if (!beams_i)
    a.translate (dy + beamdy - dir_ * dy, Y_AXIS);
  else
    a.translate (2 * beamdy - dir_ * (beamdy - dy), Y_AXIS);
  	
  for (int i = 0; i < abbrev_flag_i_; i++) 
    {
      Atom b (a);
      b.translate (-dir_ * dy * (beams_i + i), Y_AXIS);
      beams.add (b);
    }

  return beams;
}

Molecule*
Stem::brew_molecule_p () const 
{
  Real bot  = stem_bottom_f_;
  Real top = stem_top_f_;
  
  assert (bot!=top);
 
  Paper_def *p =paper ();

  Real dy = p->internote_f ();
  Symbol ss =p->lookup_l ()->stem (bot*dy,top*dy);

  Molecule* mol_p = new Molecule;
  if (head_l_arr_.size() && head_l_arr_[0]->balltype_i_)
    mol_p->add (Atom (ss));

  if (!beam_l_ && abs (flag_i_) > 4)
    {
      Symbol fl = p->lookup_l ()->flag (flag_i_);
      Molecule m (fl);
      if (flag_i_ < -4){		
	mol_p->add_bottom (m);
      }
      else if (flag_i_ > 4) 
	{
	  mol_p->add_top (m);
	}
      else
	assert (false); 
      assert (!abbrev_flag_i_);
    }

  if (abbrev_flag_i_)
    mol_p->add (abbrev_mol ());

  mol_p->translate (stem_xoffset_f_, X_AXIS);
  return mol_p;
}

Real
Stem::hpos_f () const
{
  return Item::hpos_f () + stem_xoffset_f_;
}


void
Stem::do_substitute_dependency (Score_elem*o,Score_elem*n)
{
  Item * o_l = o->item ();
  Item * n_l = n? n->item ():0;
//  whole_l_arr_.substitute ((Note_head*)o_l, (Note_head*)n_l);
  head_l_arr_.substitute ((Note_head*)o_l, (Note_head*)n_l);
  rest_l_arr_.substitute ((Note_head*)o_l, (Note_head*)n_l);
}
