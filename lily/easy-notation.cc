/*
  easy-notation.cc --  implement easy notation heads

  source file of the GNU LilyPond music typesetter

  (c) 2005 Han-Wen Nienhuys <hanwen@xs4all.nl>

*/

#include "note-head.hh"

#include "text-item.hh"
#include "grob.hh"
#include "output-def.hh"
#include "music.hh"
#include "pitch.hh"
#include "font-interface.hh"
#include "staff-symbol-referencer.hh"
#include "stem.hh"

MAKE_SCHEME_CALLBACK (Note_head, brew_ez_stencil, 1);
SCM
Note_head::brew_ez_stencil (SCM smob)
{
  Grob *me = unsmob_grob (smob);
  int log = Note_head::get_balltype (me);

  SCM cause = me->get_property ("cause");
  SCM spitch = unsmob_music (cause)->get_property ("pitch");
  Pitch *pit = unsmob_pitch (spitch);

  SCM idx = scm_int2num (pit->get_notename ());
  SCM names = me->get_property ("note-names");
  SCM charstr = SCM_EOL;
  if (scm_is_vector (names))
    charstr = scm_vector_ref (names, idx);
  else
    {
      char s[2] = "a";
      s[0] = (pit->get_notename () + 2) % 7 + 'a';
      s[0] = toupper (s[0]);
      charstr = scm_makfrom0str (s);
    }

  
  SCM letter
    = Text_interface::interpret_string (me->get_layout()->self_scm (),
					Font_interface::text_font_alist_chain (me),
					charstr);
  
  Stencil l (*unsmob_stencil (letter));
  l.align_to (X_AXIS, CENTER);
  l.align_to (Y_AXIS, CENTER);

  
  l = Stencil (Box (), l.expr ()); 
  Real ss = Staff_symbol_referencer::staff_space (me);
  Real lt = Staff_symbol_referencer::line_thickness (me);

  Real radius = (ss + lt) / 2.0;
  Real stem_thick = 1.3 * lt; 
  if (Grob *stem = unsmob_grob (me->get_property ("stem")))
    {
      stem_thick = Stem::thickness (stem);
    }

  int black = (log >= 2);

  Stencil head;
  Box extent (Interval (-radius, radius),
	      Interval (-radius, radius));
	      
  Stencil black_head (extent,
		      scm_list_4 (ly_symbol2scm ("circle"),
				  scm_from_double (radius),
				  scm_from_double (0.0),
				  SCM_BOOL_T));
  Stencil white_head;
  if (black)
    {
      l = l.in_color (1, 1, 1); 
    }
  else
    {
      white_head = Stencil (extent,
			    scm_list_4 (ly_symbol2scm ("circle"),
			       scm_from_double (radius - stem_thick),
			       scm_from_double (0.0),
			       SCM_BOOL_T));

      white_head = white_head.in_color (1, 1, 1);
    }
    

  Stencil total;
  total.add_stencil (l);
  total.add_stencil (white_head);
  total.add_stencil (black_head);
  total.translate_axis (radius, X_AXIS);

  return total.smobbed_copy ();
}

