#pragma once

class json_boole : public json_base
{
    friend class ref_base;

    template<typename Ty>
    friend class ref;

public:
    static const string True;
    static const string False;

private:
    json_boole() :
        json_base(),
        _value(boole::False)
    {}

    json_boole(boole b) :
        json_base(),
        _value(b)
    {}

    json_boole(const json_boole& rhs) = delete;

public:
    virtual ~json_boole() override = default;

public:
    static ref<json_boole> new_instance(boole b)
    {
        auto rst = ref<json_boole>::new_instance(b);
        rst->setup_self(rst.observer());
        return rst;
    }

public:
    virtual json_type type() const override
    {
        return json_type::BOOLE;
    }

    virtual const char* type_name() const override
    {
        return "json_boole";
    }

    virtual s64 size() const override
    {
        return 0;
    }

    virtual ref<json_base> index(const string& key) override
    {
        assert(0);
        return ref<json_base>();
    }

    virtual ref<json_base> index(s64 order) override
    {
        assert(0);
        return ref<json_base>();
    }

    virtual JsonNs::json_parent_context get_parent_context(const string&) override
    {
        assert(0);
        return JsonNs::json_parent_context();
    }

    virtual string value() const override
    {
        return _value ? True : False;
    }

    virtual ref<json_base> clone() const override
    {
        auto rst = new_instance(_value);
        rst->setup_self(rst.observer());
        return rst;
    }

    virtual void serialize(_OUT_ string& str) const override
    {
        str.clear();
        this->json_boole::serialize_append(str);
    }

    virtual void serialize_append(_OUT_ string& str) const override
    {
        str += value();
    }

public:
    static ref<json_base> deserialize(const string& str, s64 from, s64 to)
    {
        trim_index(str, from, to);
        string sub = str.substr(from, to - from);
        if (to - from == True.size() && str.substr(from, to - from) == True)
        {
            return new_instance(boole::True);
        }
        if (to - from == False.size() && str.substr(from, to - from) == False)
        {
            return new_instance(boole::False);
        }
        return ref<json_base>();
    }

public:
    boole as_boole() const
    {
        return _value;
    }

private:
    boole _value;
};

_SELECTANY_ const string json_boole::True  = string("true");
_SELECTANY_ const string json_boole::False = string("false");
