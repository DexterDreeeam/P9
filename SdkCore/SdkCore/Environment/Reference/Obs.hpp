#pragma once

template<typename Ty>
class ref;

template<typename Ty>
class obs
{
    template<typename All_Ty>
    friend class ref;

    template<typename All_Ty>
    friend class obs;

public:
    obs();

    obs(const obs<Ty>& rhs);

    obs(obs<Ty>&& rhs) noexcept;

    template<typename Other_Ty>
    obs(const obs<Other_Ty>& rhs);

    template<typename Other_Ty>
    obs(obs<Other_Ty>&& rhs) noexcept;

    obs(const ref<Ty>& rhs);

    obs(ref<Ty>&& rhs) noexcept;

    template<typename Other_Ty>
    obs(const ref<Other_Ty>& rhs);

    template<typename Other_Ty>
    obs(ref<Other_Ty>&& rhs) noexcept;

    ~obs();

    obs<Ty>& operator =(const obs<Ty>& rhs);

    obs<Ty>& operator =(obs<Ty>&& rhs) noexcept;

    template<typename Other_Ty>
    obs<Ty>& operator =(const obs<Other_Ty>& rhs);

    template<typename Other_Ty>
    obs<Ty>& operator =(obs<Other_Ty>&& rhs) noexcept;

    obs<Ty>& operator =(const ref<Ty>& rhs);

    obs<Ty>& operator =(ref<Ty>&& rhs) noexcept;

    template<typename Other_Ty>
    obs<Ty>& operator =(const ref<Other_Ty>& rhs);

    template<typename Other_Ty>
    obs<Ty>& operator =(ref<Other_Ty>&& rhs) noexcept;

    bool operator ==(const obs<Ty>& rhs) const;

    bool operator !=(const obs<Ty>& rhs) const;

    template<typename Other_Ty>
    bool operator ==(const obs<Other_Ty>& rhs) const;

    template<typename Other_Ty>
    bool operator !=(const obs<Other_Ty>& rhs) const;

private:
    explicit obs(_Internal::_Ref::ref_context<Ty>* ctx);

public:
    template<typename Other_Ty>
    obs<Other_Ty> obs_of();

    ref<Ty> try_ref();

    boole has_value() const;

    boole empty() const { return !has_value(); }

    Ty* raw_ptr() { return _ctx.get() ? _ctx.get()->_ptr : nullptr; }

    const Ty* raw_ptr() const { return _ctx.get() ? _ctx.get()->_ptr : nullptr; }

    void clear();

private:
    atom<_Internal::_Ref::ref_context<Ty>*> _ctx;
};

template<>
class obs<void>
{
    template<typename All_Ty>
    friend class ref;

    template<typename All_Ty>
    friend class obs;

public:
    obs();

    obs(const obs<void>& rhs);

    obs(obs<void>&& rhs) noexcept;

    template<typename Other_Ty>
    obs(const obs<Other_Ty>& rhs);

    template<typename Other_Ty>
    obs(obs<Other_Ty>&& rhs) noexcept;

    obs(const ref<void>& rhs);

    obs(ref<void>&& rhs) noexcept;

    template<typename Other_Ty>
    obs(const ref<Other_Ty>& rhs);

    template<typename Other_Ty>
    obs(ref<Other_Ty>&& rhs) noexcept;

    ~obs();

    obs<void>& operator =(const obs<void>& rhs);

    obs<void>& operator =(obs<void>&& rhs) noexcept;

    template<typename Other_Ty>
    obs<void>& operator =(const obs<Other_Ty>& rhs);

    template<typename Other_Ty>
    obs<void>& operator =(obs<Other_Ty>&& rhs) noexcept;

    obs<void>& operator =(const ref<void>& rhs);

    obs<void>& operator =(ref<void>&& rhs) noexcept;

    template<typename Other_Ty>
    obs<void>& operator =(const ref<Other_Ty>& rhs);

    template<typename Other_Ty>
    obs<void>& operator =(ref<Other_Ty>&& rhs) noexcept;

    bool operator ==(const obs<void>& rhs) const;

    bool operator !=(const obs<void>& rhs) const;

    template<typename Other_Ty>
    bool operator ==(const obs<Other_Ty>& rhs) const;

    template<typename Other_Ty>
    bool operator !=(const obs<Other_Ty>& rhs) const;

private:
    explicit obs(_Internal::_Ref::ref_context<void>* ctx);

public:
    template<typename Other_Ty>
    obs<Other_Ty> obs_of();

    ref<void> try_ref();

    boole has_value() const;

    boole empty() const { return !has_value(); }

    void* raw_ptr() { return _ctx.get() ? _ctx.get()->_ptr : nullptr; }

    const void* raw_ptr() const { return _ctx.get() ? _ctx.get()->_ptr : nullptr; }

    void clear();

private:
    atom<_Internal::_Ref::ref_context<void>*> _ctx;
};
