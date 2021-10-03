#pragma once

class json_boole : public JsonNs::json_base
{
public:
    json_boole() :
        _value(boole::False)
    {}

    json_boole(boole b) :
        _value(b)
    {}

    json_boole(const json_boole& rhs) :
        _value(rhs._value)
    {}

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

    virtual JsonNs::json_base* clone() const override
    {
        return new json_boole(*this);
    }

    virtual void serialize(OUT string& str) const override
    {
        str.clear();
        this->json_boole::serialize_append(str);
    }

    virtual void serialize_append(OUT string& str) const override
    {
        if (_value)
        {
            str += "True";
        }
        else
        {
            str += "False";
        }
    }

public:
    static json_base* deserialize(const string& str, s64 from, s64 to)
    {
        if (to - from == 4 && str.substr(from, to - from) == "True")
        {
            return new json_boole(boole::True);
        }
        if (to - from == 5 && str.substr(from, to - from) == "False")
        {
            return new json_boole(boole::False);
        }
        return nullptr;
    }

private:
    boole _value;
};
