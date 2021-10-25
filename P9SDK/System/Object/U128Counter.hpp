#pragma once

class u128_counter
{
public:
    u128_counter() :
        _u64_1(0),
        _u64_2(0)
    {}

    u128_counter(u64 u64_1, u64 u64_2) :
        _u64_1(u64_1),
        _u64_2(u64_2)
    {}

    ~u128_counter() = default;

    bool operator ==(const u128_counter& rhs) const noexcept
    {
        return _u64_1 == rhs._u64_1 && _u64_2 == rhs._u64_2;
    }

    bool operator !=(const u128_counter& rhs) const noexcept
    {
        return _u64_1 != rhs._u64_1 || _u64_2 != rhs._u64_2;
    }

public:
    u64 _u64_1;
    u64 _u64_2;
};

/*
 *
 * u64 p1, p2, mark
 *
 * p1 map:
 * | --- part_1 --- | --- part_2 --- | --- part_3 --- | --- part_4 --- |
 *
 * when p1 range from part_1 to part_2, mark is set p2
 * when p1 range from part_3 to part_4, mark is set p2 + 1
 *
 * when p1 locate in part_1, read p2 as mark
 * when p1 locate in part_2 or part_3, read p2 as p2 itself
 * when p1 locate in part_4, read p2 as mark - 1
 *
 */

template<>
class atom<u128_counter>
{
public:
    atom() :
        _counter_1(0),
        _counter_2(0),
        _counter_mark(0)
    {}

    atom(const atom& rhs) = delete;

    ~atom() = default;

    u128_counter get()
    {
        u64 _1 = _counter_1.get();
        u64 _2 = 0;

        switch (_1 / _counter_threshold_fourth)
        {
        case 0:
            _2 = _counter_mark;
            break;
        case 1:
        case 2:
            _2 = _counter_2.get();
            break;
        default:
            _2 = _counter_mark - 1;
            break;
        }

        return u128_counter(_1, _2);
    }

    void set(u128_counter v)
    {
        u64 _1 = v._u64_1;
        u64 _2 = v._u64_2;
        _counter_1.set(_1);
        _counter_2.set(_2);

        switch (_1 / _counter_threshold_fourth)
        {
        case 0:
        case 1:
            _counter_mark = _2;
            break;
        default:
            _counter_mark = _2 + 1;
            break;
        }
    }

    u128_counter operator ++()
    {
        u64 _1 = ++_counter_1;
        u64 _2 = 0;

        switch (_1 / _counter_threshold_fourth)
        {
        case 0:
            _2 = _1 != 0 ? _counter_mark : ++_counter_2;
            break;
        case 1:
        case 2:
            _2 = _counter_2.get();
            if (_1 == _counter_threshold_fourth * 2)
            {
                _counter_mark = _2 + 1;
            }
            break;
        default:
            _2 = _counter_mark - 1;
        }

        return u128_counter(_1, _2);
    }

private:
    atom<u64>   _counter_1;
    atom<u64>   _counter_2;
    u64         _counter_mark;

    static const u64  _counter_threshold_fourth = u64_max / 4;
};
