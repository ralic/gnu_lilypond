\version "2.1.36"
\header{
texidoc = "Concurrent tuplets should be equidistant on all staffs.

Note that this only spaces correctly (exactly) when @code{raggedright}
is enabled. For a non-@code{raggedright} case, it still shows a bug: 
uneven spacing. 
"  }



multipart = \notes \relative c'{ 
    \context StaffGroup << 
                \new Staff  \context Voice { 
                   \times 2/10 {  c8[ c c c c c c c c c] } 
                   \times 2/10 {  c[  c c c c c c c c c] } 
		}
                \new Staff  \context Voice { 
                   \times 2/11 {  c8[ c c c c c c c c c c] } 
                   \times 2/11 {  c[  c c c c c c c c c c] } 
                }
            >>
	}
    
\score{
    \notes { 
	\multipart 
    }

%    \paper { raggedright = ##t }

}


