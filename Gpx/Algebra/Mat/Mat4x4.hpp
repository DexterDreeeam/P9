#pragma once

#include "Mat.hpp"

namespace gpx
{

template<mat_major Major = mat_major::Row>
class mat4x4;

template<mat_major Major>
class mat4x4 : public mat<4, 4, f32, Major>
{
    using SelfTy = mat4x4<Major>;
    using BaseTy = mat<4, 4, f32, Major>;

public:
    mat4x4() :
        BaseTy()
    {
    }

    mat4x4(const SelfTy& rhs) :
        BaseTy(rhs)
    {
    }

    mat4x4(const BaseTy& rhs) :
        BaseTy(rhs)
    {
    }

    template<typename ...Args>
    mat4x4(Args ...args) :
        BaseTy(args...)
    {
    }

    ~mat4x4() = default;

public:
    void self_transpose()
    {
        swap(BaseTy::at(0, 1), BaseTy::at(1, 0));
        swap(BaseTy::at(0, 2), BaseTy::at(2, 0));
        swap(BaseTy::at(0, 3), BaseTy::at(3, 0));
        swap(BaseTy::at(1, 2), BaseTy::at(2, 1));
        swap(BaseTy::at(1, 3), BaseTy::at(3, 1));
        swap(BaseTy::at(2, 3), BaseTy::at(3, 2));
    }

    SelfTy transpose() const
    {
        SelfTy m(*this);
        m.self_transpose();
        return m;
    }
};

}
