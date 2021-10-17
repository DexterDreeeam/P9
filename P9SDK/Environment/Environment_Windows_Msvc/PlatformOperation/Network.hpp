#pragma once

#pragma comment(lib, "ws2_32.lib")

typedef void*  network_client;
typedef void*  network_server;
typedef void*  network_connect;

_INLINE_ network_client   network_client_create(const char* server_addr, s64 server_port);
_INLINE_ boole            network_client_send(network_client x, const void* msg, s64 len);
_INLINE_ s64              network_client_wait_receive(network_client x, _OUT_ void* msg);
_INLINE_ void             network_client_destroy(network_client x);

_INLINE_ network_server   network_server_create(s64 server_port);
_INLINE_ void             network_server_destroy(network_server x);

_INLINE_ network_connect  network_connect_create(network_server x);
_INLINE_ boole            network_connect_send(network_connect x, const void* msg, s64 len);
_INLINE_ s64              network_connect_wait_receive(network_connect x, _OUT_ void* msg, _OUT_ s64& read_len);
_INLINE_ void             network_connect_destroy(network_connect x);

struct _network_client
{
    WindowsMsvcNs::SOCKET _sock;
    lock _lock;
};

struct _network_server
{
    WindowsMsvcNs::SOCKET _sock;
};

struct _network_connect
{
    WindowsMsvcNs::SOCKET _sock;
    WindowsMsvcNs::sockaddr _addr;
    int _addr_len;
    lock _lock;
};

_INLINE_ network_client network_client_create(const char* server_addr, s64 server_port)
{
    WindowsMsvcNs::WSADATA wsa_data = {};
    if (WindowsMsvcNs::WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
    {
        return (network_client)nullptr;
    }

    _network_client* nc = new _network_client();

    s64 ret;
    char host_name[256] = {};
    WindowsMsvcNs::SOCKADDR_IN sock_addr = {};

    if (server_port < 0 || server_port > 65535)
    {
        goto L_error;
    }
    ret = WindowsMsvcNs::gethostname(host_name, sizeof(host_name));
    if (ret != 0)
    {
        goto L_error;
    }
    nc->_sock = WindowsMsvcNs::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (nc->_sock == INVALID_SOCKET)
    {
        goto L_error;
    }
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = WindowsMsvcNs::inet_addr(server_addr);
    sock_addr.sin_port = WindowsMsvcNs::htons(server_port);
    ret = WindowsMsvcNs::connect(nc->_sock, (WindowsMsvcNs::SOCKADDR*)&sock_addr, sizeof(WindowsMsvcNs::SOCKADDR));
    if (ret != 0)
    {
        goto L_error;
    }

    goto L_finish;

L_error:
    if (nc)
    {
        delete nc;
    }
    nc = nullptr;

L_finish:
    nc->_lock = lock_create();
    return (network_client)nc;
}

_INLINE_ boole network_client_send(network_client x, const void* msg, s64 len)
{
    _network_client* nc = (_network_client*)x;
    assert(nc && nc->_sock);

    s64 buf_len = sizeof(s64) + len;
    char* buf = (char*)memory_alloc(buf_len);
    assert(buf);
    *pointer_convert(buf, 0, s64*) = len;
    memory_copy(msg, buf + sizeof(s64), len);

    s64 actual_send = WindowsMsvcNs::send(nc->_sock, buf, buf_len, 0);
    if (actual_send == buf_len)
    {
        return boole::True;
    }
    else
    {
        return boole::False;
    }
}

_INLINE_ s64 network_client_wait_receive(network_client x, _OUT_ void* msg)
{
    _network_client* nc = (_network_client*)x;
    assert(nc && nc->_sock);

    s64 content_len = -1;

    lock_wait_get(nc->_lock);
    if (WindowsMsvcNs::recv(nc->_sock, pointer_convert(&content_len, 0, char*), sizeof(s64), 0) < 0)
    {
        goto L_error;
    }
    if (content_len <= 0)
    {
        goto L_error;
    }
    if (WindowsMsvcNs::recv(nc->_sock, pointer_convert(msg, 0, char*), content_len, 0) != content_len)
    {
        goto L_error;
    }

    goto L_finish;

L_error:
    content_len = -1;
L_finish:
    assert(content_len > 0);
    lock_put(nc->_lock);
    return content_len;
}

_INLINE_ void network_client_destroy(network_client x)
{
    _network_client* nc = (_network_client*)x;
    assert(nc && nc->_sock && nc->_lock);
    lock_destroy(nc->_lock);
    WindowsMsvcNs::closesocket(nc->_sock);
    delete nc;
}

_INLINE_ network_server network_server_create(s64 server_port)
{
    WindowsMsvcNs::WSADATA wsa_data = {};
    if (WindowsMsvcNs::WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
    {
        return (network_server)nullptr;
    }

    _network_server* ns = new _network_server();

    s64 ret;
    char host_name[256] = {};
    WindowsMsvcNs::SOCKADDR_IN sock_addr = {};

    if (server_port < 0 || server_port > 65535)
    {
        goto L_error;
    }
    ret = WindowsMsvcNs::gethostname(host_name, sizeof(host_name));
    if (ret != 0)
    {
        goto L_error;
    }
    ns->_sock = WindowsMsvcNs::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ns->_sock == INVALID_SOCKET)
    {
        goto L_error;
    }
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = WindowsMsvcNs::htonl(INADDR_ANY);
    sock_addr.sin_port = WindowsMsvcNs::htons(server_port);
    ret = WindowsMsvcNs::bind(ns->_sock, (WindowsMsvcNs::SOCKADDR*)&sock_addr, sizeof(WindowsMsvcNs::SOCKADDR));
    if (ret != 0)
    {
        goto L_error;
    }
    ret = WindowsMsvcNs::listen(ns->_sock, SOMAXCONN);
    if (ret != 0)
    {
        goto L_error;
    }
    goto L_finish;

L_error:
    if (ns)
    {
        delete ns;
    }
    ns = nullptr;

L_finish:
    return (network_server)ns;
}

_INLINE_ void network_server_destroy(network_server x)
{
    _network_server* ns = (_network_server*)x;
    assert(ns && ns->_sock);
    WindowsMsvcNs::closesocket(ns->_sock);
    delete ns;
}

_INLINE_ network_connect network_connect_create(network_server x)
{
    _network_server* ns = (_network_server*)x;
    assert(ns && ns->_sock);

    _network_connect* connect = new _network_connect();
    connect->_sock = WindowsMsvcNs::accept(ns->_sock, &connect->_addr, &connect->_addr_len);
    if (connect->_sock < 0)
    {
        goto L_error;
    }

    goto L_finish;

L_error:
    if (connect)
    {
        delete connect;
    }
    return nullptr;

L_finish:
    connect->_lock = lock_create();
    return (network_connect)connect;
}

_INLINE_ boole network_connect_send(network_connect x, const void* msg, s64 len)
{
    _network_connect* connect = (_network_connect*)x;
    assert(connect && connect->_sock);

    s64 buf_len = sizeof(s64) + len;
    char* buf = (char*)memory_alloc(buf_len);
    assert(buf);
    *pointer_convert(buf, 0, s64*) = len;
    memory_copy(msg, buf + sizeof(s64), len);

    s64 actual_send = WindowsMsvcNs::send(connect->_sock, buf, buf_len, 0);
    if (actual_send == buf_len)
    {
        return boole::True;
    }
    else
    {
        return boole::False;
    }
}

_INLINE_ s64 network_connect_wait_receive(network_connect x, _OUT_ void* msg, _OUT_ s64& read_len)
{
    _network_connect* connect = (_network_connect*)x;
    assert(connect && connect->_sock && connect->_lock);

    s64 content_len = -1;

    lock_wait_get(connect->_lock);
    if (WindowsMsvcNs::recv(connect->_sock, pointer_convert(&content_len, 0, char*), sizeof(s64), 0) < 0)
    {
        goto L_error;
    }
    if (content_len <= 0)
    {
        goto L_error;
    }
    if (WindowsMsvcNs::recv(connect->_sock, pointer_convert(msg, 0, char*), content_len, 0) != content_len)
    {
        goto L_error;
    }

    goto L_finish;

L_error:
    content_len = -1;
L_finish:
    assert(content_len > 0);
    lock_put(connect->_lock);
    return content_len;
}

_INLINE_ void network_connect_destroy(network_connect x)
{
    _network_connect* connect = (_network_connect*)x;
    assert(connect && connect->_sock && connect->_lock);
    WindowsMsvcNs::closesocket(connect->_sock);
    lock_destroy(connect->_lock);
    delete connect;
}
