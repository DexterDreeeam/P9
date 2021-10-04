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

private:
    JsonNs::json_base* _json;
};
