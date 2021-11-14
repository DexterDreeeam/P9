
#include "../EnvironmentHeader.hpp"
#include "../../Interface.hpp"

struct mutex_context
{
    pthread_mutex_t _mt;
};

boole mutex::init()
{
    assert(_ctx == nullptr);

    auto* ctx = new mutex_context();
    ::pthread_mutex_init(&ctx->_mt, nullptr);
    _ctx = ctx;
    return boole::True;
}

boole mutex::uninit()
{
    auto* ctx = pointer_convert(_ctx, 0, mutex_context*);
    assert(ctx);

    ::pthread_mutex_destroy(&ctx->_mt);
    delete ctx;
    _ctx = nullptr;
    return boole::True;
}

boole mutex::try_acquire()
{
    auto* ctx = pointer_convert(_ctx, 0, mutex_context*);
    assert(ctx);

    return ::pthread_mutex_trylock(&ctx->_mt) == 0;
}

boole mutex::wait_acquire()
{
    auto* ctx = pointer_convert(_ctx, 0, mutex_context*);
    assert(ctx);

    ::pthread_mutex_lock(&ctx->_mt);
    return boole::True;
}

boole mutex::release()
{
    auto* ctx = pointer_convert(_ctx, 0, mutex_context*);
    assert(ctx);

    ::pthread_mutex_unlock(&ctx->_mt);
    return boole::True;
}
