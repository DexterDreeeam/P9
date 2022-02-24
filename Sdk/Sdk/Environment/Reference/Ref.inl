#pragma once

template<typename Ty>
_INLINE_ ref<Ty>::ref() :
    _ctx(nullptr)
{
}

template<typename Ty>
_INLINE_ ref<Ty>::ref(const ref<Ty>& rhs)
{
    auto* ctx = rhs._ctx.get();
    if (ctx)
    {
        ctx->add_ref();
    }
    _ctx.set(ctx);
}

template<typename Ty>
_INLINE_ ref<Ty>::ref(ref<Ty>&& rhs) noexcept
{
    auto* ctx = rhs._ctx.exchange(nullptr);
    _ctx.set(ctx);
}

template<typename Ty>
template<typename Other_Ty>
_INLINE_ ref<Ty>::ref(const ref<Other_Ty>& rhs)
{
    Ty* pointer_checker = (Other_Ty*)nullptr;

    auto* ctx = pointer_convert(rhs._ctx.get(), 0, _RefNs::context<Ty>*);
    if (ctx)
    {
        ctx->add_ref();
    }
    _ctx.set(ctx);
}

template<typename Ty>
template<typename Other_Ty>
_INLINE_ ref<Ty>::ref(ref<Other_Ty>&& rhs) noexcept
{
    Ty* pointer_checker = (Other_Ty*)nullptr;

    auto* ctx = rhs._ctx.exchange(nullptr);
    _ctx.set(pointer_convert(ctx, 0, _RefNs::context<Ty>*));
}

template<typename Ty>
_INLINE_ ref<Ty>::~ref()
{
    auto* ctx = _ctx.get();
    if (ctx && ctx->remove_ref())
    {
        delete ctx;
    }
}

template<typename Ty>
_INLINE_ ref<Ty>& ref<Ty>::operator =(const ref<Ty>& rhs)
{
    auto* ctx = rhs._ctx.get();
    if (ctx)
    {
        ctx->add_ref();
    }

    auto* origin = _ctx.exchange(ctx);
    if (origin && origin->remove_ref())
    {
        delete origin;
    }

    return *this;
}

template<typename Ty>
_INLINE_ ref<Ty>& ref<Ty>::operator =(ref<Ty>&& rhs) noexcept
{
    auto* ctx = rhs._ctx.exchange(nullptr);
    auto* origin = _ctx.exchange(ctx);
    if (origin && origin->remove_ref())
    {
        delete origin;
    }

    return *this;
}

template<typename Ty>
template<typename Other_Ty>
_INLINE_ ref<Ty>& ref<Ty>::operator =(const ref<Other_Ty>& rhs)
{
    Ty* pointer_checker = (Other_Ty*)nullptr;

    auto* ctx = pointer_convert(rhs._ctx.get(), 0, _RefNs::context<Ty>*);
    if (ctx)
    {
        ctx->add_ref();
    }

    auto* origin = _ctx.exchange(ctx);
    if (origin && origin->remove_ref())
    {
        delete origin;
    }

    return *this;
}

template<typename Ty>
template<typename Other_Ty>
_INLINE_ ref<Ty>& ref<Ty>::operator =(ref<Other_Ty>&& rhs) noexcept
{
    Ty* pointer_checker = (Other_Ty*)nullptr;

    auto* ctx = rhs._ctx.exchange(nullptr);
    auto* origin = _ctx.exchange(pointer_convert(ctx, 0, _RefNs::context<Ty>*));
    if (origin && origin->remove_ref())
    {
        delete origin;
    }

    return *this;
}

template<typename Ty>
_INLINE_ bool ref<Ty>::operator ==(const ref<Ty>& rhs) const
{
    return _ctx.get() == rhs._ctx.get();
}

template<typename Ty>
_INLINE_ bool ref<Ty>::operator !=(const ref<Ty>& rhs) const
{
    return _ctx.get() != rhs._ctx.get();
}

template<typename Ty>
template<typename Other_Ty>
_INLINE_ bool ref<Ty>::operator ==(const ref<Other_Ty>& rhs) const
{
    return _ctx.get() == pointer_convert(rhs._ctx.get(), 0, _RefNs::context<Ty>*);
}

template<typename Ty>
template<typename Other_Ty>
_INLINE_ bool ref<Ty>::operator !=(const ref<Other_Ty>& rhs) const
{
    return _ctx.get() != pointer_convert(rhs._ctx.get(), 0, _RefNs::context<Ty>*);
}

template<typename Ty>
_INLINE_ Ty* ref<Ty>::operator ->()
{
    assert(_ctx.get() == nullptr || _ctx.get()->_ptr);
    auto* ctx = _ctx.get();
    return ctx ? ctx->_ptr : nullptr;
}

template<typename Ty>
_INLINE_ const Ty* ref<Ty>::operator ->() const
{
    assert(_ctx.get() == nullptr || _ctx.get()->_ptr);
    auto* ctx = _ctx.get();
    return ctx ? ctx->_ptr : nullptr;
}

template<typename Ty>
_INLINE_ Ty& ref<Ty>::operator *()
{
    auto* ctx = _ctx.get();
    assert(ctx && ctx->_ptr);
    return *ctx->_ptr;
}

template<typename Ty>
_INLINE_ const Ty& ref<Ty>::operator *() const
{
    auto* ctx = _ctx.get();
    assert(ctx && ctx->_ptr);
    return *ctx->_ptr;
}

template<typename Ty>
_INLINE_ ref<Ty>::ref(_RefNs::context<Ty>* ctx) :
    _ctx(ctx)
{
}

template<typename Ty>
template<typename Other_Ty>
ref<Other_Ty> ref<Ty>::ref_of()
{
    auto* ctx = _ctx.get();
    if (ctx)
    {
        ctx->add_ref();
        auto r = ref<Other_Ty>(pointer_convert(ctx, 0, _RefNs::context<Other_Ty>*));
        return r;
    }
    else
    {
        return ref<Other_Ty>();
    }
}

template<typename Ty>
_INLINE_ boole ref<Ty>::has_value() const
{
    auto* ctx = _ctx.get();
    return ctx != nullptr;
}

template<typename Ty>
_INLINE_ void ref<Ty>::clear()
{
    auto* ctx = _ctx.exchange(nullptr);
    if (ctx && ctx->remove_ref())
    {
        delete ctx;
    }
}

template<typename Ty>
_INLINE_ void ref<Ty>::debug_report() const
{
    auto* ctx = _ctx.get();
    ctx->debug_report();
}

template<typename Ty>
template<typename ...Args>
_INLINE_ ref<Ty> ref<Ty>::new_instance(Args ...args)
{
    Ty* obj = new (memory::alloc(sizeof(Ty))) Ty(args...);
    return new_instance_with_ptr(obj);
}

template<typename Ty>
_INLINE_ ref<Ty> ref<Ty>::new_instance_with_ptr(Ty* raw_ptr)
{
    auto* ctx = _RefNs::context<Ty>::new_instance_with_ptr(raw_ptr);
    return ref<Ty>(ctx);
}

_INLINE_ ref<void>::ref() :
    _ctx(nullptr)
{
}

_INLINE_ ref<void>::ref(const ref<void>& rhs)
{
    auto* ctx = rhs._ctx.get();
    if (ctx)
    {
        ctx->add_ref();
    }
    _ctx.set(ctx);
}

_INLINE_ ref<void>::ref(ref<void>&& rhs) noexcept
{
    auto* ctx = rhs._ctx.exchange(nullptr);
    _ctx.set(ctx);
}

template<typename Other_Ty>
_INLINE_ ref<void>::ref(const ref<Other_Ty>& rhs)
{
    auto* ctx = pointer_convert(rhs._ctx.get(), 0, _RefNs::context<void>*);
    if (ctx)
    {
        ctx->add_ref();
    }
    _ctx.set(ctx);
}

template<typename Other_Ty>
_INLINE_ ref<void>::ref(ref<Other_Ty>&& rhs) noexcept
{
    auto* ctx = rhs._ctx.exchange(nullptr);
    _ctx.set(pointer_convert(ctx, 0, _RefNs::context<void>*));
}

_INLINE_ ref<void>::~ref()
{
    auto* ctx = _ctx.get();
    if (ctx && ctx->remove_ref())
    {
        delete ctx;
    }
}

_INLINE_ ref<void>& ref<void>::operator =(const ref<void>& rhs)
{
    auto* ctx = rhs._ctx.get();
    if (ctx)
    {
        ctx->add_ref();
    }

    auto* origin = _ctx.exchange(ctx);
    if (origin && origin->remove_ref())
    {
        delete origin;
    }

    return *this;
}

_INLINE_ ref<void>& ref<void>::operator =(ref<void>&& rhs) noexcept
{
    auto* ctx = rhs._ctx.exchange(nullptr);
    auto* origin = _ctx.exchange(ctx);
    if (origin && origin->remove_ref())
    {
        delete origin;
    }

    return *this;
}

template<typename Other_Ty>
_INLINE_ ref<void>& ref<void>::operator =(const ref<Other_Ty>& rhs)
{
    auto* ctx = pointer_convert(rhs._ctx.get(), 0, _RefNs::context<void>*);
    if (ctx)
    {
        ctx->add_ref();
    }

    auto* origin = _ctx.exchange(ctx);
    if (origin && origin->remove_ref())
    {
        delete origin;
    }

    return *this;
}

template<typename Other_Ty>
_INLINE_ ref<void>& ref<void>::operator =(ref<Other_Ty>&& rhs) noexcept
{
    auto* ctx = rhs._ctx.exchange(nullptr);
    auto* origin = _ctx.exchange(pointer_convert(ctx, 0, _RefNs::context<void>*));
    if (origin && origin->remove_ref())
    {
        delete origin;
    }

    return *this;
}

_INLINE_ bool ref<void>::operator ==(const ref<void>& rhs) const
{
    return _ctx.get() == rhs._ctx.get();
}

_INLINE_ bool ref<void>::operator !=(const ref<void>& rhs) const
{
    return _ctx.get() != rhs._ctx.get();
}

template<typename Other_Ty>
_INLINE_ bool ref<void>::operator ==(const ref<Other_Ty>& rhs) const
{
    return _ctx.get() == pointer_convert(rhs._ctx.get(), 0, _RefNs::context<void>*);
}

template<typename Other_Ty>
_INLINE_ bool ref<void>::operator !=(const ref<Other_Ty>& rhs) const
{
    return _ctx.get() != pointer_convert(rhs._ctx.get(), 0, _RefNs::context<void>*);
}

_INLINE_ ref<void>::ref(_RefNs::context<void>* ctx) :
    _ctx(ctx)
{
}

template<typename Other_Ty>
ref<Other_Ty> ref<void>::ref_of()
{
    auto* ctx = _ctx.get();
    if (ctx)
    {
        ctx->add_ref();
        auto r = ref<Other_Ty>(pointer_convert(ctx, 0, _RefNs::context<Other_Ty>*));
        return r;
    }
    else
    {
        return ref<Other_Ty>();
    }
}

_INLINE_ boole ref<void>::has_value() const
{
    auto* ctx = _ctx.get();
    return ctx != nullptr;
}

_INLINE_ void ref<void>::clear()
{
    auto* ctx = _ctx.exchange(nullptr);
    if (ctx && ctx->remove_ref())
    {
        delete ctx;
    }
}
