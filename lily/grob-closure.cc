#include "grob.hh"
#include "simple-closure.hh"
#include "unpure-pure-container.hh"
#include "lily-imports.hh"

SCM
axis_offset_symbol (Axis a)
{
  return a == X_AXIS
         ? ly_symbol2scm ("X-offset")
         : ly_symbol2scm ("Y-offset");
}

SCM
axis_parent_positioning (Axis a)
{
  return (a == X_AXIS)
         ? Grob::x_parent_positioning_proc
         : Grob::y_parent_positioning_proc;
}

/*
  Replace

  (orig-proc GROB)

  by

  (+ (PROC GROB) (orig-proc GROB))
*/
void
add_offset_callback (Grob *g, SCM proc, Axis a)
{
  SCM data = g->get_property_data (axis_offset_symbol (a));
  if (!scm_is_number (data)
      && !ly_is_procedure (data)
      && !unsmob<Simple_closure> (data))
    {
      g->set_property (axis_offset_symbol (a), proc);
      return;
    }

  if (ly_is_procedure (data) || unsmob<Unpure_pure_container> (data))
    data = Simple_closure::make_smob (scm_list_1 (data));
  else if (Simple_closure *sc = unsmob<Simple_closure> (data))
    data = sc->expression ();

  if (ly_is_procedure (proc))
    proc = Simple_closure::make_smob (scm_list_1 (proc));

  SCM expr = scm_list_3 (Guile_user::plus, proc, data);
  g->set_property (axis_offset_symbol (a), Simple_closure::make_smob (expr));
}

/*
  replace

  (orig-proc GROB)

  by

  (PROC GROB (orig-proc GROB))
*/
void
chain_callback (Grob *g, SCM proc, SCM sym)
{
  SCM data = g->get_property_data (sym);

  if (ly_is_procedure (data) || unsmob<Unpure_pure_container> (data))
    data = Simple_closure::make_smob (scm_list_1 (data));
  else if (Simple_closure *sc = unsmob<Simple_closure> (data))
    data = sc->expression ();
  else
    /*
      Data may be nonnumber. In that case, it is assumed to be
      undefined.
    */

    data = SCM_UNDEFINED;

  SCM expr = scm_list_2 (proc, data);
  g->set_property (sym,

                   // twice: one as a wrapper for grob property routines,
                   // once for the actual delayed binding.
                   Simple_closure::make_smob (Simple_closure::make_smob (expr)));
}

void
chain_offset_callback (Grob *g, SCM proc, Axis a)
{
  chain_callback (g, proc, axis_offset_symbol (a));
}
