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

    obs(obs<Ty>&& rhs);

    template<typename Other_Ty>
    obs(const obs<Other_Ty>& rhs);

    template<typename Other_Ty>
    obs(obs<Other_Ty>&& rhs);

    obs(const ref<Ty>& rhs);

    obs(ref<Ty>&& rhs);

    template<typename Other_Ty>
    obs(const ref<Other_Ty>& rhs);

    template<typename Other_Ty>
    obs(ref<Other_Ty>&& rhs);

    ~obs();

    obs<Ty>& operator =(const obs<Ty>& rhs);

    obs<Ty>& operator =(obs<Ty>&& rhs);

    template<typename Other_Ty>
    obs<Ty>& operator =(const obs<Other_Ty>& rhs);

    template<typename Other_Ty>
    obs<Ty>& operator =(obs<Other_Ty>&& rhs);

    obs<Ty>& operator =(const ref<Ty>& rhs);

    obs<Ty>& operator =(ref<Ty>&& rhs);

    template<typename Other_Ty>
    obs<Ty>& operator =(const ref<Other_Ty>& rhs);

    template<typename Other_Ty>
    obs<Ty>& operator =(ref<Other_Ty>&& rhs);

    bool operator ==(const obs<Ty>& rhs) const;

    bool operator !=(const obs<Ty>& rhs) const;

    template<typename Other_Ty>
    bool operator ==(const obs<Other_Ty>& rhs) const;

    template<typename Other_Ty>
    bool operator !=(const obs<Other_Ty>& rhs) const;

public:
    template<typename Other_Ty>
    obs<Other_Ty> obs_of();

    ref<Ty> try_ref();

    boole has_value() const;

    boole empty() const { return !has_value(); }

    void clear();

private:
    atom<_RefNs::context<Ty>*> _ctx;
};
