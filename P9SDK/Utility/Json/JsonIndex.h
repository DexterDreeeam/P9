#pragma once

_INLINE_ void json_report(JsonNs::json_base* json)
{
#if DEBUG_LEVEL >= DEBUG_LEVEL_CALIBRATION_LOG_NONE
    if (json == nullptr)
    {
        return;
    }
    json->Iterate(
        [](JsonNs::json_base* j)
        {
            switch (j->type())
            {
            case json_type::OBJECT:
            case json_type::ARRAY:
                return;
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
        _json(nullptr)
    {}

    json_cursor(JsonNs::json_base* json) :
        _json(json)
    {}

    ~json_cursor() = default;

    JsonNs::json_base* json()
    {
        return _json;
    }

    boole valid() const
    {
        return _json != nullptr;
    }

    boole invalid() const
    {
        return _json == nullptr;
    }

    json_cursor parent()
    {
        return json_cursor(_json->my_parent());
    }

    JsonNs::json_base* operator ->()
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

    boole as_boole() const
    {
        assert(_json->type() == json_type::BOOLE);
        return pointer_convert(_json, 0, json_boole*)->as_boole();
    }

    f64 as_f64() const
    {
        assert(_json->type() == json_type::FLOAT);
        return pointer_convert(_json, 0, json_float*)->as_f64();
    }

    s64 as_s64() const
    {
        assert(_json->type() == json_type::INT);
        return pointer_convert(_json, 0, json_int*)->as_s64();
    }

    string as_string() const
    {
        assert(_json->type() == json_type::STRING);
        return pointer_convert(_json, 0, json_string*)->as_string();
    }

private:
    JsonNs::json_base* _json;
};
