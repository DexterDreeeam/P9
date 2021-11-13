#pragma once

_INLINE_ file::file() :
    _ctx(nullptr)
{
}

_INLINE_ file::file(const file& rhs) :
    _ctx(rhs._ctx)
{
}

_INLINE_ file& file::operator =(const file& rhs)
{
    _ctx = rhs._ctx;
    return *this;
}

_INLINE_ boole file::is_init()
{
    return _ctx != nullptr;
}

_INLINE_ boole file::output(const char* content)
{
    assert(content);
    return output(content, str_len(content));
}
