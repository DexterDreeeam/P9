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

    ~json_object()
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
            return nullptr;
        }
        assert(str[from] == '{');
        assert(str[to - 1] == '}');
        ++from;
        --to;

        trim_index_from(str, OUT from, to);

        // todo

        return nullptr;
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
