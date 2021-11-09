#pragma once

#include "Math.hpp"

class random final
{
public:
    static u64 new_u64() noexcept;

    static u64 new_u64_with_mod(u64 m) noexcept { return new_u64() % m; }
};

class date final
{
public:
    date() = default;

    date(const date&) = default;

    date& operator =(const date&) = default;

    ~date() = default;

public:
    static date now();

    static date now_utc();

    u64 timestamp();

public:
    s32 year;
    s32 month;
    s32 day;
    s32 week_day;
    s32 hour;
    s32 minute;
    s32 second;
    s32 millisec;
};

class tick final
{
public:
    tick() = default;

    tick(const tick&) = default;

    ~tick() = default;

public:
    static void sleep(u64 ms);

    static u64 count();

    static void elapse_init()
    {
        record(count());
    }

    static u64 elapse()
    {
        assert_info(record(0), "call tick::start() first before tick_elapse()");
        return count() - record(0);
    }

    static void elapse_print()
    {
        print("Time elapse is: %llums.\n", elapse());
    }

    static tick new_sleep_cycle()
    {
        tick t;
        t._sleep_cycle = 0;
        sleep(0);
    }

    void sleep_cycle()
    {
        static u64 sleep_intervals[32] =
        {
            1, 1, 1, 1, 2, 2, 2, 2,
            4, 4, 4, 4, 8, 8, 8, 8,
            16, 16, 16, 16, 32, 32, 32, 32,
            64, 64, 64, 64, 128, 128, 256, 256
        };

        assert(_sleep_cycle >= 0 && _sleep_cycle < 32);
        sleep(sleep_intervals[_sleep_cycle]);
        _sleep_cycle = math::max(_sleep_cycle + 1, 31);
    }

private:
    static u64 record(u64 r = 0)
    {
        static u64 myrec = 0;
        return r == 0 ? myrec : (myrec = r);
    }

private:
    union
    {
        u64 _sleep_cycle;
        u64 _launch_tick;
    };
};

namespace _InternalNs
{

class tick_initializer
{
public:
    tick_initializer()
    {
        ::tick::elapse_init();
    }

    ~tick_initializer() = default;
};

_SELECTANY_ tick_initializer global_tick_initializer;

}
