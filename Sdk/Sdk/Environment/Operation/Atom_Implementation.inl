#pragma once

template<typename Ty>
_INLINE_ atom<Ty*>::atom() :
    _shadow()
{
}

template<typename Ty>
_INLINE_ atom<Ty*>::atom(const atom<Ty*>& rhs) :
    _shadow(rhs._shadow)
{
}

template<typename Ty>
_INLINE_ atom<Ty*>::atom(Ty* v) :
    _shadow(v)
{
}

template<typename Ty>
_INLINE_ Ty* atom<Ty*>::get() const
{
    return (Ty*)_shadow.get();
}

template<typename Ty>
_INLINE_ void atom<Ty*>::set(Ty* v)
{
    _shadow.set(v);
}

template<typename Ty>
_INLINE_ atom<Ty*>::operator Ty* ()
{
    return (Ty*)_shadow.operator void* ();
}

template<typename Ty>
_INLINE_ atom<Ty*>& atom<Ty*>::operator =(const atom<Ty*>& rhs)
{
    _shadow.operator =(rhs._shadow);
    return *this;
}

template<typename Ty>
_INLINE_ atom<Ty*>& atom<Ty*>::operator =(Ty* v)
{
    _shadow.operator =(v);
    return *this;
}

template<typename Ty>
_INLINE_ Ty* atom<Ty*>::exchange(Ty* replace)
{
    return (Ty*)_shadow.exchange(replace);
}

template<typename Ty>
_INLINE_ Ty* atom<Ty*>::compare_exchange(Ty* expected, Ty* replace)
{
    return (Ty*)_shadow.compare_exchange(expected, replace);
}
