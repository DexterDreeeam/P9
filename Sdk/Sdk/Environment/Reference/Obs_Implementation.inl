#pragma once

template<typename Ty>
_INLINE_ obs<Ty>::obs() :
    _ctx(nullptr)
{
}

template<typename Ty>
_INLINE_ obs<Ty>::obs(const obs<Ty>& rhs)
{
    auto* ctx = rhs._ctx.get();
    if (ctx)
    {
        ctx->add_obs();
    }
    _ctx.set(ctx);
}

template<typename Ty>
_INLINE_ obs<Ty>::obs(obs<Ty>&& rhs)
{
    auto* ctx = rhs._ctx.exchange(nullptr);
    _ctx.set(ctx);
}

template<typename Ty>
template<typename Other_Ty>
_INLINE_ obs<Ty>::obs(const obs<Other_Ty>& rhs)
{
    Ty* pointer_checker = (Other_Ty*)nullptr;

    auto* ctx = pointer_convert(rhs._ctx.get(), 0, _RefNs::context<Ty>*);
    if (ctx)
    {
        ctx->add_obs();
    }
    _ctx.set(ctx);
}

template<typename Ty>
template<typename Other_Ty>
_INLINE_ obs<Ty>::obs(obs<Other_Ty>&& rhs)
{
    Ty* pointer_checker = (Other_Ty*)nullptr;

    auto* ctx = rhs._ctx.exchange(nullptr);
    _ctx.set(pointer_convert(ctx, 0, _RefNs::context<Ty>*));
}

template<typename Ty>
_INLINE_ obs<Ty>::obs(const ref<Ty>& rhs)
{
    auto* ctx = rhs._ctx.get();
    if (ctx)
    {
        ctx->add_obs();
    }
    _ctx.set(ctx);
}

template<typename Ty>
_INLINE_ obs<Ty>::obs(ref<Ty>&& rhs)
{
    auto* ctx = rhs._ctx.exchange(nullptr);
    if (ctx)
    {
        ctx->add_obs();
        ctx->remove_ref(); // need not check return value
    }
    _ctx.set(ctx);
}

template<typename Ty>
template<typename Other_Ty>
_INLINE_ obs<Ty>::obs(const ref<Other_Ty>& rhs)
{
    auto* ctx = rhs._ctx.get();
    if (ctx)
    {
        ctx->add_obs();
    }
    _ctx.set(pointer_convert(ctx, 0, _RefNs::context<Ty>*));
}

template<typename Ty>
template<typename Other_Ty>
_INLINE_ obs<Ty>::obs(ref<Other_Ty>&& rhs)
{
    auto* ctx = rhs._ctx.exchange(nullptr);
    if (ctx)
    {
        ctx->add_obs();
        ctx->remove_ref(); // need not check return value
    }
    _ctx.set(pointer_convert(ctx, 0, _RefNs::context<Ty>*));
}

template<typename Ty>
_INLINE_ obs<Ty>::~obs()
{
    auto* ctx = _ctx.get();
    if (ctx && ctx->remove_obs())
    {
        delete ctx;
    }
}

template<typename Ty>
_INLINE_ obs<Ty>& obs<Ty>::operator =(const obs<Ty>& rhs)
{
    auto* ctx = rhs._ctx.get();
    if (ctx)
    {
        ctx->add_obs();
    }

    auto* origin = _ctx.exchange(ctx);
    if (origin && origin->remove_obs())
    {
        delete origin;
    }

    return *this;
}

template<typename Ty>
_INLINE_ obs<Ty>& obs<Ty>::operator =(obs<Ty>&& rhs)
{
    auto* ctx = rhs._ctx.exchange(nullptr);
    auto* origin = _ctx.exchange(ctx);
    if (origin && origin->remove_obs())
    {
        delete origin;
    }

    return *this;
}

template<typename Ty>
template<typename Other_Ty>
_INLINE_ obs<Ty>& obs<Ty>::operator =(const obs<Other_Ty>& rhs)
{
    Ty* pointer_checker = (Other_Ty*)nullptr;

    auto* ctx = pointer_convert(rhs._ctx.get(), 0, _RefNs::context<Ty>*);
    if (ctx)
    {
        ctx->add_obs();
    }

    auto* origin = _ctx.exchange(ctx);
    if (origin && origin->remove_obs())
    {
        delete origin;
    }

    return *this;
}

template<typename Ty>
template<typename Other_Ty>
_INLINE_ obs<Ty>& obs<Ty>::operator =(obs<Other_Ty>&& rhs)
{
    auto* ctx = rhs._ctx.exchange(nullptr);
    auto* origin = _ctx.exchange(pointer_convert(ctx, 0, _RefNs::context<Ty>*));
    if (origin && origin->remove_obs())
    {
        delete origin;
    }

    return *this;
}

template<typename Ty>
_INLINE_ obs<Ty>& obs<Ty>::operator =(const ref<Ty>& rhs)
{
    auto* ctx = rhs._ctx.get();
    if (ctx)
    {
        ctx->add_obs();
    }

    auto* origin = _ctx.exchange(ctx);
    if (origin && origin->remove_obs())
    {
        delete origin;
    }

    return *this;
}

template<typename Ty>
_INLINE_ obs<Ty>& obs<Ty>::operator =(ref<Ty>&& rhs)
{
    auto* ctx = rhs._ctx.exchange(nullptr);
    ctx->add_obs();
    ctx->remove_ref(); // no need to check

    auto* origin = _ctx.exchange(ctx);
    if (origin && origin->remove_obs())
    {
        delete origin;
    }

    return *this;
}

template<typename Ty>
template<typename Other_Ty>
_INLINE_ obs<Ty>& obs<Ty>::operator =(const ref<Other_Ty>& rhs)
{
    Ty* pointer_checker = (Other_Ty*)nullptr;

    auto* ctx = pointer_convert(rhs._ctx.get(), 0, _RefNs::context<Ty>*);
    if (ctx)
    {
        ctx->add_obs();
    }

    auto* origin = _ctx.exchange(ctx);
    if (origin && origin->remove_obs())
    {
        delete origin;
    }

    return *this;
}

template<typename Ty>
template<typename Other_Ty>
_INLINE_ obs<Ty>& obs<Ty>::operator =(ref<Other_Ty>&& rhs)
{
    auto* ctx = rhs._ctx.exchange(nullptr);
    ctx->add_obs();
    ctx->remove_ref(); // no need to check

    auto* origin = _ctx.exchange(pointer_convert(ctx, 0, _RefNs::context<Ty>*));
    if (origin && origin->remove_obs())
    {
        delete origin;
    }

    return *this;
}

template<typename Ty>
_INLINE_ bool obs<Ty>::operator ==(const obs<Ty>& rhs) const
{
    return _ctx.get() == rhs._ctx.get();
}

template<typename Ty>
_INLINE_ bool obs<Ty>::operator !=(const obs<Ty>& rhs) const
{
    return _ctx.get() != rhs._ctx.get();
}

template<typename Ty>
template<typename Other_Ty>
_INLINE_ bool obs<Ty>::operator ==(const obs<Other_Ty>& rhs) const
{
    return _ctx.get() == pointer_convert(rhs._ctx.get(), 0, _RefNs::context<Ty>*);
}

template<typename Ty>
template<typename Other_Ty>
_INLINE_ bool obs<Ty>::operator !=(const obs<Other_Ty>& rhs) const
{
    return _ctx.get() != pointer_convert(rhs._ctx.get(), 0, _RefNs::context<Ty>*);
}

template<typename Ty>
template<typename Other_Ty>
obs<Other_Ty> obs<Ty>::obs_of()
{
    auto* ctx = _ctx.get();
    if (ctx)
    {
        ctx->add_obs();
        auto o = obs<Other_Ty>(pointer_convert(ctx, 0, _RefNs::context<Other_Ty>*));
        return o;
    }
    else
    {
        return obs<Other_Ty>();
    }
}

template<typename Ty>
_INLINE_ ref<Ty> obs<Ty>::try_ref()
{
    auto* ctx = _ctx.get();
    if (ctx && ctx->try_ref())
    {
        auto r = ref<Ty>(ctx);
        return r;
    }
    else
    {
        return ref<Ty>();
    }
}

template<typename Ty>
_INLINE_ boole obs<Ty>::has_value() const
{
    auto* ctx = _ctx.get();
    return ctx != nullptr;
}

template<typename Ty>
_INLINE_ void obs<Ty>::clear()
{
    auto* ctx = _ctx.exchange(nullptr);
    if (ctx && ctx->remove_obs())
    {
        delete ctx;
    }
}
