#(ly:set-option 'old-relative)
\header {
 texidoc = "The number of beams doesn't change on a rest."
}

\version "1.9.0"

\score { \notes \relative c, {
  \clef bass	 
   e8-[ r16 f d'8 e16 e16]  c8-[ c16 r16 bes8 r16 c16-] |
 }
\paper { raggedright = ##t}
}
