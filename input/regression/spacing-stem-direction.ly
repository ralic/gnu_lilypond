#(ly:set-option 'old-relative)
\version "1.9.0"

\header{

texidoc="

LilyPond corrects for optical spacing of stems. The overlap between to
adjacent stems of different direction is used as a measure for how
much to correct."

}

\score { 
  \context Voice \notes\relative c {
  % make sure neutral is down.
    \property Voice.Stem \override #'neutral-direction = #-1
	\time 16/4  c''4 c c,  c' d, c' e, c' f, c' g c a c b c
	
  }
  \paper {
    raggedright = ##t
  }  
}

