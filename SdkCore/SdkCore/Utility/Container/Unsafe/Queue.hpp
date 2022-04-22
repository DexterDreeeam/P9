#pragma once

const s64 queue_unit_extent = 4LL;
const s64 queue_cap_max = 1LL << 30;

template<typename Ty>
class queue
{
    using Self_Ty = queue<Ty>;

public:
    queue() :
        elem(nullptr),
        cap(0),
        sz(0),
        start_at(0)
    {}

    template<typename ...Args>
    queue(Args...args) :
        elem((Ty*)memory::alloc<void>(sizeof(Ty)* _alloc_ceil(sizeof...(args)))),
        cap(_alloc_ceil(sizeof...(args))),
        sz(0)
    {
        push(args...);
    }

    s64 size() const noexcept
    {
        return sz;
    }

    bool empty() const noexcept
    {
        return sz == 0;
    }

    Ty& back() noexcept
    {
        assert(sz > 0);
        s64 pos = (start_at + sz - 1) % cap;
        return elem[pos];
    }

    const Ty& back() const noexcept
    {
        assert(sz > 0);
        s64 pos = (start_at + sz - 1) % cap;
        return elem[pos];
    }

    Ty& front() noexcept
    {
        assert(sz > 0);
        return elem[start_at];
    }

    const Ty& front() const noexcept
    {
        assert(sz > 0);
        return elem[start_at];
    }

    template<typename ...Args>
    void push(const Ty& first, Args...args)
    {
        _need_space(sizeof...(args) + 1);
        assert(sz < cap);
        _push_back(first);
        ++sz;
        push(args...);
    }

    template<typename ...Args>
    void push(Ty&& first, Args...args)
    {
        _need_space(sizeof...(args) + 1);
        assert(sz < cap);
        _push_back(first);
        ++sz;
        push(args...);
    }

    void push(const Ty& e) noexcept
    {
        _need_space(1);
        assert(sz < cap);
        _push_back(e);
        ++sz;
    }

    void push(Ty&& e) noexcept
    {
        _need_space(1);
        assert(sz < cap);
        _push_back(e);
        ++sz;
    }

    void pop() noexcept
    {
        assert(sz > 0);
        --sz;
        _pop_front();
    }

private:
    static s64 _alloc_ceil(s64 x) noexcept
    {
        return math::ceil(x, queue_unit_extent);
    }

    void _push_back(const Ty& e) noexcept
    {
        s64 target = (start_at + sz) % cap;
        new (pointer_convert(elem, sizeof(Ty) * target, Ty*)) Ty(e);
    }

    void _push_back(Ty&& e) noexcept
    {
        s64 target = (start_at + sz) % cap;
        new (pointer_convert(elem, sizeof(Ty) * target, Ty*)) Ty(e);
    }

    void _pop_front() noexcept
    {
        pointer_convert(elem, sizeof(Ty) * start_at, Ty*)->~Ty();
        ++start_at;
    }

    void _need_space(s64 add)
    {
        assert(sz <= cap);
        if (sz + add <= cap)
        {
            return;
        }
        void* p_tobe_release = elem;
        s64 new_cap = cap;
        while (sz + add > new_cap)
        {
            new_cap = new_cap == 0 ? queue_unit_extent : (new_cap << 1);
            assert(new_cap <= queue_cap_max);
        }
        void* p_new_mem = memory::alloc<void>(sizeof(Ty) * new_cap);
        if (sz == 0)
        {
            elem = (Ty*)p_new_mem;
        }
        else if (start_at + sz <= cap)
        {
            memory::copy(pointer_convert(elem, sizeof(Ty) * start_at, void*), p_new_mem, sizeof(Ty) * sz);
            elem = (Ty*)p_new_mem;
        }
        else
        {
            memory::copy(pointer_convert(elem, sizeof(Ty) * start_at, void*), p_new_mem, sizeof(Ty) * (cap - start_at));
            memory::copy(elem, pointer_convert(p_new_mem, sizeof(Ty) * (cap - start_at), void*), sizeof(Ty) * (sz + start_at - cap));
            elem = (Ty*)p_new_mem;
        }
        start_at = 0;
        cap = new_cap;
        if (p_tobe_release)
        {
            memory::free(p_tobe_release);
        }
    }

private:
    Ty* elem;
    s64 cap;
    s64 sz;
    s64 start_at;
};
