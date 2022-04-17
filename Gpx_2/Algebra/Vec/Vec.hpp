#pragma once

#include "../../../Sdk/_Interface.hpp"

namespace gpx
{

template<s64 Dimension, typename ElemTy = f32>
class vec;

template<s64 Dimension, typename ElemTy>
class vec
{
    using SelfTy = vec<Dimension, ElemTy>;

public:
    vec() :
        elem()
    {
    }

    template<typename ...Args>
    vec(ElemTy e, Args ...args) :
        vec()
    {
        _construct(0, e, args...);
    }

    vec(const SelfTy& rhs)
    {
        for (s64 i = 0; i < Dimension; ++i)
        {
            elem[i] = rhs.elem[i];
        }
    }

    SelfTy& operator =(const SelfTy& rhs)
    {
        for (s64 i = 0; i < Dimension; ++i)
        {
            elem[i] = rhs.elem[i];
        }
        return *this;
    }

    ~vec() = default;

public:
    ElemTy& operator [](s64 ie)
    {
        assert(ie >= 0 && ie < Dimension);
        return elem[ie];
    }

    const ElemTy& operator [](s64 ie) const
    {
        assert(ie >= 0 && ie < Dimension);
        return elem[ie];
    }

    ElemTy& at(s64 ie)
    {
        assert(ie >= 0 && ie < Dimension);
        return elem[ie];
    }

    const ElemTy& at(s64 ie) const
    {
        assert(ie >= 0 && ie < Dimension);
        return elem[ie];
    }

public:
    SelfTy& operator +=(const SelfTy& rhs)
    {
        for (s64 i = 0; i < Dimension; ++i)
        {
            elem[i] += rhs.elem[i];
        }
        return *this;
    }

    SelfTy operator +(const SelfTy& rhs) const
    {
        SelfTy v(*this);
        v += rhs;
        return v;
    }

    SelfTy& operator -=(const SelfTy& rhs)
    {
        for (s64 i = 0; i < Dimension; ++i)
        {
            elem[i] -= rhs.elem[i];
        }
        return *this;
    }

    SelfTy operator -(const SelfTy& rhs) const
    {
        SelfTy v(*this);
        v -= rhs;
        return v;
    }

public:
    ElemTy dot(const SelfTy& rhs) const
    {
        ElemTy rst = 0;
        for (s64 i = 0; i < Dimension; ++i)
        {
            rst += elem[i] * rhs.elem[i];
        }
        return rst;
    }

    ElemTy operator *(const SelfTy& rhs) const
    {
        return dot(rhs);
    }

    void self_cross(const SelfTy& rhs)
    {
        assert(0);
    }

    SelfTy cross(const SelfTy& rhs) const
    {
        assert(0);
        return SelfTy();
    }

    ElemTy norm_sq() const
    {
        ElemTy rst = 0;
        for (s64 i = 0; i < Dimension; ++i)
        {
            rst += elem[i] * elem[i];
        }
        return rst;
    }

    ElemTy norm() const
    {
        return math::square_root(norm_sq());
    }

    void self_normalize()
    {
        ElemTy n = norm();
        assert(n > math::epsilon());

        for (s64 i = 0; i < Dimension; ++i)
        {
            elem[i] /= n;
        }
    }

    SelfTy normalize() const
    {
        SelfTy v(*this);
        v.self_normalize();
        return v;
    }

    void self_homogenize()
    {
        assert(Dimension >= 2);

        ElemTy w = elem[Dimension - 1];
        assert(w >= math::epsilon());

        for (s64 i = 0; i < Dimension - 1; ++i)
        {
            elem[i] /= w;
        }
        elem[Dimension - 1] = 1;
    }

    SelfTy homogenize() const
    {
        SelfTy v(*this);
        v.self_homogenize();
        return v;
    }

private:
    template<typename ...Args>
    void _construct(s64 pos, ElemTy e, Args ...args)
    {
        assert(pos < Dimension);

        elem[pos] = e;
        _construct(pos + 1, args...);
    }

    void _construct(s64 pos, ElemTy e)
    {
        assert(pos < Dimension);

        elem[pos] = e;
    }

public:
    ElemTy elem[Dimension];
};

}
