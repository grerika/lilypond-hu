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

#include "grob-array.hh"
#include "item.hh"
#include "spanner.hh"


Item *
Grob_array::item (vsize i)
{
  return dynamic_cast<Item *> (grobs_.at (i));
}

Spanner *
Grob_array::spanner (vsize i)
{
  return dynamic_cast<Spanner *> (grobs_.at (i));
}

Grob_array::Grob_array ()
{
  ordered_ = true;
}

vector<Grob *> &
Grob_array::array_reference ()
{
  return grobs_;
}

vector<Grob *> const &
Grob_array::array () const
{
  return grobs_;
}

SCM
Grob_array::mark_smob () const
{
#if 0  /* see System::derived_mark () const */
  for (vsize i = 0; i < grobs_.size (); i++)
    scm_gc_mark (grobs_[i]->self_scm ());
#endif
  return SCM_UNDEFINED;
}

int
Grob_array::print_smob (SCM port, scm_print_state *) const
{
  scm_puts ("#<Grob_array", port);
  for (vsize i = 0; i < size (); i++)
    {
      scm_display (grob (i)->self_scm (), port);
      scm_puts (" ", port);
    }
  scm_puts (">", port);
  return 1;
}

SCM
Grob_array::make_array ()
{
  Grob_array ga;
  return ga.smobbed_copy ();
}

void
Grob_array::clear ()
{
  grobs_.clear ();
}

void
Grob_array::remove_duplicates ()
{
  assert (!ordered_);

  uniquify (grobs_);
}

bool
Grob_array::empty () const
{
  return grobs_.empty ();
}

void
Grob_array::set_array (vector<Grob *> const &src)
{
  grobs_ = src;
}

const char Grob_array::type_p_name_[] = "ly:grob-array?";


SCM
grob_list_to_grob_array (SCM lst)
{
  SCM arr_scm = Grob_array::make_array ();
  Grob_array *ga = unsmob<Grob_array> (arr_scm);
  for (SCM s = lst; scm_is_pair (s); s = scm_cdr (s))
    ga->add (unsmob<Grob> (scm_car (s)));
  return arr_scm;
}

SCM
grob_array_to_list (Grob_array *array)
{
  SCM list = SCM_EOL;
  SCM *tail = &list;

  for (vsize i = 0; i < array->size (); i++)
    {
      *tail = scm_cons (array->grob (i)->self_scm (), SCM_EOL);
      tail = SCM_CDRLOC (*tail);
    }
  return list;
}
