/*   
  music-sequence.hh -- declare Music_sequence
  
  source file of the GNU LilyPond music typesetter
  
  (c) 2000--2004 Han-Wen Nienhuys <hanwen@cs.uu.nl>
  
 */

#ifndef MUSIC_SEQUENCE_HH
#define MUSIC_SEQUENCE_HH

#include "music.hh"

class Music_sequence : public Music
{
public:
  Music_sequence (SCM h);
  VIRTUAL_COPY_CONSTRUCTOR (Music, Music_sequence);
  DECLARE_SCHEME_CALLBACK(cumulative_length_callback, (SCM));
  DECLARE_SCHEME_CALLBACK(maximum_length_callback, (SCM));
  DECLARE_SCHEME_CALLBACK(minimum_start_callback, (SCM));
  DECLARE_SCHEME_CALLBACK(first_start_callback, (SCM));
  SCM music_list () const;
  
  Pitch do_relative_octave (Pitch p, bool b);
  
  static Moment cumulative_length (SCM) ;
  static Moment maximum_length (SCM) ;
  static Moment first_start (SCM list) ;
  static Moment minimum_start (SCM list);
};

Pitch music_list_to_relative (SCM l,Pitch p, bool ret_first);
void transpose_music_list (SCM , Pitch);
void compress_music_list (SCM, Moment);

#endif
