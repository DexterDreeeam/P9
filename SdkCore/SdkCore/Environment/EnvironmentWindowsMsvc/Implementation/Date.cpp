
#include "../../_Interface.hpp"
#include "../EnvironmentHeader.hpp"

u64 random::new_u64() noexcept
{
    //struct waiter
    //{
    //    waiter(f64 us_last)
    //    {
    //        while (1)
    //        {
    //            MemoryBarrier();
    //            if (quartz::count_us() - us_last > 0.6)
    //            {
    //                MemoryBarrier();
    //                break;
    //            }
    //            MemoryBarrier();
    //            yield();
    //        }
    //    }
    //};

    //u64 rst = 0;
    //u64 bits = 0;
    //f64 us_last = quartz::count_us();
    //LARGE_INTEGER ticks = {};
    //while (bits < 64)
    //{
    //    waiter w(us_last);
    //    f64 r = quartz::count_us();
    //    u64 ru = *pointer_convert(&r, 0, u64*);
    //    u64 compose = (ru % 7) + 1;
    //    u64 mask = 0;
    //    for (s64 i = 0; i < compose; ++i)
    //    {
    //        MemoryBarrier();
    //        ::QueryPerformanceCounter(&ticks);
    //        MemoryBarrier();
    //        mask ^= ticks.QuadPart & 1;
    //        MemoryBarrier();
    //        yield();
    //    }
    //    rst = rst << 1;
    //    rst |= mask;
    //    MemoryBarrier();
    //    ++bits;
    //    us_last = quartz::count_us();
    //}
    //return rst;

    u64 rst = 0;
    for (s64 i = 0; i < 8; ++i)
    {
        rst = rst << 8;
        rst |= native_rand();
    }
    return rst;
}

u8 random::native_rand() noexcept
{
    static atom<s64> first = 1;
    if (first.compare_exchange(1, -1))
    {
        LARGE_INTEGER ticks = {};
        ::QueryPerformanceCounter(&ticks);
        srand((u32)ticks.QuadPart);
        first.set(0);
    }
    while (first.get() != 0)
    {
        yield();
    }
    return (u8)rand();
}

date date::now()
{
    SYSTEMTIME sys;
    ::GetLocalTime(&sys);

    date rst;
    rst.year = sys.wYear;
    rst.month = sys.wMonth;
    rst.day = sys.wDay;
    rst.week_day = sys.wDayOfWeek;
    rst.hour = sys.wHour;
    rst.minute = sys.wMinute;
    rst.second = sys.wSecond;
    rst.millisec = sys.wMilliseconds;
    return rst;
}

date date::now_utc()
{
    SYSTEMTIME sys;
    ::GetSystemTime(&sys);

    date rst;
    rst.year = sys.wYear;
    rst.month = sys.wMonth;
    rst.day = sys.wDay;
    rst.week_day = sys.wDayOfWeek;
    rst.hour = sys.wHour;
    rst.minute = sys.wMinute;
    rst.second = sys.wSecond;
    rst.millisec = sys.wMilliseconds;
    return rst;
}

u64 date::timestamp()
{
    time_t time_utc;
    ::time(&time_utc);
    return (u64)time_utc;
}

void tick::sleep(u64 ms)
{
    ::Sleep(ms);
}

void tick::usleep(u64 us)
{
    HANDLE timer;
    LARGE_INTEGER ft = {};
    ft.QuadPart = -((s64)10 * (s64)us);
    timer = CreateWaitableTimerA(nullptr, TRUE, nullptr);
    SetWaitableTimer(timer, &ft, 0, nullptr, nullptr, 0);
    WaitForSingleObject(timer, INFINITE);
    CloseHandle(timer);
}

u64 tick::count()
{
    return (u64)::GetTickCount64();
}

f64 quartz::count_ms()
{
    LARGE_INTEGER ticks;
    LARGE_INTEGER freq;
    if (!QueryPerformanceCounter(&ticks))
    {
        return 0;
    }
    if (!QueryPerformanceFrequency(&freq))
    {
        return 0;
    }
    f64 rst = (f64)ticks.QuadPart / ((f64)freq.QuadPart / (f64)1000);
    return rst;
}

f64 quartz::count_us()
{
    LARGE_INTEGER ticks;
    LARGE_INTEGER freq;
    if (!QueryPerformanceCounter(&ticks))
    {
        return 0;
    }
    if (!QueryPerformanceFrequency(&freq))
    {
        return 0;
    }
    f64 rst = (f64)ticks.QuadPart / ((f64)freq.QuadPart / (f64)1000 / (f64)1000);
    return rst;
}

f64 quartz::count_n_us(u64 unit_us_count)
{
    LARGE_INTEGER ticks;
    LARGE_INTEGER freq;
    if (!QueryPerformanceCounter(&ticks))
    {
        return 0;
    }
    if (!QueryPerformanceFrequency(&freq))
    {
        return 0;
    }
    f64 rst = (f64)ticks.QuadPart / ((f64)freq.QuadPart * (f64)unit_us_count / (f64)1000 / (f64)1000);
    return (u64)rst;
}
