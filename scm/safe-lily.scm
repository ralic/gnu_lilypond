(define safe-objects
  
  (map
   (lambda (sym)
     (cons sym (primitive-eval sym))) 
   '(ly:add-interface
     ly:add-moment
     ly:all-grob-interfaces
     ly:bracket
     ly:context-find
     ly:context-id
     ly:context-name
     ly:context-parent
     ly:context-property
     ly:context-property-where-defined
     ly:context-pushpop-property
     ly:context-set-property!
     ly:context-unset-property
     ly:context?
     ly:dimension?
     ly:dir?
     ly:div-moment
     ly:duration-dot-count
     ly:duration-factor
     ly:duration-log
     ly:duration<?
     ly:duration?
     ly:export
     ly:find-glyph-by-name
     ly:font-design-size
     ly:font-encoding-alist
     ly:font-filename
     ly:font-magnification
     ly:font-metric?
     ly:font-name
     ly:format-output
     ly:get-all-function-documentation
     ly:get-all-translators
     ly:get-default-font
     ly:get-glyph
     ly:get-option
     ly:grob-alist-chain
     ly:grob-extent
     ly:grob-original
     ly:grob-paper
     ly:grob-parent
     ly:grob-pq-less?
     ly:grob-property
     ly:grob-script-priority-less
     ly:grob-set-property!
     ly:grob-staff-position
     ly:grob-suicide
     ly:grob-system
     ly:grob-translate-axis!
     ly:grob?
     ly:input-location
     ly:input-location?
     ly:input-message
     ly:intlog2
     ly:item-break-dir
     ly:item?
     ly:iterator?
     ly:make-bare-music
     ly:make-duration
     ly:make-moment
     ly:make-music-function
     ly:make-pitch
     ly:make-stencil
     ly:moment<?
     ly:moment?
     ly:mul-moment
     ly:music-compress
     ly:music-deep-copy
     ly:music-duration-compress
     ly:music-duration-length
     ly:music-length
     ly:music-list?
     ly:music-mutable-properties
     ly:music-name
     ly:music-property
     ly:music-scorify
     ly:music-set-property!
     ly:music-transpose
     ly:music?
     ly:my-lily-parser?
     ly:number->string
     ly:option-usage
     ly:output-def-clone
     ly:output-def-scope
     ly:output-description
     ly:page-header-lines-footer-stencil
     ly:page-paper-lines
     ly:page?
     ly:paper-book?
     ly:paper-def?
     ly:paper-get-font
     ly:paper-get-number
     ly:paper-line-break-penalty
     ly:paper-line-extent
     ly:paper-line-number
     ly:paper-line-stencil
     ly:paper-line?
     ly:output-def-lookup
     ly:parse-string
     ly:parser-parse-string
     ly:parser-print-book
     ly:parser-print-score
     ly:pitch-alteration
     ly:pitch-diff
     ly:pitch-notename
     ly:pitch-octave
     ly:pitch-quartertones
     ly:pitch-semitones
     ly:pitch-steps
     ly:pitch-transpose
     ly:pitch<?
     ly:pitch?
     ly:ragged-page-breaks
     ly:round-filled-box
     ly:run-translator
     ly:set-option
     ly:score-bookify
     ly:set-point-and-click
     ly:spanner-broken-into
     ly:spanner-get-bound
     ly:spanner?
     ly:stencil-add
     ly:stencil-align-to!
     ly:stencil-combine-at-edge
     ly:stencil-expr
     ly:stencil-extent
     ly:stencil-moved-to-edge
     ly:stencil-set-extent!
     ly:stencil-translate
     ly:stencil-translate-axis
     ly:stencil?
     ly:text-dimension
     ly:translator-description
     ly:translator-name
     ly:translator?
     ly:transpose-key-alist
     ly:unit
     ly:version
     ly:warn

     ;; need these for parsing init files:
     ;; todo: should have a macro define-safe-public 
     DOUBLE-FLAT 
     THREE-Q-FLAT 
     FLAT 
     SEMI-FLAT 
     NATURAL 
     SEMI-SHARP 
     SHARP 
     THREE-Q-SHARP
     DOUBLE-SHARP
     SEMI-TONE
     START
     STOP
     alist->hash-table
     default-chord-modifier-list
     sequential-music-to-chord-exceptions
     make-music
     make-articulation
     make-event-chord
     make-penalty-music
     make-span-event
     set-start-grace-properties
     set-stop-grace-properties
     set-default-paper-size
     set-part-combine-listener
     context-spec-music
     make-voice-props-revert
     make-voice-props-set
     chord-name->german-markup
     note-name->german-markup
     default-toplevel-music-handler
     default-toplevel-book-handler
     default-toplevel-score-handler

     Accidental_interface::after_line_breaking
     Accidental_interface::print
     Ambitus::print
     Arpeggio::print
     Arpeggio::brew_chord_bracket
     Arpeggio::width_callback
     Axis_group_interface::group_extent_callback
     Bar_line::before_line_breaking
     Bar_line::get_staff_bar_size
     Bar_line::print
     Beam::after_line_breaking
     Beam::before_line_breaking
     Beam::check_concave
     Beam::least_squares
     Beam::print
     Beam::quanting
     Beam::shift_region_to_valid
     Beam::slope_damping
     Beam::space_function
     Break_align_interface::alignment_callback
     Breathing_sign::offset_callback
     Chord_name::after_line_breaking
     Clef::before_line_breaking
     Clef::print
     Cluster::print
     Cluster_beacon::height
     Custos::print
     Dot_column::side_position
     Dots::print
     Hairpin::print
     Hara_kiri_group_spanner::force_hara_kiri_callback
     Hara_kiri_group_spanner::y_extent
     Horizontal_bracket::print
     Hyphen_spanner::print
     Hyphen_spanner::set_spacing_rods
     Key_signature_interface::print
     Line_spanner::after_line_breaking
     Line_spanner::print
     Lyric_extender::print
     Measure_grouping::print
     Mensural_ligature::brew_ligature_primitive
     Mensural_ligature::print
     Multi_measure_rest::percent
     Multi_measure_rest::print
     Multi_measure_rest::set_spacing_rods
     Note_head::extent
     Note_head::print
     Note_head::brew_ez_stencil
     Ottava_bracket::print
     Paper_column::before_line_breaking
     Paper_column::print
     Percent_repeat_item_interface::beat_slash
     Percent_repeat_item_interface::double_percent
     Piano_pedal_bracket::print
     Rest::after_line_breaking
     Rest::extent_callback
     Rest::polyphonic_offset_callback
     Rest::print
     Script_column::before_line_breaking
     Script_interface::before_line_breaking
     Script_interface::print
     Self_alignment_interface::aligned_on_parent
     Self_alignment_interface::aligned_on_self
     Self_alignment_interface::centered_on_other_axis_parent
     Self_alignment_interface::centered_on_parent
     Separating_group_spanner::set_spacing_rods
     Side_position_interface::aligned_on_support_refpoints
     Side_position_interface::aligned_side
     Slur::after_line_breaking
     Slur::height
     Slur::print
     Spacing_spanner::set_springs
     Span_bar::before_line_breaking
     Span_bar::get_bar_size
     Span_bar::print
     Span_bar::width_callback
     Spanner::set_spacing_rods
     Staff_symbol::print
     Staff_symbol_referencer::callback
     Stem::before_line_breaking
     Stem::dim_callback
     Stem::height
     Stem::off_callback
     Stem::print
     Stem_tremolo::height
     Stem_tremolo::print
     Sustain_pedal::print
     System_start_delimiter::after_line_breaking
     System_start_delimiter::print
     Text_item::print
     Text_spanner::print
     Tie::print
     Tie_column::after_line_breaking
     Time_signature::print
     Tuplet_bracket::after_line_breaking
     Tuplet_bracket::before_line_breaking
     Tuplet_bracket::print
     Vaticana_ligature::brew_ligature_primitive
     Vaticana_ligature::print
     Volta_bracket_interface::print



     
     )))

(define-public (make-safe-lilypond-module)
  (let*
      ((m (make-safe-module)))
    (for-each
     (lambda (p) (module-define! m (car p) (cdr p)))
     safe-objects)
    m))
    
