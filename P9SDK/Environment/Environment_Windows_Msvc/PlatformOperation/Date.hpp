#pragma once

#include "Math.hpp"
#include "Thread.hpp"

_INLINE_ u64   random();
_INLINE_ u64   random(u64 mod);

_INLINE_ date  date_query();
_INLINE_ date  date_query_utc();
_INLINE_ u64   date_timestamp_utc();

_INLINE_ void  tick_sleep(u64 ms);
_INLINE_ u64   tick_sleep_cycle(u64& counter);
_INLINE_ u64   tick_count();
_INLINE_ u64   tick_record(u64 rec);
_INLINE_ void  tick_start();
_INLINE_ u64   tick_elapse();
_INLINE_ void  tick_elapse_print();

_INLINE_ u64 random()
{
    u64 rst = 0;
    WindowsMsvcNs::LARGE_INTEGER ticks;
    for (s64 i = 0; i < 16; ++i)
    {
        yield();
        WindowsMsvcNs::QueryPerformanceCounter(&ticks);
        rst = rst << 4;
        rst |= ticks.QuadPart & 0b00001111;
    }
    return rst;
}

_INLINE_ u64 random(u64 mod)
{
    return random() % mod;
}

_INLINE_ date date_query()
{
    WindowsMsvcNs::SYSTEMTIME sys;
    WindowsMsvcNs::GetLocalTime(&sys);
    date rst =
    {
        sys.wYear,
        sys.wMonth,
        sys.wDay,
        sys.wDayOfWeek,
        sys.wHour,
        sys.wMinute,
        sys.wSecond,
        sys.wMilliseconds,
    };
    return rst;
}

_INLINE_ date date_query_utc()
{
    WindowsMsvcNs::SYSTEMTIME sys;
    WindowsMsvcNs::GetSystemTime(&sys);
    date rst =
    {
        sys.wYear,
        sys.wMonth,
        sys.wDay,
        sys.wDayOfWeek,
        sys.wHour,
        sys.wMinute,
        sys.wSecond,
        sys.wMilliseconds,
    };
    return rst;
}

_INLINE_ u64 date_timestamp_utc()
{
    WindowsMsvcNs::time_t time_utc;
    WindowsMsvcNs::time(&time_utc);
    return time_utc;
}

_INLINE_ void tick_sleep(u64 ms)
{
    WindowsMsvcNs::Sleep((WindowsMsvcNs::DWORD)ms);
}

_INLINE_ u64 tick_sleep_cycle(u64& counter)
{
    static u64 sleep_intervals[32] =
    {
        1, 1, 1, 1, 2, 2, 2, 2,
        4, 4, 4, 4, 8, 8, 8, 8,
        16, 16, 16, 16, 32, 32, 32, 32,
        64, 64, 64, 64, 128, 128, 256, 256
    };

    u64 cnt = counter;
    assert(cnt >= 0 && cnt < 32);
    tick_sleep(sleep_intervals[cnt]);
    cnt = _max(cnt + 1, 31);
    counter = cnt;
    return cnt;
}

_INLINE_ u64 tick_count()
{
    return WindowsMsvcNs::GetTickCount64();
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
