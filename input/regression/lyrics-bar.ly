\version "2.1.19"

\header{
texidoc="
Adding a @code{Bar_engraver} to the LyricsVoice context makes sure that
lyrics don't collide with barlines.
"
}

\score {
	\context StaffGroup <<
	\notes \context Staff {
	        b1 \bar "|:" b1 \bar ":|"
	}
	\lyrics <<
	 	\context LyricsVoiceWithBars {
%		        thisContextHasBarEngraver1  added
		        ThisContextCertainlyHasBarEngraverAddedButThereHasBeenSomethingFunnyBefore1.  Here.
		}
		\context LyricsVoice {
		        this4 one has no BarEngraverAddedToContext1
		}
	>>
	\notes \new Staff { b1 b1 }
	>>
	\paper {
		raggedright = ##t
		\translator {
			\ScoreContext
			\accepts "LyricsVoiceWithBars"
		}
		\translator {
			\LyricsVoiceContext
			\consists "Bar_engraver"
			\name "LyricsVoiceWithBars"
		}
	}
}

