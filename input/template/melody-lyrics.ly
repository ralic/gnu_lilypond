\version "2.1.19"

\header {
  texidoc = "Melody and lyrics."
}

melody = \notes \relative c'' {
  a b c d
}

text = \lyrics {
  Aaa Bee Cee Dee
}

\score {
  <<
      \context Voice = one {
	  \property Staff.autoBeaming = ##f
	  \melody
      }
      \lyricsto "one" \new LyricsVoice \text
  >>
  \paper { }
  \midi  { }
}
