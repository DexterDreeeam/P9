#pragma once

#include "Selectee.hpp"

namespace _Internal {
namespace _SafeSlotter {

template<u64 Size>
struct SlotNode
{
    u64 index;
    u8  mem[Size];
};

}} // _Internal::_SafeSlotter

template<u64 Cap, u64 SlotBytes>
class safe_slotter
{
    using NodeTy = _Internal::_SafeSlotter::SlotNode<SlotBytes>;

    const s64 mem_ofst = class_offset(NodeTy, mem);

public:
    safe_slotter() :
        _mem_selectee()
    {
        for (s64 i = 0; i < Cap; ++i)
        {
            auto* s = memory::alloc<NodeTy>(sizeof(NodeTy));
            assert(s);
            s->index = i;
            _mem_selectee.at(i) = s;
        }
    }

    ~safe_slotter()
    {
        for (s64 i = 0; i < Cap; ++i)
        {
            auto* s = _mem_selectee.at(i);
            if (s)
            {
                memory::free<NodeTy>(s);
            }
        }
    }

    template<typename Ty = void>
    Ty* get()
    {
        auto idx = _mem_selectee.get();
        if (idx >= 0)
        {
            auto* s = _mem_selectee.at(idx);
            return pointer_convert(s, mem_ofst, Ty*);
        }
        else
        {
            return nullptr;
        }
    }

    void put(void* ptr)
    {
        auto* s = pointer_convert(ptr, -mem_ofst, NodeTy*);
        _mem_selectee.put(s->index);
    }

private:
    selectee<NodeTy*, Cap, false> _mem_selectee;
};
