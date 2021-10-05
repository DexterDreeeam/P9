#pragma once

_INLINE_ JsonNs::json_base* JsonDeserialize(const string& str, s64 from, s64 to)
{
    trim_index(str, from, to);

    if (from == to)
    {
        return nullptr;
    }
    JsonNs::json_base* rst = nullptr;

    //===================================================== json_object
    rst = json_object::deserialize(str, from, to);
    if (rst)
    {
        goto L_finish;
    }

    //===================================================== json_array
    rst = json_array::deserialize(str, from, to);
    if (rst)
    {
        goto L_finish;
    }

    //===================================================== json_string
    rst = json_string::deserialize(str, from, to);
    if (rst)
    {
        goto L_finish;
    }

    //===================================================== json_boole
    rst = json_boole::deserialize(str, from, to);
    if (rst)
    {
        goto L_finish;
    }

    //===================================================== json_null
    rst = json_null::deserialize(str, from, to);
    if (rst)
    {
        goto L_finish;
    }

    //===================================================== json_float
    rst = json_float::deserialize(str, from, to);
    if (rst)
    {
        goto L_finish;
    }

    //===================================================== json_int
    rst = json_int::deserialize(str, from, to);
    if (rst)
    {
        goto L_finish;
    }

    assert_info(0, "encounter unexpected charactor");
    return nullptr;

L_finish:
    return rst;
}

_INLINE_ JsonNs::json_base* JsonDeserialize(const string& str)
{
    return JsonDeserialize(str, 0, str.size());
}
