;;; ps.scm -- implement Scheme output routines for PostScript
;;;
;;;  source file of the GNU LilyPond music typesetter
;;; 
;;; (c) 1998--2001 Jan Nieuwenhuizen <janneke@gnu.org>
;;; Han-Wen Nienhuys <hanwen@cs.uu.nl>


(define (ps-scm action-name)

  ;; alist containing fontname -> fontcommand assoc (both strings)
  (define font-alist '())
  (define font-count 0)
  (define current-font "")

  
  (define (cached-fontname i)
    (string-append
     "lilyfont"
     (make-string 1 (integer->char (+ 65 i)))))
    

  (define (select-font name-mag-pair)
    (let*
	(
	 (c (assoc name-mag-pair font-name-alist))
	 )

      (if (eq? c #f)
	  (begin
	    (display "FAILED\n")
	    (display (object-type (car name-mag-pair)))
	    (display (object-type (caaar font-name-alist)))

	    (ly-warn (string-append
		      "Programming error: No such font known "
		      (car name-mag-pair) " "
		      (ly-number->string (cdr name-mag-pair))
		      ))
	    
	    "") ; issue no command	  
	  (string-append " " (cddr c) " "))
      ))

    (define (font-load-command name-mag command)
      (string-append
       "/" command
       " { /"
       (car name-mag)
       " findfont "
       "12 " (ly-number->string (cdr name-mag)) " mul "
       "lilypondpaperoutputscale div scalefont setfont } bind def "
       "\n"))


  (define (beam width slope thick)
    (string-append
     (numbers->string (list width slope thick)) " draw_beam" ))

  (define (comment s)
    (string-append "% " s))

  (define (bracket arch_angle arch_width arch_height  height arch_thick thick)
    (string-append
     (numbers->string (list arch_angle arch_width arch_height height arch_thick thick)) " draw_bracket" ))

  (define (char i)
    (invoke-char " show" i))


  (define (hairpin thick width starth endh )
    (string-append 
     (numbers->string (list width starth endh thick))
     " draw_hairpin"))
  
  ;; what the heck is this interface ?
  (define (dashed-slur thick dash l)
    (string-append 
     (apply string-append (map control->string l)) 
     (ly-number->string thick) 
     " [ "
     (ly-number->string dash)
     " "
     (ly-number->string (* 10 thick))	;UGH.  10 ?
     " ] 0 draw_dashed_slur"))

  (define (dashed-line thick on off dx dy)
    (string-append 
     (ly-number->string dx)
     " "
     (ly-number->string dy)
     " "
     (ly-number->string thick) 
     " [ "
     (ly-number->string on)
     " "
     (ly-number->string off)
     " ] 0 draw_dashed_line"))
  
  (define (repeat-slash wid slope thick)
   (string-append (numbers->string (list wid slope thick))
    " draw_repeat_slash"))
  
  (define (end-output)
    "\nend-lilypond-output\n")
  
  (define (experimental-on) "")
  
  (define (filledbox breapth width depth height) 
    (string-append (numbers->string (list breapth width depth height))
		   " draw-box" ))

  ;; obsolete?
  (define (font-def i s)
    (string-append
     "\n/" (font i) " {/" 
     (substring s 0 (- (string-length s) 4))
     " findfont 12 scalefont setfont} bind def \n"))

  (define (font-switch i)
    (string-append (font i) " "))

  (define (header-end)
    (string-append
     ;; URG: now we can't use scm output without Lily
     (ly-gulp-file "lilyponddefs.ps")
     " {exch pop //systemdict /run get exec} "
     (ly-gulp-file "lily.ps")
     "{ exch pop //systemdict /run get exec } "
    ))
  
  (define (lily-def key val)

     (if (string=? (substring key 0 (min (string-length "lilypondpaper") (string-length key))) "lilypondpaper")
	 (string-append "/" key " {" val "} bind def\n")
	 (string-append "/" key " (" val ") def\n")
	 )
     )

  (define (header creator generate) 
    (string-append
     "%!PS-Adobe-3.0\n"
     "%%Creator: " creator generate "\n"))
  
  (define (invoke-char s i)
    (string-append 
     "(\\" (inexact->string i 8) ") " s " " ))
  
  (define (invoke-dim1 s d) 
    (string-append
     (ly-number->string (* d  (/ 72.27 72))) " " s ))

  (define (placebox x y s) 
    (string-append 
     (ly-number->string x) " " (ly-number->string y) " {" s "} place-box\n"))

  (define (bezier-sandwich l thick)
    (string-append 
     (apply string-append (map control->string l))
     (ly-number->string  thick)
     " draw_bezier_sandwich"))

  (define (start-line height)
	  "\nstart-line {
lilypondpaperoutputscale lilypondpaperoutputscale scale
")
  
  (define (stem breapth width depth height) 
    (string-append (numbers->string (list breapth width depth height))
		   " draw-box" ))

  (define (stop-line)
      "}\nstop-line\n")

  (define (text s)
    (string-append "(" s ") show  "))


  (define (volta h w thick vert_start vert_end)
    (string-append 
     (numbers->string (list h w thick (inexact->exact vert_start) (inexact->exact vert_end)))
     " draw_volta"))

  (define (tuplet ht gap dx dy thick dir)
    (string-append 
     (numbers->string (list ht gap dx dy thick (inexact->exact dir)))
     " draw_tuplet"))


  (define (unknown) 
    "\n unknown\n")

  (define (ez-ball ch letter-col ball-col)
    (string-append
     " (" ch ") "
     (numbers->string (list letter-col ball-col))
     " /Helvetica-Bold " ;; ugh
     " draw_ez_ball"))

  (define (define-origin a b c ) "")
  (define (no-origin) "")
  
  ;; PS
  (cond ((eq? action-name 'all-definitions)
	 `(begin
	    (define beam ,beam)
	    (define tuplet ,tuplet)
	    (define bracket ,bracket)
	    (define char ,char)
	    (define hairpin ,hairpin)
	    (define volta ,volta)
	    (define bezier-sandwich ,bezier-sandwich)
	    (define dashed-line ,dashed-line) 
	    (define dashed-slur ,dashed-slur) 
	    (define end-output ,end-output)
	    (define experimental-on ,experimental-on)
	    (define filledbox ,filledbox)
	    (define font-def ,font-def)
	    (define font-switch ,font-switch)
	    (define header-end ,header-end)
	    (define lily-def ,lily-def)
	    (define font-load-command ,font-load-command)
	    (define header ,header) 
	    (define invoke-char ,invoke-char) 
	    (define invoke-dim1 ,invoke-dim1)
	    (define placebox ,placebox)
	    (define select-font ,select-font)
	    (define start-line ,start-line)
	    (define stem ,stem)
	    (define stop-line ,stop-line)
	    (define stop-last-line ,stop-line)
	    (define repeat-slash ,repeat-slash)
	    (define text ,text)
	    (define no-origin ,no-origin)
	    (define define-origin ,define-origin)
	    (define ez-ball ,ez-ball)
	    ))
	((eq? action-name 'repeat-slash) repeat-slash)
	((eq? action-name 'tuplet) tuplet)
	((eq? action-name 'beam) beam)
	((eq? action-name 'bezier-sandwich) bezier-sandwich)
	((eq? action-name 'bracket) bracket)
	((eq? action-name 'char) char)
	((eq? action-name 'dashed-line) dashed-line) 
	((eq? action-name 'dashed-slur) dashed-slur) 
	((eq? action-name 'hairpin) hairpin)
	((eq? action-name 'experimental-on) experimental-on)
	((eq? action-name 'filledbox) filledbox)
	((eq? action-name 'ez-ball) ez-ball)	
	((eq? action-name 'select-font) select-font)
	((eq? action-name 'volta) volta)
	(else (error "unknown tag -- PS-SCM " action-name))
	)
  )

(define (scm-ps-output)
  (ly-eval (ps-scm 'all-definitions)))
