#pragma once

// range:
// from
//     -1e18 - 1
// to
//     +1e18 + 1
//
// fraction precision
// 1e-18

const s64 json_float_precision = 18;

class json_float : public json_base
{
public:
    json_float() = delete;

    json_float(f64 f) :
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

        f *= power(10, json_float_precision);
        _fraction = (u64)f;
    }

    json_float(boole sign, u64 integer, u64 fraction) :
        json_base(),
        _sign(sign),
        _integer(integer),
        _fraction(fraction)
    {}

    json_float(const json_int& rhs) = delete;

    virtual ~json_float() override = default;

public:
    virtual json_type type() const override
    {
        return json_type::FLOAT;
    }

    virtual const char* type_name() const override
    {
        return "json_float";
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
        return new json_float(_sign, _integer, _fraction);
    }

    virtual void serialize(OUT string& str) const override
    {
        str.clear();
        this->json_float::serialize_append(str);
    }

    virtual void serialize_append(OUT string& str) const override
    {
        str += value();
    }

public:
    static json_base* deserialize(const string& str, s64 from, s64 to);

public:
    f64 as_f64() const
    {
        f64 f = _integer + _fraction / power(10, json_float_precision);
        return _sign ? f : -f;
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

_INLINE_ json_base* json_float::deserialize(const string& str, s64 from, s64 to)
{
    trim_index(str, from, to);
    if (to - from < 3)
    {
        // at least "x.y"
        // error
        return nullptr;
    }

    boole sign = boole::True;
    u64 integer = 0;
    u64 fraction = 0;

    if (str[from] == '-')
    {
        sign = boole::False;
        ++from;
    }
    s64 stop_pos = -1;

    for (s64 i = from; i < to; ++i)
    {
        char ch = str[i];
        if (is_digit(ch))
        {
            continue;
        }
        else if (ch == '.' && stop_pos == -1)
        {
            stop_pos = i;
        }
        else
        {
            // error
            return nullptr;
        }
    }
    if (stop_pos == -1)
    {
        // error
        return nullptr;
    }

    s64 v;
    if (text_to_s64(str.data() + from, stop_pos - from, v) == boole::False)
    {
        // error
        return nullptr;
    }
    integer = v;

    s64 fraction_len = to - stop_pos - 1;
    if (text_to_s64(str.data() + stop_pos + 1, fraction_len, v) == boole::False)
    {
        // error
        return nullptr;
    }
    fraction = v;
    while (fraction_len++ < json_float_precision)
    {
        fraction *= 10;
    }
    return new json_float(sign, integer, fraction);
}
