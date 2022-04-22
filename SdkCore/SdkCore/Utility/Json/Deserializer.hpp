#pragma once

_INLINE_ ref<json_base> json_deserialize(const string& str, s64 from, s64 to)
{
    trim_index(str, from, to);

    if (from == to)
    {
        return ref<json_base>();
    }
    ref<json_base> rst = ref<json_base>();

    //===================================================== json_object
    rst = json_object::deserialize(str, from, to);
    if (rst.has_value())
    {
        goto L_finish;
    }

    //===================================================== json_array
    rst = json_array::deserialize(str, from, to);
    if (rst.has_value())
    {
        goto L_finish;
    }

    //===================================================== json_string
    rst = json_string::deserialize(str, from, to);
    if (rst.has_value())
    {
        goto L_finish;
    }

    //===================================================== json_boole
    rst = json_boole::deserialize(str, from, to);
    if (rst.has_value())
    {
        goto L_finish;
    }

    //===================================================== json_null
    rst = json_null::deserialize(str, from, to);
    if (rst.has_value())
    {
        goto L_finish;
    }

    //===================================================== json_float
    rst = json_number::deserialize(str, from, to);
    if (rst.has_value())
    {
        goto L_finish;
    }

    assert_info(0, "encounter unexpected charactor");
    return ref<json_base>();

L_finish:
    return rst;
}

_INLINE_ ref<json_base> json_deserialize(const string& str)
{
    return json_deserialize(str, 0, str.size());
}

_INLINE_ ref<json_base> json_deserialize_from_file(const string& path)
{
    string content = "";
    char buf[1024];
    file f;
    s64 try_read_len = 512;
    s64 len = try_read_len;
    if (!f.init_input(path.data()))
    {
        return ref<json_base>();
    }
    escape_function ef =
        [=]() mutable
        {
            f.uninit();
        };
    while (len == try_read_len)
    {
        f.input(buf, try_read_len, len);
        buf[len] = 0;
        content += buf;
    }
    return json_deserialize(content);
}
