#pragma once

class ref_base;

template<typename Ty>
class ref;

class ref_base : public RefNs::context
{
    template<typename All_Ty> friend class ref;

public:
    ref_base();

protected:
    ref_base(void* ptr, RefNs::counter* cnt, s64 data_size, RefNs::deconstructor* dc);

    ref_base(const ref_base& rhs);

    ref_base(ref_base&& rhs);

    // try to convert observer to ref
    ref_base(const RefNs::observer_base& ob);

public:
    ~ref_base();

public:
    ref_base& operator =(const ref_base& rhs);

    ref_base& operator =(ref_base&& rhs) noexcept;

    ref_base& operator =(const RefNs::observer_base& ob);

    bool operator ==(const ref_base& rhs) const noexcept;

public:
    operator boole() const;

    operator bool() const;

    boole valid() const;

    boole invalid() const;

    template<typename Ty>
    ref<Ty> ref_of();

    void clear();

protected:
    template<typename Ty>
    Ty* pointer_of();

    template<typename Ty>
    const Ty* pointer_of() const;

protected:
    template<typename Ty, typename ...Args>
    static ref_base new_instance(RefNs::deconstructor* dc, Args... args);

    template<typename Ty>
    static ref_base new_instance(Ty* raw_ptr, RefNs::deconstructor* dc);

protected:
    RefNs::observer_base observer();
};

template<typename Ty>
class ref : public ref_base
{
    friend class ref_base;
    template<typename All_Ty> friend class ref;

    using Self_Ty = typename::ref<Ty>;
    using Observer_Ty = typename::observer<Ty>;

public:
    ref();

    ref(const Self_Ty& rhs);

    ref(Self_Ty&& rhs);

    template<typename Ty2>
    ref(const ref<Ty2>& rhs);

    ref(const Observer_Ty& ob);

private:
    ref(const ref_base& rb);

public:
    ~ref();

    Self_Ty& operator =(const Self_Ty& rhs);

    Self_Ty& operator =(Self_Ty&& rhs);

    template<typename Ty2>
    Self_Ty& operator =(const ref<Ty2>& rhs);

    Self_Ty& operator =(const Observer_Ty& ob);

    bool operator ==(const ref<Ty>& rhs) const noexcept;

    bool operator !=(const ref<Ty>& rhs) const noexcept;

    Ty* operator ->();

    const Ty* operator ->() const;

    Ty& operator *();

    const Ty& operator *() const;

public:
    template<typename ...Args>
    static Self_Ty new_instance(Args ...args);

    static Self_Ty new_instance(Ty* raw_ptr);

    static void deconstruct(void* p);

public:
    typename::observer<Ty> observer();

    operator typename::observer<Ty>();

#if DEBUG_LEVEL >= DEBUG_LEVEL_CALIBRATION_ALL

private:
    const Ty* const* _debug_pptr;
    const Ty* const& _debug_ptr;

#endif
};
