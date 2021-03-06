/*
  This file is part of LilyPond, the GNU music typesetter.

  Copyright (C) 2005--2015 Han-Wen Nienhuys <hanwen@xs4all.nl>

  LilyPond is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  LilyPond is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with LilyPond.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SIMPLE_CLOSURE_HH
#define SIMPLE_CLOSURE_HH

#include "lily-guile.hh"
#include "small-smobs.hh"

class Simple_closure : public Smob1<Simple_closure>
{
public:
  SCM expression() const { return scm1 (); }
  int print_smob (SCM, scm_print_state *) const;
  static const char type_p_name_[];
};

SCM evaluate_with_simple_closure (SCM delayed_argument, SCM expr, bool pure, int start, int end);

#endif /* SIMPLE_CLOSURE_HH */
