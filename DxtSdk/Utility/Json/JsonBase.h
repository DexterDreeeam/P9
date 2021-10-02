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

class json_object;
class json_array;
class json_int;
class json_string;
class json_boole;

namespace JsonNs
{

const char blank_chars[] = { ' ', '\n', '\t' };

enum class json_type : u64
{
    BASE = 9009,

    OBJECT = BASE,
    ARRAY,
    INT,
    STRING,
    BOOLE
};

class json_base
{
public:
    virtual json_type type() const = 0;

    virtual json_base* clone() const = 0;

    virtual void serialize(OUT string& str) const = 0;

    virtual void serialize_append(OUT string& str) const = 0;

public:
    static json_base* deserialize(const string& str, s64 from, s64 to);

    static void destroy(json_base* j)
    {
        delete j;
    }

protected:
    static void trim_index_from(const string& str, OUT s64& from, s64 to)
    {
        while (from < to && is_blank_char(str[from]))
        {
            ++from;
        }
    }

    static void trim_index_to(const string& str, s64 from, OUT s64& to)
    {
        while (to > from && is_blank_char(str[to - 1]))
        {
            --to;
        }
    }

    static void trim_index(const string& str, OUT s64& from, OUT s64& to)
    {
        trim_index_from(str, from, to);
        trim_index_to(str, from, to);
    }

    static boole is_blank_char(char ch)
    {
        for (char c : blank_chars)
        {
            if (c == ch)
            {
                return boole::True;
            }
        }
        return boole::False;
    }
};

}


#include "JsonType/JsonArray.h"
#include "JsonType/JsonBoole.h"
#include "JsonType/JsonInt.h"
#include "JsonType/JsonObject.h"
#include "JsonType/JsonString.h"


JsonNs::json_base* JsonNs::json_base::deserialize(const string& str, s64 from, s64 to)
{
    trim_index(str, OUT from, OUT to);

    if (from == to)
    {
        return nullptr;
    }
    char ch = str[from];
    if (ch == '{')
    {
        // json object
        return json_object::deserialize(str, from, to);
    }
    if (ch == '[')
    {
        // json array
        return json_array::deserialize(str, from, to);
    }
    if (ch == '\"')
    {
        // json string
        return json_string::deserialize(str, from, to);
    }
    if (is_digit(ch))
    {
        // json int
        return json_int::deserialize(str, from, to);
    }
    if (to - from == 4 && str.substr(from, to - from) == "True")
    {
        return new json_boole(boole::True);
    }
    if (to - from == 5 && str.substr(from, to - from) == "False")
    {
        return new json_boole(boole::False);
    }
    assert_info(0, "encounter unexpected charactor");
    return nullptr;
}
