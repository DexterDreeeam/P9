#pragma once

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

namespace RandomNs
{

class random_initializer
{
public:
    random_initializer()
    {
        u32 seed = (u32)(date_timestamp_utc() % u32_max);
        WindowsMsvcNs::srand(seed);
    }

    ~random_initializer() = default;
};

_SELECTANY_ random_initializer __random_initializer;

}

_INLINE_ u64 random()
{
    u8 _u8_0 = (u8)(u32)WindowsMsvcNs::rand();
    u8 _u8_1 = (u8)(u32)WindowsMsvcNs::rand();
    u8 _u8_2 = (u8)(u32)WindowsMsvcNs::rand();
    u8 _u8_3 = (u8)(u32)WindowsMsvcNs::rand();
    u8 _u8_4 = (u8)(u32)WindowsMsvcNs::rand();
    u8 _u8_5 = (u8)(u32)WindowsMsvcNs::rand();
    u8 _u8_6 = (u8)(u32)WindowsMsvcNs::rand();
    u8 _u8_7 = (u8)(u32)WindowsMsvcNs::rand();
    return
        ((u64)_u8_0) |
        (((u64)_u8_1) << 8) |
        (((u64)_u8_2) << 16) |
        (((u64)_u8_3) << 24) |
        (((u64)_u8_4) << 32) |
        (((u64)_u8_5) << 40) |
        (((u64)_u8_6) << 48) |
        (((u64)_u8_7) << 56);
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
