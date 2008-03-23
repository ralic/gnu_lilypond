%% Do not edit this file; it is auto-generated from LSR http://lsr.dsi.unimi.it
%% This file is in the public domain.
\version "2.11.38"

\header {
  lsrtags = "rhythms, tweaks-and-overrides"
 texidoc = "
The default behaviour of tuplet-bracket visibility is to print a
bracket unless there is a beam of the same length as the tuplet. To
control the visibility of tuplet brackets, you can set the property
@code{TupletBracket #'bracket-visibility} to either @code{##t} (always
print a bracket), @code{##f} (never print a bracket) or
@code{#'if-no-beam} (only print a bracket if there is no beam). 
" }
% begin verbatim
mus = \relative c'' {
  \times 2/3 { c16[ d e } f8]
  \times 2/3 { c8 d e }
  \times 2/3 { c4 d e }
}

\new Voice \relative c'{
  << \mus s4^"default" >>
  \override TupletBracket #'bracket-visibility = #'if-no-beam
  << \mus s4^"'if-no-beam" >>
  \override TupletBracket #'bracket-visibility = ##t
  << \mus s4^"#t" >>
  \override TupletBracket #'bracket-visibility = ##f
  << \mus s4^"#f" >>
} 

