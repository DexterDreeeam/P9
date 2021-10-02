#pragma once

_INLINE_ date  date_query();
_INLINE_ date  date_query_utc();
_INLINE_ u64   date_timestamp_utc();
_INLINE_ void  tick_sleep(u64 ms);
_INLINE_ u64   tick_count();
_INLINE_ u64   tick_record(u64 rec);
_INLINE_ void  tick_start();
_INLINE_ u64   tick_elapse();
_INLINE_ void  tick_elapse_print();

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
    time_t time_utc;
    time(&time_utc);
    return time_utc;
}

_INLINE_ void tick_sleep(u64 ms)
{
    WindowsMsvcNs::Sleep((WindowsMsvcNs::DWORD)ms);
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
