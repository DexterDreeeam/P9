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
            case JsonNs::json_type::OBJECT:
            case JsonNs::json_type::ARRAY:
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
    json_cursor(JsonNs::json_base* json) :
        _json(json)
    {}

    ~json_cursor() = default;

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
        assert(_json->type() == JsonNs::json_type::BOOLE);
        return pointer_convert(_json, 0, json_boole*)->as_boole();
    }

    f64 as_f64() const
    {
        assert(_json->type() == JsonNs::json_type::FLOAT);
        return pointer_convert(_json, 0, json_float*)->as_f64();
    }

    s64 as_s64() const
    {
        assert(_json->type() == JsonNs::json_type::INT);
        return pointer_convert(_json, 0, json_int*)->as_s64();
    }

    string as_string() const
    {
        assert(_json->type() == JsonNs::json_type::STRING);
        return pointer_convert(_json, 0, json_string*)->as_string();
    }

private:
    JsonNs::json_base* _json;
};
