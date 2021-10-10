#pragma once

/*
 * json_base
 * |
 * |_ json_object
 * |_ json_array
 * |_ json_int
 * |_ json_string
 * |_ json_boole
 * 
 */

class json_base;

class json_object;
class json_array;
class json_int;
class json_string;
class json_boole;

template<typename Fn_Ty>
void json_iterate(json_base* json, Fn_Ty fn, boole leaves_only = boole::False);

json_base* json_deserialize(const string& str, s64 from, s64 to);

enum class json_type : u64
{
    NONE = 9009,

    OBJECT,
    ARRAY,
    INT,
    FLOAT,
    STRING,
    BOOLE,
    JSON_NULL,
};

namespace JsonNs
{

class json_parent_context
{
public:
    json_parent_context() :
        parent_type(json_type::NONE),
        parent_json(nullptr),
        parent_order(-1),
        parent_key()
    {}

    json_parent_context(json_type type, json_base* parent, s64 order, string parent_key) :
        parent_type(type),
        parent_json(parent),
        parent_order(order),
        parent_key(parent_key)
    {}

    ~json_parent_context() = default;

    boole is_valid_parent()
    {
        return parent_type == json_type::OBJECT || parent_type == json_type::ARRAY;
    }

public:
    json_type  parent_type;
    json_base* parent_json;
    s64        parent_order;
    string     parent_key;
};

}

class json_base
{
    friend json_base* json_deserialize(const string& str, s64 from, s64 to);

public:
    json_base() :
        _parent()
    {}

    virtual ~json_base() = default;

    virtual json_type type() const = 0;

    virtual const char* type_name() const = 0;

    virtual s64 size() const = 0;

    virtual json_base* index(const string& key) = 0;

    virtual json_base* index(s64 order) = 0;

    virtual JsonNs::json_parent_context get_parent_context(s64 order) = 0;

    virtual string value() const = 0;

    virtual json_base* clone() const = 0;

    virtual void serialize(OUT string& str) const = 0;

    virtual void serialize_append(OUT string& str) const = 0;

public:
    static json_base* deserialize(const string& str, s64 from, s64 to)
    {
        return json_deserialize(str, from, to);
    }

    static void destroy(json_base* j)
    {
        delete j;
    }

public:
    JsonNs::json_parent_context& my_parent_context()
    {
        return _parent;
    }

    json_base* my_parent()
    {
        return _parent.parent_json;
    }

    string my_parent_key() const
    {
        return _parent.parent_key;
    }

    string my_path() const
    {
        return _parent.parent_json ?
            _parent.parent_json->my_path() + '[' + _parent.parent_key + ']' : "";
    }

    string my_path_index_string() const
    {
        string rst = _parent.parent_json ?
            _parent.parent_json->my_path_index_string() : "";

        string parent_key = _parent.parent_key;
        if (parent_key.size() >= 2 && parent_key.front() == '\"' && parent_key.back() == '\"')
        {
            parent_key = parent_key.substr(1, parent_key.size() - 2);
        }

        return rst + '.' + parent_key;
    }

    template<typename Fn_Ty>
    void iterate(Fn_Ty fn, boole leaves_only = boole::False)
    {
        json_iterate(this, fn, leaves_only);
    }

private:
    JsonNs::json_parent_context _parent;
};
