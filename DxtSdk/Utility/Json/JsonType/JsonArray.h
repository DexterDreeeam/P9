#pragma once

class json_array : public JsonNs::json_base
{
public:
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

    virtual ~json_array() override
    {
        for (auto* item : _items)
        {
            destroy(item);
        }
    }

public:
    virtual JsonNs::json_type type() const override
    {
        return JsonNs::json_type::ARRAY;
    }

    virtual const char* type_name() const override
    {
        return "json_array";
    }

    virtual s64 size() const override
    {
        return _items.size();
    }

    virtual JsonNs::json_parent_context get_parent_context(s64 order) override
    {
        assert(order >= 0 && order < size());
        char buf[32];
        s64 text_len = s64_to_text(order, buf);
        buf[text_len] = 0;
        return JsonNs::json_parent_context(type(), this, order, string(buf));
    }

    virtual string element_value() const override
    {
        return string();
    }

    virtual JsonNs::json_base* clone() const override
    {
        json_array* rst = new json_array();
        for (auto* i : _items)
        {
            rst->add_item(i->clone());
        }
        return rst;
    }

    virtual void Iterate(JsonNs::JsonIterateFunc function) override
    {
        function(this);
        for (auto* i : _items)
        {
            i->Iterate(function);
        }
    }

    virtual void serialize(OUT string& str) const override
    {
        str.clear();
        this->json_array::serialize_append(str);
    }

    virtual void serialize_append(OUT string& str) const override
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

    static json_base* deserialize(const string& str, s64 from, s64 to);

public:
    void add_item(const json_base& json)
    {
        _items.push_back(json.clone());
        _items.back()->my_parent_context() = get_parent_context(_items.size() - 1);
    }

    void add_item(json_base* json)
    {
        _items.push_back(json);
        _items.back()->my_parent_context() = get_parent_context(_items.size() - 1);
    }

private:
    template<typename ...Args>
    void constructor_iterator(const json_base& jo, Args ...args)
    {
        _items.push_back(jo.clone());
        constructor_iterator(args...);
    }

    void constructor_iterator(const json_base& jo)
    {
        _items.push_back(jo.clone());
    }

private:
    vector<json_base*> _items;
};

_INLINE_ JsonNs::json_base* json_array::deserialize(const string& str, s64 from, s64 to)
{
    trim_index(str, from, to);
    if (from >= to)
    {
        // error
        return nullptr;
    }
    if (str[from] != '[' || str[to - 1] != ']')
    {
        // error
        return nullptr;
    }
    ++from;
    --to;

    json_array* rst = new json_array();
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
                auto* j = JsonNs::json_base::deserialize(str, value_from_pos, from);
                if (j == nullptr)
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
                auto* j = JsonNs::json_base::deserialize(str, value_from_pos, from);
                if (j == nullptr)
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
    if (rst)
    {
        delete rst;
        rst = nullptr;
    }
    return nullptr;

L_process_finish:
    return rst;
}
