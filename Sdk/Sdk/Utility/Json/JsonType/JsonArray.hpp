#pragma once

class json_array : public json_base
{
    template<typename Fn_Ty>
    friend void json_iterate(ref<json_base> json, Fn_Ty fn, boole leaves_only);

    friend class ref_base;

    template<typename Ty>
    friend class ref;

private:
    json_array() :
        json_base(),
        _items()
    {
    }

    json_array(const json_array& rhs) = delete;

    template<typename ...Args>
    json_array(Args ...args) :
        json_base(),
        _items()
    {
        constructor_iterator(args...);
    }

public:
    virtual ~json_array() override
    {
    }

public:
    static ref<json_array> new_instance()
    {
        auto rst = ref<json_array>::new_instance();
        rst->setup_self(rst.observer());
        return rst;
    }

public:
    virtual json_type type() const override
    {
        return json_type::ARRAY;
    }

    virtual const char* type_name() const override
    {
        return "json_array";
    }

    virtual s64 size() const override
    {
        return _items.size();
    }

    virtual ref<json_base> index(const string& key) override
    {
        s64 order;
        if (text_to_s64(key.data(), key.size(), order))
        {
            return index(order);
        }
        else
        {
            return ref<json_base>();
        }
    }

    virtual ref<json_base> index(s64 order) override
    {
        if (order >= 0 && order < _items.size())
        {
            return _items[order];
        }
        else
        {
            return ref<json_base>();
        }
    }

    virtual JsonNs::json_parent_context get_parent_context(const string& str = "") override
    {
        char buf[32];
        s64 text_len = s64_to_text(size(), buf);
        buf[text_len] = 0;

        JsonNs::json_parent_context ctx;
        ctx.parent_type = type();
        ctx.parent_json = _self;
        ctx.parent_order = size();
        ctx.parent_key = string(buf);
        return ctx;
    }

    virtual string value() const override
    {
        string rst;
        serialize(rst);
        return rst;
    }

    virtual ref<json_base> clone() const override
    {
        auto copy = new_instance();
        for (auto r : _items)
        {
            copy->add_item(r->clone());
        }
        return copy;
    }

    virtual void serialize(_OUT_ string& str) const override
    {
        str.clear();
        this->json_array::serialize_append(str);
    }

    virtual void serialize_append(_OUT_ string& str) const override
    {
        str += '[';
        boole not_first = boole::False;
        for (auto item : _items)
        {
            if (not_first)
            {
                str += ',';
            }
            else
            {
                not_first = boole::True;
            }
            item->serialize_append(str);
        }
        str += ']';
    }

    static ref<json_base> deserialize(const string& str, s64 from, s64 to);

public:
    void add_item(ref<json_base> json)
    {
        json->setup_parent(get_parent_context());
        _items.push_back(json);
    }

private:
    template<typename ...Args>
    void constructor_iterator(ref<json_base> jo, Args ...args)
    {
        _items.push_back(jo);
        constructor_iterator(args...);
    }

    void constructor_iterator(ref<json_base> jo)
    {
        _items.push_back(jo);
    }

private:
    vector<ref<json_base>> _items;
};

_INLINE_ ref<json_base> json_array::deserialize(const string& str, s64 from, s64 to)
{
    trim_index(str, from, to);
    if (from >= to)
    {
        // error
        return ref<json_base>();
    }
    if (str[from] != '[' || str[to - 1] != ']')
    {
        // error
        return ref<json_base>();
    }
    ++from;
    --to;

    auto rst = json_array::new_instance();
    while (1)
    {
        trim_index_from(str, from, to);
        if (from == to)
        {
            // complete
            goto L_process_finish;
        }
        if (from > to || str[from] == ',')
        {
            // error
            goto L_process_error;
        }

        s64 value_from_pos = from;
        s64 curly_brace_cnt = 0;
        s64 bracket_cnt = 0;

        while (1)
        {
            // search for json value tail
            if (from == to && (curly_brace_cnt != 0 || bracket_cnt != 0))
            {
                // error
                goto L_process_error;
            }
            if (from == to)
            {
                auto j = json_base::deserialize(str, value_from_pos, from);
                if (j.invalid())
                {
                    // error
                    goto L_process_error;
                }
                rst->add_item(j);
                // complte
                goto L_process_finish;
            }
            char c = str[from];
            if (c == ',' && curly_brace_cnt == 0 && bracket_cnt == 0)
            {
                auto j = json_base::deserialize(str, value_from_pos, from);
                if (j.invalid())
                {
                    // error
                    goto L_process_error;
                }
                rst->add_item(j);
                ++from;
                break; // loop back
            }
            s64 pos = 0;
            switch (c)
            {
            case '\"':
                pos = iterate_quotation_json_string(str, from, [](char) {});
                if (pos < 0 || pos >= to)
                {
                    // error
                    goto L_process_error;
                }
                from = pos;
                break;
            case '{':
                ++curly_brace_cnt;
                break;
            case '}':
                --curly_brace_cnt;
                break;
            case '[':
                ++bracket_cnt;
                break;
            case ']':
                --bracket_cnt;
                break;
            default:
                break;
            }
            ++from;
        }
    }

L_process_error:
    return ref<json_base>();

L_process_finish:
    return rst;
}
