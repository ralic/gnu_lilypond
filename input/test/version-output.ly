\version "2.1.19"

\header {


    texidoc = #(string-append "By putting the output of
    @code{lilypond-version} into a lyric, we can print the version
    number in a score, or a lilypond-book document. Another option is
    to append the version to the doc-string, like this: "

  (lilypond-version)
  )
    
}

\score { \context LyricsVoice \notes {
    \property Score.RehearsalMark \set #'self-alignment-X = #LEFT
    \mark #(ly:export (string-append "Processed with LilyPond version " (lilypond-version)))
    s2
  }
}


