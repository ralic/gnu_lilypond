\version "1.7.18"
\header {texidoc = "
if this test isn't covered in regression, we're in trouble.  :)  DELETE. "}

\score{
	\notes\relative c''{
		\slurUp
		a2 (a-)\break
		\slurDown
		a2 (a-)\break
		\slurUp
		c2 (c-)\break
		\slurDown
		c2 (c-)\break

	}
	\paper{
		raggedright = ##t
	}
}


