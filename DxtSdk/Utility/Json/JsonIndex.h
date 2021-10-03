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
            string value = j->element_value();
            print("%s %s\n", path.data(), value.data());
        });
#endif
}
