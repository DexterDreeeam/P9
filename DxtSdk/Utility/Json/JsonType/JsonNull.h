#pragma once

class json_null : public JsonNs::json_base
{
public:
    static const string Null;

public:
    json_null() :
        json_base()
    {}

    virtual ~json_null() override = default;

public:
    virtual JsonNs::json_type type() const override
    {
        return JsonNs::json_type::JSON_NULL;
    }

    virtual const char* type_name() const override
    {
        return "json_null";
    }

    virtual JsonNs::json_base* clone() const override
    {
        return new json_null();
    }

    virtual void serialize(OUT string& str) const override
    {
        str.clear();
        this->json_null::serialize_append(str);
    }

    virtual void serialize_append(OUT string& str) const override
    {
        str += Null;
    }

public:
    static json_base* deserialize(const string& str, s64 from, s64 to)
    {
        trim_index(str, from, to);
        if (to - from == Null.size() && str.substr(from, to - from) == Null)
        {
            return new json_null();
        }
        return nullptr;
    }
};

_SELECTANY_ const string json_null::Null = string("null");
