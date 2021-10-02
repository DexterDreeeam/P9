#pragma once

/*
 * json_base
 * |
 * |_ json_object
 * |_ json_array
 * |_ json_int
 * |_ json_string
 * |_ json_boole
 * |_ json_time
 * |_ json_timestamp
 * 
 */

class json_object;
class json_array;

const char blank_chars[] = { ' ', '\n', '\t' };

class json_base
{
public:

    virtual boole is_object() = 0;

    virtual boole is_array() = 0;

    virtual boole is_int() = 0;

    virtual boole is_string() = 0;

    virtual boole is_boole() = 0;

    virtual boole is_time() = 0;

    virtual boole is_timestamp() = 0;

    virtual json_object* as_json_object() = 0;

    virtual json_array* as_json_array() = 0;

    virtual void serialize(string& str) = 0;

    virtual void serialize_append(string& str) = 0;

    virtual json_base* deserialize(const string& str) = 0;

    static json_base* deserialize(const string& str, s64 pos = 0)
    {
        s64 len = str.size();
        while (pos < len)
        {
            char ch = str[pos];
            for (char c : blank_chars)
            {
                if (c == ch)
                {
                    goto L_next;
                }
            }
            if (ch == '{')
            {
                return json_object::deserialize(str, pos);
            }
            if (ch == '[')
            {
                return json_array::deserialize(str, pos);
            }
            assert_info(0, "encounter unexpected charactor");

        L_next:
            ++pos;
        }
    }
};
