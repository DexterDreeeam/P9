#pragma once

class json_string : public JsonNs::json_base
{
public:
    json_string() :
        _value(0)
    {}

    json_string(const string& s) :
        _value(s)
    {}

    json_string(string&& s) :
        _value(right_value_type(s))
    {}

    json_string(const json_string& rhs) :
        _value(rhs._value)
    {}

    json_string(json_string&& rhs) noexcept :
        _value(right_value_type(rhs._value))
    {}

    virtual ~json_string() override = default;

public:
    virtual JsonNs::json_type type() const override
    {
        return JsonNs::json_type::STRING;
    }

    virtual JsonNs::json_base* clone() const override
    {
        return new json_string(*this);
    }

    virtual void serialize(OUT string& str) const override
    {
        str.clear();
        this->json_string::serialize_append(str);
    }

    virtual void serialize_append(OUT string& str) const override
    {
        str += '\"';
        str += str;
        str += '\"';
    }

public:
    static json_base* deserialize(const string& str, s64 from, s64 to)
    {
        trim_index(str, from, to);
        if (to - from < 2 || str[from] != '\"' || str[to - 1] != '\"')
        {
            return nullptr;
        }

        string value = "";

        s64 tail =
            iterate_quotation_json_string(
                str,
                from,
                [&value](char c)
                {
                    value += c;
                });

        if (tail != to - 1)
        {
            // error
            return nullptr;
        }

        return new json_string(value);
    }

private:
    string _value;
};
