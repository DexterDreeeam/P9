
#include "../EnvironmentHeader.hpp"
#include "../../Interface.hpp"

#if defined(MEMORY_DEBUG)

struct debug_header
{
    u64    _execute_thread_id;
    u64    _execute_timestamp;
    s64    _memory_size;
    void*  _memory_addr;
};

static atom<s64> _global_memory_debug_unrelease_number = 0;
static s64 _global_memory_debug_unrelease_max = 0;

void* memory::alloc(u64 sz)
{
    s64 alloc_sz = sz + sizeof(debug_header);
    auto* p = ::malloc(alloc_sz);
    auto* header = pointer_convert(p, 0, debug_header*);
    header->_execute_thread_id = current_thread_id();
    header->_execute_timestamp = date::timestamp();
    header->_memory_size = sz;
    header->_memory_addr = pointer_convert(p, sizeof(debug_header), void*);

    s64 unrelease = ++_global_memory_debug_unrelease_number;
    if (unrelease > _global_memory_debug_unrelease_max)
    {
        _global_memory_debug_unrelease_max = unrelease;
    }
    print("Memory alloc: [%lld][%lld]\n\tthread: %llu\n\ttimestamp: %llu\n\tmemory_size: %lld\n\tmemory_addr: %llu\n",
        unrelease, _global_memory_debug_unrelease_max,
        header->_execute_thread_id, header->_execute_timestamp,
        header->_memory_size, (u64)header->_memory_addr);

    return header->_memory_addr;
}

void memory::free(void* addr)
{
    auto* header = pointer_convert(addr, -(s64)sizeof(debug_header), debug_header*);
    print("Memory free: [%lld]\n\tmemory_addr: %llu\n",
        --_global_memory_debug_unrelease_number, (u64)header->_memory_addr);
    ::free(header);
}

#else

void* memory::alloc(u64 sz)
{
    void* p = ::malloc(sz);
    assert(p);
    return p;
}

void memory::free(void* addr)
{
    ::free(addr);
}

#endif

void memory::set(void* addr, u8 v, u64 sz)
{
    assert(addr);
    ::memset(addr, v, sz);
}

void memory::copy(const void* src, void* dst, u64 sz)
{
    if (sz != 0)
    {
        assert(src && dst);
        ::memcpy_s(dst, sz, src, sz);
    }
}
