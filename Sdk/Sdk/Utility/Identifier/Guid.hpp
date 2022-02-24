#pragma once

#include "../../Environment/Interface.hpp"
#include "../Container/Unsafe/String.hpp"
#include "../Container/Unsafe/Pair.hpp"

class guid
{
private:
    guid()
    {}

public:
    guid(const guid& rhs) noexcept :
        _u64_0(rhs._u64_0),
        _u64_1(rhs._u64_1)
    {}

    ~guid() noexcept = default;

    guid& operator =(const guid& rhs) noexcept
    {
        _u64_0 = rhs._u64_0;
        _u64_1 = rhs._u64_1;
        return *this;
    }

    bool operator ==(const guid& rhs) const noexcept
    {
        return _u64_0 == rhs._u64_0 && _u64_1 == rhs._u64_1;
    }

    bool operator !=(const guid& rhs) const noexcept
    {
        return _u64_0 != rhs._u64_0 || _u64_1 != rhs._u64_1;
    }

public:
    pair<u64, u64> as_u64_pair() const noexcept
    {
        return pair<u64, u64>(_u64_0, _u64_1);
    }

    string as_string() const noexcept
    {
        char buf[32];
        s64 len;
        string rst = "";

        len = u64_to_text_hex_padding(_u64_0, buf, 16);
        buf[len] = 0;
        rst += buf;

        len = u64_to_text_hex_padding(_u64_1, buf, 16);
        buf[len] = 0;
        rst += buf;

        return rst;
    }

    string as_guid_string() const noexcept
    {
        char buf[16];
        s64 len;
        string rst = "";

        len = u64_to_text_hex_padding(_u16_3, buf, 4);
        buf[len] = 0;
        rst += buf;

        len = u64_to_text_hex_padding(_u16_2, buf, 4);
        buf[len] = 0;
        rst += buf;

        rst += '-';

        len = u64_to_text_hex_padding(_u16_1, buf, 4);
        buf[len] = 0;
        rst += buf;

        rst += '-';

        len = u64_to_text_hex_padding(_u16_0, buf, 4);
        buf[len] = 0;
        rst += buf;

        rst += '-';

        len = u64_to_text_hex_padding(_u16_7, buf, 4);
        buf[len] = 0;
        rst += buf;

        rst += '-';

        len = u64_to_text_hex_padding(_u16_6, buf, 4);
        buf[len] = 0;
        rst += buf;

        len = u64_to_text_hex_padding(_u16_5, buf, 4);
        buf[len] = 0;
        rst += buf;

        len = u64_to_text_hex_padding(_u16_4, buf, 4);
        buf[len] = 0;
        rst += buf;

        return rst;
    }

    static guid new_instance() noexcept
    {
        guid g;
        g._u64_0 = random::new_u64();
        g._u64_1 = random::new_u64();
        return g;
    }

private:
    union
    {
        u64 _u64s[2];
        u32 _u32s[4];
        u16 _u16s[8];
        struct
        {
            u64 _u64_0;
            u64 _u64_1;
        };
        struct
        {
            u32 _u32_0;
            u32 _u32_1;
            u32 _u32_2;
            u32 _u32_3;
        };
        struct
        {
            u16 _u16_0;
            u16 _u16_1;
            u16 _u16_2;
            u16 _u16_3;
            u16 _u16_4;
            u16 _u16_5;
            u16 _u16_6;
            u16 _u16_7;
        };
    };
};
