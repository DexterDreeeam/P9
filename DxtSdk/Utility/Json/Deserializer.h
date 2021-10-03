#pragma once

JsonNs::json_base* JsonDeserialize(const string& str, s64 from, s64 to)
{
    JsonNs::json_base::trim_index(str, from, to);

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

JsonNs::json_base* JsonDeserialize(const string& str)
{
    return JsonDeserialize(str, 0, str.size());
}
