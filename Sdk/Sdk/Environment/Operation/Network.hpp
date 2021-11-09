#pragma once

class network_client final
{
public:
    network_client();

    network_client(const network_client& rhs);

    network_client& operator =(const network_client& rhs);

    ~network_client() = default;

public:
    boole init(const char* server_addr, s64 server_port);

    boole is_init();

    boole uninit();

    boole send(const void* content, s64 send_len);

    boole receive(void* buf, s64& receive_len);

private:
    void* _ctx;
};

class network_connect final
{
    friend class network_server;

public:
    network_connect();

    network_connect(const network_connect& rhs);

    network_connect& operator =(const network_connect& rhs);

    ~network_connect() = default;

public:
    boole send(const void* content, s64 send_len);

    boole receive(void* buf, s64& receive_len);

    boole destroy();

private:
    void* _ctx;
};

class network_server final
{
public:
    network_server();

    network_server(const network_server& rhs);

    network_server& operator =(const network_server& rhs);

    ~network_server() = default;

public:
    boole init(s64 server_port);

    boole is_init();

    boole uninit();

    boole new_connect(network_connect& nc);

private:
    void* _ctx;
};


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
