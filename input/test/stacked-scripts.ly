\version "1.7.18"

\header { texidoc = "@cindex Stacked Text Articulation
You can stack text and articulations. "
}

\score {
	\notes {
		c2_"geen gedonder"_"jij gaat onder"
		c2^^^.^|^"down"^"up"
	 }
	\paper { raggedright = ##t}
}

