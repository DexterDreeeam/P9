#pragma once

/*
 * json_base
 * |
 * |_ json_object
 * |_ json_array
 * |_ json_number
 * |_ json_string
 * |_ json_boole
 * |_ json_null
 * 
 */

class json_base;

class json_object;
class json_array;
class json_string;
class json_boole;
class json_number;
class json_null;

template<typename Fn_Ty>
void json_iterate(ref<json_base> json, Fn_Ty fn, boole leaves_only = boole::False);

ref<json_base> json_deserialize(const string& str, s64 from, s64 to);

enum class json_type : u64
{
    NONE = 9009,

    OBJECT,
    ARRAY,
    NUMBER,
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
        parent_json(),
        parent_order(-1),
        parent_key()
    {}

    json_parent_context(const json_parent_context& rhs) :
        parent_type(rhs.parent_type),
        parent_json(rhs.parent_json),
        parent_order(rhs.parent_order),
        parent_key(rhs.parent_key)
    {}

    json_parent_context& operator =(const json_parent_context& rhs)
    {
        parent_type = rhs.parent_type;
        parent_json = rhs.parent_json;
        parent_order = rhs.parent_order;
        parent_key = rhs.parent_key;
        return *this;
    }

    ~json_parent_context() = default;

    boole is_valid_parent()
    {
        return parent_type == json_type::OBJECT || parent_type == json_type::ARRAY;
    }

public:
    json_type       parent_type;
    obs<json_base>  parent_json;
    s64             parent_order;
    string          parent_key;
};

}

class json_base
{
    friend class json_object;
    friend class json_array;
    friend class json_string;
    friend class json_boole;
    friend class json_number;
    friend class json_null;
    friend ref<json_base> json_deserialize(const string& str, s64 from, s64 to);

private:
    json_base() :
        _parent(),
        _self()
    {}

    void setup_self(obs<json_base> self)
    {
        _self = self;
    }

    void setup_parent(const JsonNs::json_parent_context& prnt)
    {
        _parent = prnt;
    }

public:
    virtual ~json_base() = default;

    virtual json_type type() const = 0;

    virtual const char* type_name() const = 0;

    virtual s64 size() const = 0;

    virtual ref<json_base> index(const string& key) = 0;

    virtual ref<json_base> index(s64 order) = 0;

    virtual JsonNs::json_parent_context get_parent_context(const string& key = "") = 0;

    virtual string value() const = 0;

    virtual ref<json_base> clone() const = 0;

    virtual void serialize(_OUT_ string& str) const = 0;

    virtual void serialize_append(_OUT_ string& str) const = 0;

public:
    static ref<json_base> deserialize(const string& str, s64 from, s64 to)
    {
        return json_deserialize(str, from, to);
    }

public:
    JsonNs::json_parent_context& my_parent_context()
    {
        return _parent;
    }

    ref<json_base> my_parent()
    {
        return _parent.parent_json.try_ref();
    }

    string my_parent_key() const
    {
        return _parent.parent_key;
    }

    string my_path()
    {
        return _parent.parent_json.has_value() ?
            _parent.parent_json.try_ref()->my_path() + '[' + _parent.parent_key + ']' : "";
    }

    string my_path_index_string()
    {
        if (_parent.parent_json.has_value() == boole::False)
        {
            return "";
        }
        string rst = _parent.parent_json.try_ref()->my_path_index_string();
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
        json_iterate(_self.try_ref(), fn, leaves_only);
    }

private:
    JsonNs::json_parent_context _parent;
    obs<json_base>         _self;
};
