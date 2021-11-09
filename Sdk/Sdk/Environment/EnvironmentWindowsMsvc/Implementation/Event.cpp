
#include "../../Interface.hpp"
#include "../EnvironmentHeader.hpp"

class event_context
{
public:
    HANDLE _event_handle;
};

boole event::init()
{
    assert(_ctx == nullptr);

    HANDLE h = ::CreateEventA(NULL, FALSE, NULL, NULL);
    if (h)
    {
        auto* ctx = new event_context();
        ctx->_event_handle = h;
        _ctx = ctx;
        return boole::True;
    }
    else
    {
        return boole::False;
    }
}

boole event::uninit()
{
    auto* ctx = pointer_convert(_ctx, 0, event_context*);
    assert(ctx);
    assert(ctx->_event_handle);

    if (::CloseHandle(ctx->_event_handle))
    {
        return boole::True;
    }
    else
    {
        return boole::False;
    }
}

boole event::wait()
{
    auto* ctx = pointer_convert(_ctx, 0, event_context*);
    assert(ctx);
    assert(ctx->_event_handle);

    if (::WaitForSingleObject(ctx->_event_handle, INFINITE) == WAIT_OBJECT_0)
    {
        return boole::True;
    }
    else
    {
        return boole::False;
    }
}

boole event::set()
{
    auto* ctx = pointer_convert(_ctx, 0, event_context*);
    assert(ctx);
    assert(ctx->_event_handle);

    if (::SetEvent(ctx->_event_handle))
    {
        return boole::True;
    }
    else
    {
        return boole::False;
    }
}
