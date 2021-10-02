#pragma once

#include "../Container/Interface.h"
#include "JsonObject.h"

class json_array : protected json_base
{
public:
    json_array() :
        _items()
    {
    }

    template<typename ...Args>
    json_array(Args ...args) :
        _items()
    {
        compose_jobject_vector(_items, args...);
    }

    ~json_array() = default;

public:
    virtual boole is_object() override
    {
        return boole_false;
    }

    virtual boole is_array() override
    {
        return boole_true;
    }

    virtual json_object* as_json_object() override
    {
        assert(0);
        return nullptr;
    }

    virtual json_array* as_json_array() override
    {
        return this;
    }

    virtual void serialize(string& str) override
    {
        str.clear();
        this->json_array::serialize_append(str);
    }

    virtual void serialize_append(string& str) override
    {
        str += '[';
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
            string item_str;
            item.serialize(item_str);
            str += item_str;
        }
        str += ']';
    }

    static json_base* deserialize(const string& str, s64 pos)
    {
        return nullptr;
    }

private:
    template<typename ...Args>
    void compose_jobject_vector(vector<json_base*>& vec, const json_object& jo, Args ...args)
    {
        vec.push_back(jo);
        compose_jobject_vector(vec, args...);
    }

    template<typename ...Args>
    void compose_jobject_vector(vector<json_base*>& vec, json_object&& jo, Args ...args)
    {
        vec.push_back(jo);
        compose_jobject_vector(vec, args...);
    }

    void compose_jobject_vector(vector<json_base*>& vec, const json_object& jo)
    {
        vec.push_back(jo);
    }

    void compose_jobject_vector(vector<json_base*>& vec, json_object&& jo)
    {
        vec.push_back(jo);
    }

private:
    vector<json_base*> _items;
};
