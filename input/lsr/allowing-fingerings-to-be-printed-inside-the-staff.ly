%% Do not edit this file; it is auto-generated from LSR http://lsr.dsi.unimi.it
%% This file is in the public domain.
\version "2.11.38"

\header {
  lsrtags = "editorial-and-educational-use, spacing"
 texidoc = "
By default, fingering numbers will be printed outside the staff.
However, this behavior can be canceled.
" }
% begin verbatim
\relative c' {
  <c-1 e-2 g-3 b-5>2
  \once \override Fingering  #'staff-padding = #'()
  <c-1 e-2 g-3 b-5>
}
