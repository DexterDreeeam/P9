#pragma once

namespace _Internal {
namespace _SafeQueue {

constexpr u64 ceil_pow2(u64 num)
{
    if ((num & (num - 1)) == 0)
    {
        return num;
    }
    u64 ret = 1ULL;
    while ((~ret + 1) & num)
    {
        ret = ret << 1;
    }
    return ret;
}

}} // _Internal::_SafeQueue

template<typename Ty, s64 Cap, s64 ExtremeThreadCnt = 8ULL>
class safe_queue
{
    struct token
    {
        token() :
            t(0)
        {}

        atom<s64> t;
    };

public:
    safe_queue() :
        tks(),
        head(0),
        tail(0)
    {
    }

    ~safe_queue()
    {
        clear();
    }

    bool enqueue(const Ty& in)
    {
        if (_overload())
        {
            return false;
        }
        u64 holder = tail++;
        new (_at(holder & _mask())) Ty(in);
        tks[holder & _mask()].t = 1;
        return true;
    }

    bool dequeue(Ty* out)
    {
        u64 holder = head.get();
        if (tks[holder & _mask()].t.compare_exchange(1, 0) == 1)
        {
            while ((holder & _mask()) != (head.get() & _mask()))
            {
                yield();
            }
            new (out) Ty(*_at(holder & _mask()));
            ++head;
            _at(holder & _mask())->~Ty();
            return true;
        }
        else
        {
            return false;
        }
    }

    void clear()
    {
        while (head.get() != tail.get())
        {
            _at(head.get() & _mask())->~Ty();
            head.weak_add(1);
        }
    }

private:
    bool _overload()
    {
        return (u64)tail.get() - (u64)head.get() >= (s64)_size() - (s64)ExtremeThreadCnt;
    }

    Ty* _at(s64 idx)
    {
        return pointer_convert(data, sizeof(Ty) * idx, Ty*);
    }

    s64 _size()
    {
        return _Internal::_SafeQueue::ceil_pow2(Cap);
    }

    u64 _mask()
    {
        return (u64)_size() - 1;
    }

private:
    char      data[sizeof(Ty) * _Internal::_SafeQueue::ceil_pow2(Cap)];
    token     tks[_Internal::_SafeQueue::ceil_pow2(Cap)];
    atom<u64> head;
    atom<u64> tail;
};
