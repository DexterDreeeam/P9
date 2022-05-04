#pragma once

#include "_Common.hpp"
#include "_Declaration.hpp"

namespace sdk
{

enum class result_type : s64
{
    Fail = s64_min,
    Unknown,
    ResourceNotFound,
    OutOfMemory,

    Success = 0,
};

namespace _ResultNs
{

class result_base
{
public:
    result_base(result_type type = result_type::Unknown) :
        _type(type)
    {
    }

    result_type type() const { return _type; }

protected:
    result_type  _type;
};

}

template<>
class result<void> : public obj<result<void>, has<_ResultNs::result_base>>
{
public:
    result() = default;

    result(result_type type)
    {
        _ResultNs::result_base::_type = type;
    }

    result(const result& rhs)
    {
        _ResultNs::result_base::_type = rhs.type();
    }

    result& operator =(const result& rhs)
    {
        _ResultNs::result_base::_type = rhs.type();
    }

    virtual ~result() = default;

    virtual const char* obj_type() const { return "result"; }

    virtual u128 obj_type_id() const { return u128(0x76a2a0b722eb518b, 0x8a8173498ebbe460); }

};

template<typename Ty>
class result : public obj<result<Ty>, has<_ResultNs::result_base>>
{
public:
    result() = default;

    result(result_type type, const Ty& content) :
        _content(content)
    {
        _ResultNs::result_base::_type = type;
    }

    result(const result& rhs) :
        _content(rhs.content())
    {
        _ResultNs::result_base::_type = rhs.type();
    }

    result& operator =(const result& rhs)
    {
        _content = rhs.content();
        _ResultNs::result_base::_type = rhs.type();
    }

    virtual ~result() = default;

    virtual const char* obj_type() const { return "result"; }

    virtual u128 obj_type_id() const { return u128(0x76a2a0b722eb518b, 0x8a8173498ebbe460); }

public:
    Ty& content() { return _content; }

    const Ty& content() const { return _content; }

protected:
    Ty _content;
};

}
