#pragma once

class json_format
{
public:
    virtual JsonNs::json_base* serialize() = 0;
};
