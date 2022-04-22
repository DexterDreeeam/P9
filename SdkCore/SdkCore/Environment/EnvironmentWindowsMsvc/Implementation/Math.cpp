
#include "../../_Interface.hpp"
#include "../EnvironmentHeader.hpp"

f64 math::power(f64 x, f64 exp)
{
    return (f64)::pow(x, exp);
}

f64 math::tan(f64 x)
{
    return (f64)::tan(x);
}

f64 math::sin(f64 x)
{
    return (f64)::sin(x);
}

f64 math::cos(f64 x)
{
    return (f64)::cos(x);
}

f64 math::atan2(f64 x, f64 y)
{
    return (f64)::atan2(x, y);
}

f64 math::asin(f64 x)
{
    return (f64)::asin(x);
}

f64 math::acos(f64 x)
{
    return (f64)::acos(x);
}
