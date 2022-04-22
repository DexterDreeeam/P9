#pragma once

namespace _SelecteeNs
{

_INLINE_ constexpr s64 safe_selectee_layer(u64 cap)
{
    s64 current_cap = 1LL;
    s64 lay = 1LL;
    while (current_cap < (s64)cap)
    {
        ++lay;
        current_cap = current_cap << 1;
    }
    return lay;
}

_INLINE_ constexpr s64 safe_selectee_length(u64 cap)
{
    s64 lay = safe_selectee_layer(cap);
    return (1LL << lay) - 1;
}

_INLINE_ constexpr s64 safe_selectee_last_layer_head(u64 cap)
{
    s64 lay = safe_selectee_layer(cap);
    return (1LL << (lay - 1)) - 1;
}

_INLINE_ s64 safe_selectee_myleft(s64 idx)
{
    return (idx << 1) + 1;
}

_INLINE_ s64 safe_selectee_myright(s64 idx)
{
    return (idx << 1) + 2;
}

_INLINE_ s64 safe_selectee_myparent(s64 idx)
{
    return idx > 0 ? ((idx - 1) >> 1) : -1;
}

template<typename Ty>
class safe_selectee_node
{
public:
    safe_selectee_node() :
        signal(0),
        ptr(nullptr)
    {}

    ~safe_selectee_node()
    {
        if (ptr)
        {
            ptr->~Ty();
            memory::free(ptr);
            ptr = nullptr;
        }
    }

public:
    atom<s64> signal;
    Ty*       ptr;
};

}

template<typename Ty, u64 Cap, bool InitSignal>
class selectee
{
    using Node_Ty = _SelecteeNs::safe_selectee_node<Ty>;

public:
    selectee() :
        data()
    {
        s64 idx = _SelecteeNs::safe_selectee_last_layer_head(Cap);
        for (; idx < _length(); ++idx)
        {
            data[idx].ptr = memory::alloc<Ty>(sizeof(Ty));
            new (data[idx].ptr) Ty();
            data[idx].signal = InitSignal;
        }
        if (InitSignal)
        {
            idx = _SelecteeNs::safe_selectee_last_layer_head(Cap);
            while (--idx >= 0)
            {
                s64 left = _SelecteeNs::safe_selectee_myleft(idx);
                s64 right = _SelecteeNs::safe_selectee_myright(idx);
                data[idx].signal = data[left].signal + data[right].signal;
            }
        }
    }

    ~selectee()
    {
    }

    Ty& at(s64 ext_idx)
    {
        assert(ext_idx >= 0 && ext_idx < Cap);
        return *data[_ext_to_int(ext_idx)].ptr;
    }

    s64 get()
    {
        if (--data[0].signal < 0)
        {
            ++data[0].signal;
            return -1;
        }
        s64 idx = 0;
        while (idx < _SelecteeNs::safe_selectee_last_layer_head(Cap))
        {
            s64 left = _SelecteeNs::safe_selectee_myleft(idx);
            s64 right = _SelecteeNs::safe_selectee_myright(idx);
            assert(left < _length() && right < _length());
            if (--data[left].signal >= 0)
            {
                idx = left;
                continue;
            }
            else
            {
                ++data[left].signal;
            }
            if (--data[right].signal >= 0)
            {
                idx = right;
                continue;
            }
            else
            {
                ++data[right].signal;
            }
            assert(0);
        }
        return _int_to_ext(idx);
    }

    void put(s64 ext_idx)
    {
        assert(ext_idx >= 0 && ext_idx < Cap);
        s64 idx = _ext_to_int(ext_idx);
        if (++data[idx].signal > 1)
        {
            //# error instance, put one idx twice, return immediately
            --data[idx].signal;
            assert(0);
            return;
        }
        idx = _SelecteeNs::safe_selectee_myparent(idx);
        while (idx >= 0)
        {
            ++data[idx].signal;
            idx = _SelecteeNs::safe_selectee_myparent(idx);
        }
    }

private:
    s64 _length() const
    {
        return _SelecteeNs::safe_selectee_length(Cap);
    }

    s64 _ext_to_int(s64 idx)
    {
        return _SelecteeNs::safe_selectee_last_layer_head(Cap) + idx;
    }

    s64 _int_to_ext(s64 idx)
    {
        return idx - _SelecteeNs::safe_selectee_last_layer_head(Cap);
    }

private:
    Node_Ty data[_SelecteeNs::safe_selectee_length(Cap)];
};
