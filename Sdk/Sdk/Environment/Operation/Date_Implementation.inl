#pragma once

_INLINE_ u64 random::new_u64_with_mod(u64 m) noexcept
{
    return new_u64() % m;
}

_INLINE_ void tick::elapse_init()
{
    record(count());
}

_INLINE_ u64 tick::elapse()
{
    u64 rec = record(0);
    if (rec)
    {
        return count() - rec;
    }
    else
    {
        elapse_init();
        return elapse();
    }
}

_INLINE_ void tick::elapse_print()
{
    print("Time elapse is: %llums.\n", elapse());
}

_INLINE_ tick tick::new_sleep_cycle()
{
    tick t;
    t._sleep_cycle = 0;
    sleep(0);
    return t;
}

_INLINE_ void tick::sleep_cycle()
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

_INLINE_ u64 tick::record(u64 r)
{
    static u64 myrec = 0;
    return r == 0 ? myrec : (myrec = r);
}
