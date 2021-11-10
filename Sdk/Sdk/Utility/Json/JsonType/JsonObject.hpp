#pragma once

class json_object : public json_base
{
    friend class ref_base;

    template<typename Ty>
    friend class ref;

    template<typename Fn_Ty>
    friend void json_iterate(ref<json_base> json, Fn_Ty fn, boole leaves_only);

private:
    json_object() :
        json_base(),
        _items()
    {}

    json_object(const json_object& rhs) = delete;

    json_object& operator =(const json_object& rhs) = delete;

public:
    virtual ~json_object() override
    {
    }

public:
    static ref<json_object> new_instance()
    {
        auto rst = ref<json_object>::new_instance();
        rst->setup_self(rst.observer());
        return rst;
    }

public:
    virtual json_type type() const override
    {
        return json_type::OBJECT;
    }

    virtual const char* type_name() const override
    {
        return "json_object";
    }

    virtual s64 size() const override
    {
        return _items.size();
    }

    virtual ref<json_base> index(const string& key) override
    {
        for (auto& p : _items)
        {
            if (p.first == key)
            {
                return p.second;
            }
        }
        return ref<json_base>();
    }

    virtual ref<json_base> index(s64 order) override
    {
        if (order >= 0 && order < _items.size())
        {
            return _items[order].second;
        }
        else
        {
            return ref<json_base>();
        }
    }

    virtual JsonNs::json_parent_context get_parent_context(const string& key) override
    {
        JsonNs::json_parent_context ctx;
        ctx.parent_type = type();
        ctx.parent_json = _self;
        ctx.parent_order = size();
        ctx.parent_key = '\"' + key + '\"';
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
        auto rst = new_instance();
        for (auto p : _items)
        {
            rst->add_item(p.first, p.second->clone());
        }
        return rst;
    }

    virtual void serialize(_OUT_ string& str) const override
    {
        str.clear();
        this->json_object::serialize_append(str);
    }

    virtual void serialize_append(_OUT_ string& str) const override
    {
        str += '{';
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
            str += '\"';
            str += item.first; // key
            str += '\"';

            str += ':';
            item.second->serialize_append(str); // value
        }
        str += '}';
    }

public:
    static ref<json_base> deserialize(const string& str, s64 from, s64 to);

public:
    void add_item(const string& key, ref<json_base> value)
    {
        assert(
            _items.find(
                [&](const pair<string, ref<json_base>>& p) -> boole
                {
                    return p.first == key;
                }
            ) == _items.end()
        );

        value->setup_parent(get_parent_context(key));
        _items.push_back(make_pair<string, ref<json_base>>(key, value));
    }

private:
    vector<pair<string, ref<json_base>>> _items;
};

_INLINE_ ref<json_base> json_object::deserialize(const string& str, s64 from, s64 to)
{
    trim_index(str, _OUT_ from, _OUT_ to);

    if (from == to)
    {
        // error
        return ref<json_base>();
    }
    if (str[from] != '{' || str[to - 1] != '}')
    {
        // error
        return ref<json_base>();
    }
    ++from;
    --to;

    trim_index(str, from, to);

    if (from == to)
    {
        // empty object but valid
        return new_instance();
    }

    auto rst = json_object::new_instance();
    while (1)
    {
        //================================================= search key
        if (from > to)
        {
            // error
            goto L_process_error;
        }
        trim_index_from(str, from, to);
        if (from == to)
        {
            // complete
            goto L_process_finish;
        }
        if (str[from] != '\"')
        {
            // error
            goto L_process_error;
        }
        string key = "";
        s64 key_tail = iterate_quotation_json_string(str, from, [&key](char c) { key += c; });
        if (key_tail < 0)
        {
            // error
            goto L_process_error;
        }
        from = key_tail + 1;

        //================================================= search colon
        if (from >= to)
        {
            // error
            goto L_process_error;
        }
        trim_index_from(str, from, to);
        if (str[from] != ':')
        {
            // error
            goto L_process_error;
        }
        ++from;

        //================================================= search tail comma
        trim_index_from(str, from, to);
        if (from >= to)
        {
            // error
            goto L_process_error;
        }
        s64 value_start_from = from;
        s64 curly_brace_cnt = 0;
        s64 bracket_cnt = 0;
        while (1)
        {
            if (from == to)
            {
                auto j = json_base::deserialize(str, value_start_from, from);
                if (j.invalid())
                {
                    // error
                    goto L_process_error;
                }
                rst->add_item(key, j);
                goto L_process_finish; // complete
            }
            char c = str[from];
            if (c == ',' && curly_brace_cnt == 0 && bracket_cnt == 0)
            {
                auto j = json_base::deserialize(str, value_start_from, from);
                if (j.invalid())
                {
                    // error
                    goto L_process_error;
                }
                rst->add_item(key, j);
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
