#pragma once

class json_boole : public JsonNs::json_base
{
public:
    static const string True;
    static const string False;

public:
    json_boole() :
        json_base(),
        _value(boole::False)
    {}

    json_boole(boole b) :
        json_base(),
        _value(b)
    {}

    json_boole(const json_boole& rhs) = delete;

    virtual ~json_boole() override = default;

public:
    virtual JsonNs::json_type type() const override
    {
        return JsonNs::json_type::BOOLE;
    }

    virtual const char* type_name() const override
    {
        return "json_boole";
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
        return _value ? True : False;
    }

    virtual JsonNs::json_base* clone() const override
    {
        return new json_boole(_value);
    }

    virtual void Iterate(JsonNs::JsonIterateFunc function) override
    {
        function(this);
    }

    virtual void serialize(OUT string& str) const override
    {
        str.clear();
        this->json_boole::serialize_append(str);
    }

    virtual void serialize_append(OUT string& str) const override
    {
        str += element_value();
    }

public:
    static json_base* deserialize(const string& str, s64 from, s64 to)
    {
        trim_index(str, from, to);
        string sub = str.substr(from, to - from);
        if (to - from == True.size() && str.substr(from, to - from) == True)
        {
            return new json_boole(boole::True);
        }
        if (to - from == False.size() && str.substr(from, to - from) == False)
        {
            return new json_boole(boole::False);
        }
        return nullptr;
    }

private:

private:
    boole _value;
};

_SELECTANY_ const string json_boole::True  = string("true");
_SELECTANY_ const string json_boole::False = string("false");
