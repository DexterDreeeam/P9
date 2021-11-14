
#include "../EnvironmentHeader.hpp"
#include "../../Interface.hpp"

struct mutex_context
{
    HANDLE _hndl;
};

boole mutex::init()
{
    assert(_ctx == nullptr);

    HANDLE h = ::CreateMutexA(NULL, FALSE, NULL);
    if (h)
    {
        auto* ctx = new mutex_context();
        ctx->_hndl = h;
        _ctx = ctx;
        return boole::True;
    }
    else
    {
        return boole::False;
    }
}

boole mutex::uninit()
{
    auto* ctx = pointer_convert(_ctx, 0, mutex_context*);
    assert(ctx);
    assert(ctx->_hndl);

    if (::CloseHandle(ctx->_hndl))
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

boole mutex::try_acquire()
{
    auto* ctx = pointer_convert(_ctx, 0, mutex_context*);
    assert(ctx);
    assert(ctx->_hndl);

    if (::WaitForSingleObject(ctx->_hndl, 0) == WAIT_OBJECT_0)
    {
        return boole::True;
    }
    else
    {
        return boole::False;
    }
}

boole mutex::wait_acquire()
{
    auto* ctx = pointer_convert(_ctx, 0, mutex_context*);
    assert(ctx);
    assert(ctx->_hndl);

    if (::WaitForSingleObject(ctx->_hndl, INFINITE) == WAIT_OBJECT_0)
    {
        return boole::True;
    }
    else
    {
        return boole::False;
    }
}

boole mutex::release()
{
    auto* ctx = pointer_convert(_ctx, 0, mutex_context*);
    assert(ctx);
    assert(ctx->_hndl);

    if (::ReleaseMutex(ctx->_hndl))
    {
        return boole::True;
    }
    else
    {
        return boole::False;
    }
}
