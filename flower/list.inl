// -*-c++-*-

#ifndef LIST_INL
#define LIST_INL

template<class T>
inline
List<T>::List()
{
    set_empty();
}

template<class T>
inline void
List<T>::set_empty()
{
    top_ = bottom_ = 0;
    size_ = 0;
}

template<class T>
inline void
List<T>::remove( Cursor<T> me )
{
    if ( me.ok() ){
	Link<T> *lp = me.pointer();	
	lp->remove(*this);
	delete lp;
        size_--;
    }
}

template<class T>
inline int
List<T>::size() const
{ 
    return size_;
}

template<class T>
inline Cursor<T>
List<T>::top()const
{
    return Cursor<T>( *this, top_ );
}


template<class T>
inline Cursor<T>
List<T>::bottom()const
{
    return Cursor<T>( *this, bottom_ );
}


#endif
