#(ly:set-option 'old-relative)
\version "1.9.0"
\header {

texidoc = "Scripts can also be attached to chord elements."

}

\score {
    \notes \relative c''{
	<< c-.  >> 4
	<< c->  >> 4
	<< c-. e-. g-. b-. >>  
	<< c^^  >> 4
	<< c_^  >> 4
	<< c_^ e^^ >> 4
	<< c_^ e^^ >> 4


    }

}

