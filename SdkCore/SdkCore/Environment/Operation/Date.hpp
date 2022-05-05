#pragma once

#include "../BuildinType.hpp"

class random final
{
public:
    static u64 new_u64() noexcept;

    static u64 new_u64_with_mod(u64 m) noexcept;

private:
    static u8  native_rand() noexcept;
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

    static u64 timestamp();

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

    static void usleep(u64 us);

    static u64 count();

    static void elapse_init();

    static u64 elapse();

    static void elapse_print();

    static tick new_sleep_cycle();

    void sleep_cycle();

private:
    static u64 record(u64 r = 0);

private:
    union
    {
        u64 _sleep_cycle;
        u64 _launch_tick;
    };
};

class quartz final
{
public:
    quartz() = default;

    quartz(const quartz&) = default;

    ~quartz() = default;

public:
    static f64 count_ms();

    static f64 count_us();

    static f64 count_n_us(u64 unit_us_count);
};
