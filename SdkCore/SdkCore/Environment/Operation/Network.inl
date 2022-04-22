#pragma once

_INLINE_ network_client::network_client() :
    _ctx(nullptr)
{
}

_INLINE_ network_client::network_client(const network_client& rhs) :
    _ctx(rhs._ctx)
{
}

_INLINE_ network_client& network_client::operator =(const network_client& rhs)
{
    _ctx = rhs._ctx;
    return *this;
}

_INLINE_ boole network_client::is_init()
{
    return _ctx != nullptr;
}

_INLINE_ network_connect::network_connect() :
    _ctx(nullptr)
{
}

_INLINE_ network_connect::network_connect(const network_connect& rhs) :
    _ctx(rhs._ctx)
{
}

_INLINE_ network_connect& network_connect::operator =(const network_connect& rhs)
{
    _ctx = rhs._ctx;
    return *this;
}

_INLINE_ network_server::network_server() :
    _ctx(nullptr)
{
}

_INLINE_ network_server::network_server(const network_server& rhs) :
    _ctx(rhs._ctx)
{
}

_INLINE_ network_server& network_server::operator =(const network_server& rhs)
{
    _ctx = rhs._ctx;
    return *this;
}

_INLINE_ boole network_server::is_init()
{
    return _ctx != nullptr;
}
