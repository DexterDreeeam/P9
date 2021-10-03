#pragma once

class json_object : public JsonNs::json_base
{
public:
    json_object() :
        _items()
    {}

    json_object(const json_object& rhs) :
        _items(rhs._items)
    {}

    json_object(json_object&& rhs) noexcept :
        _items(right_value_type(rhs._items))
    {}

    virtual ~json_object() override
    {
        for (const auto& item : _items)
        {
            destroy(item.second);
        }
    }

public:
    virtual JsonNs::json_type type() const override
    {
        return JsonNs::json_type::OBJECT;
    }

    virtual JsonNs::json_base* clone() const override
    {
        return new json_object(*this);
    }

    virtual void serialize(OUT string& str) const override
    {
        str.clear();
        this->json_object::serialize_append(str);
    }

    virtual void serialize_append(OUT string& str) const override
    {
        str += '{';
        boole not_first = boole::False;
        for (const auto& item : _items)
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
    static JsonNs::json_base* deserialize(const string& str, s64 from, s64 to)
    {
        trim_index(str, OUT from, OUT to);

        if (from == to)
        {
            // error
            return nullptr;
        }
        if (str[from] != '{' || str[to - 1] != '}')
        {
            // error
            return nullptr;
        }
        ++from;
        --to;

        trim_index(str, from, to);

        if (from == to)
        {
            // empty object but valid
            return new json_object();
        }

        json_object* rst = new json_object();
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
                    auto* j = JsonNs::json_base::deserialize(str, value_start_from, from);
                    if (j == nullptr)
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
                    auto* j = JsonNs::json_base::deserialize(str, value_start_from, from);
                    if (j == nullptr)
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
    void add_item(const string& key, const JsonNs::json_base& value)
    {
        assert(
            _items.find(
                [&](const pair<string, JsonNs::json_base*>& p) -> boole
                {
                    return p.first == key;
                }
            ) == _items.end()
        );

        _items.push_back(make_pair<string, JsonNs::json_base*>(key, value.clone()));
    }

    void add_item(const string& key, JsonNs::json_base* value_ptr)
    {
        assert(
            _items.find(
                [&](const pair<string, JsonNs::json_base*>& p) -> boole
                {
                    return p.first == key;
                }
            ) == _items.end());

        _items.push_back(make_pair<string, JsonNs::json_base*>(key, value_ptr));
    }

    boole remove_item(const string& key)
    {
        for (int i = 0; i < _items.size(); ++i)
        {
            if (_items[i].first == key)
            {
                destroy(_items[i].second);
                _items.erase(i, 1);
            }
            return boole::True;
        }
        return boole::False;
    }

private:
    vector<pair<string, JsonNs::json_base*>> _items;
};
