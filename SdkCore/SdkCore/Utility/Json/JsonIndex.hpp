#pragma once

template<typename Fn_Ty>
_INLINE_ void json_iterate(ref<json_base> json, Fn_Ty fn, boole leaves_only)
{
    if (json.empty())
    {
        return;
    }

    if (!leaves_only)
    {
        fn(json);
    }

    switch (json->type())
    {
    case json_type::OBJECT:
        if (!leaves_only)
        {
            fn(json);
        }
        for (auto& p : json.ref_of<json_object>()->_items)
        {
            json_iterate(p.second, fn, leaves_only);
        }
        break;
    case json_type::ARRAY:
        if (!leaves_only)
        {
            fn(json);
        }
        for (auto i : json.ref_of<json_array>()->_items)
        {
            json_iterate(i, fn, leaves_only);
        }
        break;
    case json_type::BOOLE:
    case json_type::NUMBER:
    case json_type::JSON_NULL:
    case json_type::STRING:
        fn(json);
    default:
        break;
    }
}

_INLINE_ void json_report(ref<json_base> json)
{
#if DEBUG_LEVEL >= DEBUG_LEVEL_CALIBRATION_LOG_NONE
    if (json.empty())
    {
        return;
    }
    json_iterate(
        json,
        [](ref<json_base> j)
        {
            switch (j->type())
            {
            case json_type::OBJECT:
            case json_type::ARRAY:
                return;
            default:
                break;
            }
            string path  = j->my_path();
            string value = j->value();
            print("%s %s\n", path.data(), value.data());
        });
#endif
}

class json_cursor
{
public:
    json_cursor() :
        _json()
    {}

    json_cursor(ref<json_base> json) :
        _json(json)
    {}

    ~json_cursor() = default;

    ref<json_base> json()
    {
        return _json;
    }

    boole valid() const
    {
        return _json.has_value();
    }

    boole invalid() const
    {
        return _json.empty();
    }

    json_cursor parent()
    {
        return json_cursor(_json->my_parent());
    }

    ref<json_base> operator ->()
    {
        return _json;
    }

    json_cursor operator[](const string& key)
    {
        return json_cursor(_json->index(key));
    }

    json_cursor operator[](s64 order)
    {
        return json_cursor(_json->index(order));
    }

    string operator *()
    {
        return _json->value();
    }

    boole as_boole()
    {
        assert(_json->type() == json_type::BOOLE);
        return _json.ref_of<json_boole>()->as_boole();
    }

    number as_number()
    {
        assert(_json->type() == json_type::NUMBER);
        return _json.ref_of<json_number>()->as_number();
    }

    string as_string()
    {
        assert(_json->type() == json_type::STRING);
        return _json.ref_of<json_string>()->as_string();
    }

private:
    ref<json_base> _json;
};
