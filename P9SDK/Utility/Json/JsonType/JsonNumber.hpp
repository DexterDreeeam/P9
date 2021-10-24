#pragma once

// range:
// from
//     -1e18 - 1
// to
//     +1e18 + 1
//
// fraction precision
// 1e-18

class number
{
public:
    number() :
        _sign(boole::True),
        _integer(0),
        _fraction(0)
    {}

    number(boole sign, u64 integer, u64 fraction) :
        _sign(sign),
        _integer(integer),
        _fraction(fraction)
    {
        if (_integer == 0 && _fraction == 0)
        {
            _sign = boole::True;
        }
    }

    number(const number& rhs) :
        _sign(rhs._sign),
        _integer(rhs._integer),
        _fraction(rhs._fraction)
    {
    }

    ~number() = default;

    number& operator =(const number& rhs) noexcept
    {
        _sign = rhs._sign;
        _integer = rhs._integer;
        _fraction = rhs._fraction;
        return *this;
    }

    bool operator ==(const number& rhs) const noexcept
    {
        return _sign == rhs._sign && _integer == rhs._integer && _fraction == rhs._fraction;
    }

    bool operator !=(const number& rhs) const noexcept
    {
        return _sign != rhs._sign || _integer != rhs._integer || _fraction!= rhs._fraction;
    }

    bool operator <(const number& rhs) const noexcept
    {
        s64 abs_ = abs_compare(rhs);
        bool rst = false;
        rst = rst || (_sign == boole::False && rhs._sign == boole::True);
        rst = rst || (_sign == boole::True && rhs._sign == boole::True && abs_ < 0);
        rst = rst || (_sign == boole::False && rhs._sign == boole::False && abs_ > 0);
        return rst;
    }

    bool operator >(const number& rhs) const noexcept
    {
        s64 abs_ = abs_compare(rhs);
        bool rst = false;
        rst = rst || (_sign == boole::True && rhs._sign == boole::False);
        rst = rst || (_sign == boole::True && rhs._sign == boole::True && abs_ > 0);
        rst = rst || (_sign == boole::False && rhs._sign == boole::False && abs_ < 0);
        return rst;
    }

    bool operator >=(const number& rhs) const noexcept
    {
        bool rst = this->operator <(rhs);
        return !rst;
    }

    bool operator <=(const number& rhs) const noexcept
    {
        bool rst = this->operator >(rhs);
        return !rst;
    }

private:
    s64 abs_compare(const number& rhs) const noexcept
    {
        if (_integer > rhs._integer)
        {
            return 1;
        }
        else if (_integer < rhs._integer)
        {
            return -1;
        }

        if (_fraction > rhs._fraction)
        {
            return 1;
        }
        else if (_fraction < rhs._fraction)
        {
            return -1;
        }
        else
        {
            return 0;
        }
    }

public:
    boole _sign;
    u64   _integer;
    u64   _fraction;
};

const s64 json_float_precision = 18;

class json_number : public json_base
{
public:
    json_number() = delete;

    json_number(f64 f) :
        json_base(),
        _sign(boole::True),
        _integer(0),
        _fraction(0)
    {
        if (f >= 0.0)
        {
            _sign = boole::True;
        }
        else
        {
            f = -f;
            _sign = boole::False;
        }
        assert_info(f < (f64)u64_max, "out of range.");
        _integer = (u64)f;
        f -= _integer;

        f *= _power(10, json_float_precision);
        _fraction = (u64)f;
    }

    json_number(boole sign, u64 integer, u64 fraction) :
        json_base(),
        _sign(sign),
        _integer(integer),
        _fraction(fraction)
    {}

    json_number(const json_number& rhs) = delete;

    virtual ~json_number() override = default;

public:
    virtual json_type type() const override
    {
        return json_type::NUMBER;
    }

    virtual const char* type_name() const override
    {
        return "json_number";
    }

    virtual s64 size() const override
    {
        return 0;
    }

    virtual json_base* index(const string& key) override
    {
        assert(0);
        return nullptr;
    }

    virtual json_base* index(s64 order) override
    {
        assert(0);
        return nullptr;
    }

    virtual JsonNs::json_parent_context get_parent_context(s64 order) override
    {
        assert(0);
        return JsonNs::json_parent_context();
    }

    virtual string value() const override
    {
        char text[32];
        s64 len = s64_to_text(_integer, text);
        text[len] = 0;

        return string(_sign ? "" : "-") + string(text) + '.' + fraction_string();
    }

    virtual json_base* clone() const override
    {
        return new json_number(_sign, _integer, _fraction);
    }

    virtual void serialize(_OUT_ string& str) const override
    {
        str.clear();
        this->json_number::serialize_append(str);
    }

    virtual void serialize_append(_OUT_ string& str) const override
    {
        str += value();
    }

public:
    static json_base* deserialize(const string& str, s64 from, s64 to);

public:
    number as_number() const
    {
        return number(_sign, _integer, _fraction);
    }

private:
    string fraction_string() const
    {
        string r_fraction_string = "";
        u64 frc = _fraction;
        for (s64 b = 0; b < json_float_precision; ++b)
        {
            r_fraction_string += '0' + frc % 10;
            frc /= 10;
        }
        string rst = r_fraction_string.reverse();
        while (rst.size() > 1 && rst.back() == '0')
        {
            rst.pop_back();
        }
        return rst;
    }

private:
    boole _sign;
    u64   _integer;
    u64   _fraction;
};

_INLINE_ json_base* json_number::deserialize(const string& str, s64 from, s64 to)
{
    trim_index(str, from, to);
    boole sign = boole::True;
    u64 integer = 0;
    u64 fraction = 0;

    if (str[from] == '-')
    {
        sign = boole::False;
        ++from;
    }
    if (to - from < 1)
    {
        return nullptr;
    }
    s64 stop_pos = to;

    for (s64 i = from; i < to; ++i)
    {
        char ch = str[i];
        if (is_digit(ch))
        {
            continue;
        }
        else if (ch == '.' && stop_pos == to)
        {
            stop_pos = i;
        }
        else
        {
            // error
            return nullptr;
        }
    }
    if (to - from == 1 && stop_pos != to)
    {
        // only stop sign, error
        return nullptr;
    }

    s64 v;
    if (stop_pos == from)
    {
        integer = 0;
    }
    else if (text_to_s64(str.data() + from, stop_pos - from, v))
    {
        integer = v;
    }
    else
    {
        // error
        return nullptr;
    }

    s64 fraction_len = to - stop_pos - 1;
    if (fraction_len < 1)
    {
        fraction = 0;
    }
    else if (text_to_s64(str.data() + stop_pos + 1, fraction_len, v))
    {
        fraction = v;
    }
    else
    {
        // error
        return nullptr;
    }

    while (fraction_len++ < json_float_precision)
    {
        fraction *= 10;
    }
    return new json_number(sign, integer, fraction);
}
