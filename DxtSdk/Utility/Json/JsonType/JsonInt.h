#pragma once

class json_int : public JsonNs::json_base
{
public:
    json_int() = delete;

    json_int(s64 i) :
        json_base(),
        _value(i)
    {}

    json_int(const json_int& rhs) = delete;

    virtual ~json_int() override = default;

public:
    virtual JsonNs::json_type type() const override
    {
        return JsonNs::json_type::INT;
    }

    virtual const char* type_name() const override
    {
        return "json_int";
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
        s64 len = s64_to_text(_value, text);
        text[len] = 0;
        return string(text);
    }

    virtual JsonNs::json_base* clone() const override
    {
        return new json_int(_value);
    }

    virtual void Iterate(JsonNs::JsonIterateFunc function) override
    {
        function(this);
    }

    virtual void serialize(OUT string& str) const override
    {
        str.clear();
        this->json_int::serialize_append(str);
    }

    virtual void serialize_append(OUT string& str) const override
    {
        str += value();
    }

public:
    static json_base* deserialize(const string& str, s64 from, s64 to)
    {
        trim_index(str, from, to);
        s64 v;
        if (text_to_s64(str.data() + from, to - from, v))
        {
            return new json_int(v);
        }
        else
        {
            return nullptr;
        }
    }

public:
    s64 as_s64() const
    {
        return _value;
    }

private:
    s64 _value;
};
