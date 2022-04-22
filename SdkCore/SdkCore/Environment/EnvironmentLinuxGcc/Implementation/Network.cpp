
#include "../EnvironmentHeader.hpp"
#include "../../_Interface.hpp"

struct network_client_context
{
    s64      _sk;
    mutex    _mt;
};

struct network_connect_context
{
    s64      _sk;
    sockaddr _addr;
    mutex    _mt;
};

struct network_server_context
{
    s64      _sk;
};

boole network_client::init(const char* server_addr, s64 server_port)
{
    assert(_ctx == nullptr);
    auto* ctx = new network_client_context();

    s64 ret;
    sockaddr_in sock_addr = {};

    if (server_port < 0 || server_port > 65535)
    {
        goto L_error;
    }
    ctx->_sk = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ctx->_sk <= 0)
    {
        goto L_error;
    }
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = ::htons(server_port);
    if (::inet_pton(AF_INET, server_addr, &sock_addr.sin_addr) <= 0)
    {
        goto L_error;
    }

    ret = ::connect(ctx->_sk, (sockaddr*)&sock_addr, sizeof(sockaddr));
    if (ret != 0)
    {
        goto L_error;
    }

    ctx->_mt.init();
    _ctx = ctx;
    return boole::True;

L_error:
    if (ctx)
    {
        if (ctx->_sk > 0)
        {
            ::close(ctx->_sk);
        }
        delete ctx;
    }
    return boole::False;
}

boole network_client::uninit()
{
    auto* ctx = pointer_convert(_ctx, 0, network_client_context*);
    assert(ctx);
    assert(ctx->_sk > 0);

    if (::close(ctx->_sk) == 0)
    {
        ctx->_mt.uninit();
        delete ctx;
        _ctx = nullptr;
        return boole::True;
    }
    else
    {
        return boole::False;
    }
}

boole network_client::send(const void* content, s64 send_len)
{
    auto* ctx = pointer_convert(_ctx, 0, network_client_context*);
    assert(ctx);
    assert(ctx->_sk > 0);

    s64 buf_len = sizeof(s64) + send_len;
    char* buf = memory::alloc<char>(buf_len);
    assert(buf);
    *pointer_convert(buf, 0, s64*) = send_len;
    memory::copy(content, buf + sizeof(s64), send_len);

    s64 actual_send = ::send(ctx->_sk, buf, buf_len, 0);
    memory::free(buf);

    if (actual_send == buf_len)
    {
        return boole::True;
    }
    else
    {
        return boole::False;
    }
}

boole network_client::receive(void* buf, s64& receive_len)
{
    auto* ctx = pointer_convert(_ctx, 0, network_client_context*);
    assert(ctx);
    assert(ctx->_sk > 0);

    s64 content_len = -1;
    ctx->_mt.wait_acquire();
    if (::recv(ctx->_sk, pointer_convert(&content_len, 0, char*), sizeof(s64), 0) < 0)
    {
        goto L_error;
    }
    if (content_len <= 0)
    {
        goto L_error;
    }
    if (::recv(ctx->_sk, pointer_convert(buf, 0, char*), content_len, 0) != content_len)
    {
        goto L_error;
    }

    assert(content_len > 0);
    ctx->_mt.release();
    receive_len = content_len;
    return boole::True;

L_error:
    ctx->_mt.release();
    return boole::False;
}

boole network_connect::send(const void* content, s64 send_len)
{
    auto* ctx = pointer_convert(_ctx, 0, network_connect_context*);
    assert(ctx);
    assert(ctx->_sk > 0);

    s64 buf_len = sizeof(s64) + send_len;
    char* buf = memory::alloc<char>(buf_len);
    assert(buf);
    *pointer_convert(buf, 0, s64*) = send_len;
    memory::copy(content, buf + sizeof(s64), send_len);

    s64 actual_send = ::send(ctx->_sk, buf, buf_len, 0);
    memory::free(buf);

    if (actual_send == buf_len)
    {
        return boole::True;
    }
    else
    {
        return boole::False;
    }
}

boole network_connect::receive(void* buf, s64& receive_len)
{
    auto* ctx = pointer_convert(_ctx, 0, network_connect_context*);
    assert(ctx->_sk > 0);

    s64 content_len = -1;

    ctx->_mt.wait_acquire();
    if (::recv(ctx->_sk, pointer_convert(&content_len, 0, char*), sizeof(s64), 0) < 0)
    {
        goto L_error;
    }
    if (content_len <= 0)
    {
        goto L_error;
    }
    if (::recv(ctx->_sk, pointer_convert(buf, 0, char*), content_len, 0) != content_len)
    {
        goto L_error;
    }

    assert(content_len > 0);
    ctx->_mt.release();
    receive_len = content_len;
    return boole::True;

L_error:
    ctx->_mt.release();
    return boole::False;
}

boole network_connect::destroy()
{
    auto* ctx = pointer_convert(_ctx, 0, network_connect_context*);
    assert(ctx);
    assert(ctx->_sk > 0);

    if (::close(ctx->_sk) == 0)
    {
        ctx->_mt.uninit();
        delete ctx;
        _ctx = nullptr;
        return boole::True;
    }
    else
    {
        return boole::False;
    }
}

boole network_server::init(s64 server_port)
{
    assert(_ctx == nullptr);

    s64 ret;
    sockaddr_in sock_addr = {};

    auto* ctx = new network_server_context();
    if (server_port < 0 || server_port > 65535)
    {
        goto L_error;
    }
    ctx->_sk = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ctx->_sk <= 0)
    {
        goto L_error;
    }
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = ::htonl(INADDR_ANY);
    sock_addr.sin_port = ::htons(server_port);
    ret = ::bind(ctx->_sk, (sockaddr*)&sock_addr, sizeof(sockaddr));
    if (ret != 0)
    {
        goto L_error;
    }
    ret = ::listen(ctx->_sk, SOMAXCONN);
    if (ret != 0)
    {
        goto L_error;
    }

    _ctx = ctx;
    return boole::True;

L_error:
    if (ctx)
    {
        if (ctx->_sk > 0)
        {
            ::close(ctx->_sk);
        }
        delete ctx;
    }
    return boole::False;
}

boole network_server::uninit()
{
    auto* ctx = pointer_convert(_ctx, 0, network_server_context*);
    assert(ctx);
    assert(ctx->_sk > 0);

    if (::close(ctx->_sk) == 0)
    {
        delete ctx;
        _ctx = nullptr;
        return boole::True;
    }
    else
    {
        return boole::False;
    }
}

boole network_server::new_connect(network_connect& nc)
{
    auto* ctx = pointer_convert(_ctx, 0, network_server_context*);
    assert(ctx);
    assert(ctx->_sk > 0);

    auto* nc_ctx = new network_connect_context();
    socklen_t addr_len = 0;

    nc_ctx->_sk = ::accept(
        ctx->_sk, &nc_ctx->_addr, &addr_len);

    if (nc_ctx->_sk < 0)
    {
        delete nc_ctx;
        return boole::False;
    }

    nc_ctx->_mt.init();
    nc._ctx = nc_ctx;
    return boole::True;
}
