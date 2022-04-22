
#include "../EnvironmentHeader.hpp"
#include "../../_Interface.hpp"

class event_context
{
public:
    mutex _mt;
};

boole event::init()
{
    assert(_ctx == nullptr);

    auto* ctx = new event_context();
    ctx->_mt.init();
    ctx->_mt.wait_acquire();
    return boole::True;
}

boole event::uninit()
{
    auto* ctx = pointer_convert(_ctx, 0, event_context*);
    assert(ctx);

    ctx->_mt.uninit();
    delete ctx;
    _ctx = nullptr;
    return boole::True;
}

boole event::wait()
{
    auto* ctx = pointer_convert(_ctx, 0, event_context*);
    assert(ctx);

    return ctx->_mt.wait_acquire();
}

boole event::set()
{
    auto* ctx = pointer_convert(_ctx, 0, event_context*);
    assert(ctx);

    return ctx->_mt.release();
}
