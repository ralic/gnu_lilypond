\version "1.9.0"

\header {
texidoc = "Crescendi may start off-notes. In that case, they should  not collapse into flat lines."
}

\score { \notes { \context  Voice { 
  < f''1 { s4 s4 \< s4-\! \> s4-\! } >
}}

\paper { raggedright = ##t}
}

