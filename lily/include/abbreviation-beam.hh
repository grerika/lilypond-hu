/*
  abbreviation-beam-engraver.hh -- declare Abbreviation_beam_engraver

  source file of the GNU LilyPond music typesetter

  (c) 1997 Han-Wen Nienhuys <hanwen@stack.nl>
	   Jan Nieuwenhuizen <jan@digicash.com>
*/

#ifndef ABBREVIATION_BEAM_HH
#define ABBREVIATION_BEAM_HH

#include "beam.hh"

/** a beam connects multiple stems Beam adjusts the stems its owns to
  make sure that they reach the beam and that point in the correct
  direction */
class Abbreviation_beam : public Beam {
public:
  DECLARE_MY_RUNTIME_TYPEINFO;

  Abbreviation_beam();

  SCORE_ELEM_CLONE(Abbreviation_beam);

protected:
  virtual void do_print() const;
  virtual Molecule stem_beams (Stem *here, Stem *next, Stem *prev) const;
  virtual Molecule* brew_molecule_p() const;
};

#endif // ABBREVIATION_BEAM_HH

