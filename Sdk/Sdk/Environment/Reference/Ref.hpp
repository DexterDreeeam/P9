#pragma once

template<typename Ty>
class obs;

template<typename Ty>
class ref
{
    template<typename All_Ty>
    friend class ref;

    template<typename All_Ty>
    friend class obs;

public:
    ref();

    ref(const ref<Ty>& rhs);

    ref(ref<Ty>&& rhs) noexcept;

    template<typename Other_Ty>
    ref(const ref<Other_Ty>& rhs);

    template<typename Other_Ty>
    ref(ref<Other_Ty>&& rhs) noexcept;

    ~ref();

    ref<Ty>& operator =(const ref<Ty>& rhs);

    ref<Ty>& operator =(ref<Ty>&& rhs) noexcept;

    template<typename Other_Ty>
    ref<Ty>& operator =(const ref<Other_Ty>& rhs);

    template<typename Other_Ty>
    ref<Ty>& operator =(ref<Other_Ty>&& rhs) noexcept;

    bool operator ==(const ref<Ty>& rhs) const;

    bool operator !=(const ref<Ty>& rhs) const;

    template<typename Other_Ty>
    bool operator ==(const ref<Other_Ty>& rhs) const;

    template<typename Other_Ty>
    bool operator !=(const ref<Other_Ty>& rhs) const;

    Ty* operator ->();

    const Ty* operator ->() const;

    Ty& operator *();

    const Ty& operator *() const;

private:
    explicit ref(_RefNs::context<Ty>* ctx);

public:
    template<typename Other_Ty>
    ref<Other_Ty> ref_of();

    boole has_value() const;

    boole empty() const { return !has_value(); }

    Ty* raw_ptr() { return _ctx.get() ? _ctx.get()->_ptr : nullptr; }

    const Ty* raw_ptr() const { return _ctx.get() ? _ctx.get()->_ptr : nullptr; }

    void clear();

public:
    void debug_report() const;

public:
    template<typename ...Args>
    static ref<Ty> new_instance(Args ...args);

    static ref<Ty> new_instance_with_ptr(Ty* raw_ptr);

private:
    atom<_RefNs::context<Ty>*> _ctx;
};
