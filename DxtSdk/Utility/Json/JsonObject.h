#pragma once

#include "../Container/Unsafe/String.h"
#include "../Container/Unsafe/Pair.h"
#include "../Container/Unsafe/Vector.h"
#include "JsonBase.h"

class json_object : protected json_base
{
public:
    json_object() :
        _items()
    {}

    json_object(const json_object& rhs) :
        _items(rhs._items)
    {}

    json_object(json_object&& rhs) :
        _items(right_value_type(rhs._items))
    {}

public:
    virtual boole is_object() override
    {
        return boole_true;
    }

    virtual boole is_array() override
    {
        return boole_false;
    }

    virtual json_object* as_json_object() override
    {
        return this;
    }

    virtual json_array* as_json_array() override
    {
        assert(0);
        return nullptr;
    }

    virtual void serialize(string& str) override
    {
        str.clear();
        this->json_object::serialize_append(str);
    }

    virtual void serialize_append(string& str) override
    {
        str += '{';
        boole not_first = boole_false;
        for (auto item : _items)
        {
            if (not_first)
            {
                str += ',';
            }
            else
            {
                not_first = boole_true;
            }
            str += '\"';
            str += item.first;
            str += '\"';

            str += ':';
            string item_str;
            item.second.serialize(item_str);
            str += item_str;
        }
        str += '}';
    }

    static json_base* deserialize(const string& str, s64 pos)
    {
        return nullptr;
    }

public:
    void add_item(const string& key, const json_base& value)
    {
        assert(
            _items.find(
                [&](const string& x) -> boole
                {
                    return x == key;
                }
            ) != _items.end()
        );

        _items.push_back(make_pair(key, value));
    }

    boole remove_item(const string& key)
    {
        for (int i = 0; i < _items.size(); ++i)
        {
            if (_items[i].first == key)
            {
                _items.erase(i, 1);
            }
            return boole_true;
        }
        return boole_false;
    }

private:
    vector<pair<string, json_base>> _items;
};
