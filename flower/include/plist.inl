/* -*-c++-*-
  plist.inl -- part of flowerlib

  (c) 1996 Han-Wen Nienhuys& Jan Nieuwenhuizen
*/

#ifndef PLIST_INL
#define PLIST_INL

template<class T>
void
PL_copy(IPointerList<T*> &to, IPointerList<T*> const&src)
{
    for (PCursor<T*> pc(src); pc.ok(); pc++) {
	T *q = pc;
	T *p=new T(*q) ; 
	to.bottom().add(p);
    }
}

#endif
