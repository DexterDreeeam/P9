#pragma once

class ref_base;

template<typename Ty>
class ref;

namespace RefNs
{

class observer_base : public RefNs::context
{
    template<typename All_Ty> friend class observer;
    template<typename All_Ty> friend class ref;
    friend class ref_base;

public:
    observer_base();

public:
    observer_base(void* ptr, RefNs::counter* cnt, s64 data_size, RefNs::deconstructor* dc);

    observer_base(const observer_base& rhs);

    observer_base(observer_base&& rhs) noexcept;

public:
    ~observer_base();

public:
    observer_base& operator =(const observer_base& rhs);

    observer_base& operator =(observer_base&& rhs) noexcept;

public:
    boole valid() const;

    boole invalid() const;

    void clear();
};

}

template<typename Ty>
class observer : public RefNs::observer_base
{
    // friend class observer_base;
    template<typename All_Ty> friend class ref;

    using Self_Ty = typename::observer<Ty>;
    using Value_Ty = Ty;
    using Value_Ptr_Ty = Ty*;

public:
    observer();

    observer(const Self_Ty& rhs);

    observer(Self_Ty&& rhs);

    observer(const observer_base& rb);

public:
    ~observer();

    Self_Ty& operator =(const Self_Ty& rhs);

    template<typename Ty2>
    Self_Ty& operator =(const observer<Ty2>& rhs);

    ref<Ty> try_ref();

#if DEBUG_LEVEL >= DEBUG_LEVEL_CALIBRATION_ALL

private:
    const Ty* const* _debug_pptr;
    const Ty* const& _debug_ptr;

#endif
};
