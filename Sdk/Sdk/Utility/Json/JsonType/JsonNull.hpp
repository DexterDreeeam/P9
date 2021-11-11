#pragma once

class json_null : public json_base
{
    template<typename Ty>
    friend class ref;

public:
    static const string Null;

private:
    json_null() :
        json_base()
    {}

    json_null(const json_null& rhs) = delete;

    json_null& operator =(const json_null& rhs) = delete;

public:
    virtual ~json_null() override = default;

public:
    static ref<json_null> new_instance()
    {
        auto rst = ref<json_null>::new_instance();
        rst->setup_self(rst);
        return rst;
    }

public:
    virtual json_type type() const override
    {
        return json_type::JSON_NULL;
    }

    virtual const char* type_name() const override
    {
        return "json_null";
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
        return Null;
    }

    virtual ref<json_base> clone() const override
    {
        auto rst = new_instance();
        rst->setup_self(rst);
        return rst;
    }

    virtual void serialize(_OUT_ string& str) const override
    {
        str.clear();
        this->json_null::serialize_append(str);
    }

    virtual void serialize_append(_OUT_ string& str) const override
    {
        str += Null;
    }

public:
    static ref<json_base> deserialize(const string& str, s64 from, s64 to)
    {
        trim_index(str, from, to);
        if (to - from == Null.size() && str.substr(from, to - from) == Null)
        {
            return new_instance();
        }
        return ref<json_base>();
    }

public:
    void* as_null() const
    {
        return nullptr;
    }
};

_SELECTANY_ const string json_null::Null = string("null");
