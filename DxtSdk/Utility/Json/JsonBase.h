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

namespace JsonNs
{
class json_base;
}
class json_object;
class json_array;
class json_int;
class json_string;
class json_boole;

JsonNs::json_base* JsonDeserialize(const string& str, s64 from, s64 to);

namespace JsonNs
{

enum class json_type : u64
{
    BASE = 9009,

    OBJECT = BASE,
    ARRAY,
    INT,
    STRING,
    BOOLE,
    JSON_NULL,
};

class json_base
{
    friend json_base* ::JsonDeserialize(const string& str, s64 from, s64 to);

public:
    json_base() :
        _parent(nullptr)
    {}

    virtual ~json_base() = default;

    virtual json_type type() const = 0;

    virtual const char* type_name() const = 0;

    virtual json_base* clone() const = 0;

    virtual void serialize(OUT string& str) const = 0;

    virtual void serialize_append(OUT string& str) const = 0;

public:
    static json_base* deserialize(const string& str, s64 from, s64 to)
    {
        return JsonDeserialize(str, from, to);
    }

    static void destroy(json_base* j)
    {
        delete j;
    }

public:
    json_base*& my_parent()
    {
        return _parent;
    }

private:
    json_base* _parent;
};

}
