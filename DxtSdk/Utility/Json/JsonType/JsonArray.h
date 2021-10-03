#pragma once

class json_array : public JsonNs::json_base
{
public:
    json_array() :
        _items()
    {
    }

    json_array(const json_array& rhs) :
        _items()
    {
        for (const auto& item : rhs._items)
        {
            _items.push_back(item->clone());
        }
    }

    template<typename ...Args>
    json_array(Args ...args) :
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
        return "json_type";
    }

    virtual JsonNs::json_base* clone() const override
    {
        return new json_array(*this);
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

    static json_base* deserialize(const string& str, s64 from, s64 to)
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

public:
    void add_item(const json_base& json)
    {
        _items.push_back(json.clone());
    }

    void add_item(json_base* json)
    {
        _items.push_back(json);
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