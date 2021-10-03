#pragma once

class json_string : public JsonNs::json_base
{
public:
    json_string() :
        json_base(),
        _value()
    {}

    json_string(const string& s) :
        json_base(),
        _value(s)
    {}

    json_string(string&& s) :
        json_base(),
        _value(right_value_type(s))
    {}

    json_string(const json_string& rhs) = delete;

    virtual ~json_string() override = default;

public:
    virtual JsonNs::json_type type() const override
    {
        return JsonNs::json_type::STRING;
    }

    virtual const char* type_name() const override
    {
        return "json_string";
    }

    virtual s64 size() const override
    {
        return 0;
    }

    virtual JsonNs::json_parent_context get_parent_context(s64 order) override
    {
        assert(0);
        return JsonNs::json_parent_context();
    }

    virtual string element_value() const override
    {
        return '\"' + _value + '\"';
    }

    virtual JsonNs::json_base* clone() const override
    {
        return new json_string(_value);
    }

    virtual void Iterate(JsonNs::JsonIterateFunc function) override
    {
        function(this);
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
