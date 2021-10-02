#pragma once

class json_int : public JsonNs::json_base
{
public:
    json_int() :
        _value(0)
    {}

    json_int(s64 i) :
        _value(i)
    {}

    json_int(const json_int& rhs) :
        _value(rhs._value)
    {}

public:
    virtual JsonNs::json_type type() const override
    {
        return JsonNs::json_type::INT;
    }

    virtual JsonNs::json_base* clone() const override
    {
        return new json_int(*this);
    }

    virtual void serialize(OUT string& str) const override
    {
        str.clear();
        this->json_int::serialize_append(str);
    }

    virtual void serialize_append(OUT string& str) const override
    {
        char text[32];
        s64 len = s64_to_text(_value, text);
        text[len] = 0;

        str += text;
    }

public:
    static json_base* deserialize(const string& str, s64 from, s64 to)
    {
        return nullptr;
    }

private:
    s64 _value;
};
