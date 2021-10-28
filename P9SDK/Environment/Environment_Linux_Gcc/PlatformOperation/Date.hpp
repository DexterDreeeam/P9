#pragma once

#include "Thread.hpp"

_INLINE_ u64   random();
_INLINE_ u64   random(u64 mod);
_INLINE_ date  date_query();
_INLINE_ date  date_query_utc();
_INLINE_ u64   date_timestamp_utc();
_INLINE_ void  tick_sleep(u64 ms);
_INLINE_ u64   tick_count();
_INLINE_ u64   tick_record(u64 rec);
_INLINE_ void  tick_start();
_INLINE_ u64   tick_elapse();
_INLINE_ void  tick_elapse_print();

_INLINE_ u64   random()
{
    u64 rst = 0;
    timespec ts;
    for (s64 i = 0; i < 16; ++i)
    {
        yield();
        clock_gettime(CLOCK_REALTIME, &ts);
        rst = rst << 4;
        rst |= (ts.tv_nsec & (0b00001111 << 4)) >> 4;
    }
    return rst;
}

_INLINE_ u64 random(u64 mod)
{
    return rand() % (mod);
}

_INLINE_ date date_query()
{
    time_t timer = time(0);
    tm* local_time = localtime(&timer);
    date rst =
    {
        local_time->tm_year,
        local_time->tm_mon,
        local_time->tm_mday,
        local_time->tm_wday,
        local_time->tm_hour,
        local_time->tm_min,
        local_time->tm_sec,
        0,
    };
    return rst;
}

_INLINE_ date date_query_utc()
{
    time_t timer = time(0);
    tm* local_time = localtime(&timer);
    date rst =
    {
        local_time->tm_year,
        local_time->tm_mon,
        local_time->tm_mday,
        local_time->tm_wday,
        local_time->tm_hour,
        local_time->tm_min,
        local_time->tm_sec,
        0,
    };
    return rst;
}

_INLINE_ u64 date_timestamp_utc()
{
    time_t time_utc;
    time(&time_utc);
    return time_utc;
}

_INLINE_ void tick_sleep(u64 ms)
{
    usleep(ms * 1000);
}

_INLINE_ u64 tick_count()
{
    timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    u64 ms = (u64)(ts.tv_nsec / 1000000) + ((u64)ts.tv_sec * 1000ull);
    return ms;
}

_INLINE_ u64 tick_record(u64 rec)
{
    static u64 myrec = 0;
    return rec == 0 ? myrec : (myrec = rec);
}

_INLINE_ void tick_start()
{
    tick_record(tick_count());
}

_INLINE_ u64 tick_elapse()
{
    assert_info(tick_record(0), "call tick_start() first before tick_elapse()");
    return tick_count() - tick_record(0);
}

_INLINE_ void  tick_elapse_print()
{
    print("Time elapse is: %llums.\n", tick_elapse());
}
