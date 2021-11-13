#pragma once

_INLINE_ void* operator new(size_t sz);

_INLINE_ void operator delete(void* ptr);

_INLINE_ void* operator new[](size_t sz);

_INLINE_ void operator delete[](void* ptr);

_INLINE_ void* operator new(size_t sz, void* ptr);

_INLINE_ void* operator new[](size_t sz, void* ptr);
