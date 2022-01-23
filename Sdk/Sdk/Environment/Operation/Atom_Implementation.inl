#pragma once

template<typename ValTy>
_INLINE_ atom<s64>::atom(const ValTy& v) :
    atom((s64)v)
{
}

template<typename ValTy>
_INLINE_ void atom<s64>::set(const ValTy& v)
{
    this->set((s64)v);
}

template<typename ValTy>
_INLINE_ atom<s64>& atom<s64>::operator =(const ValTy& v)
{
    return this->operator =((s64)v);
}

template<typename ValTy>
_INLINE_ s64 atom<s64>::exchange(const ValTy& replace)
{
    return this->exchange((s64)replace);
}

template<typename ValTy1, typename ValTy2>
_INLINE_ s64 atom<s64>::compare_exchange(const ValTy1& expected, const ValTy2& replace)
{
    return this->compare_exchange((s64)expected, (s64)replace);
}

template<typename ValTy>
_INLINE_ atom<u64>::atom(const ValTy& v) :
    atom((u64)v)
{
}

template<typename ValTy>
_INLINE_ void atom<u64>::set(const ValTy& v)
{
    set((u64)v);
}

template<typename ValTy>
_INLINE_ atom<u64>& atom<u64>::operator =(const ValTy& v)
{
    return this->operator =((u64)v);
}

template<typename ValTy>
_INLINE_ u64 atom<u64>::exchange(const ValTy& replace)
{
    return this->exchange((u64)replace);
}

template<typename ValTy1, typename ValTy2>
_INLINE_ u64 atom<u64>::compare_exchange(const ValTy1& expected, const ValTy2& replace)
{
    return this->compare_exchange((u64)expected, (u64)replace);
}

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
