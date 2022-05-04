#pragma once

template<typename Ty = void>
class ref;

template<typename Ty = void>
class obs;

namespace _Internal {
namespace _Ref {

typedef void deconstructor_cb(void*);

class ref_context_base
{
public:
    ref_context_base() = delete;

    ref_context_base(const ref_context_base&) = delete;

    ref_context_base(deconstructor_cb* dc);

    ~ref_context_base() = default;

    ref_context_base& operator =(const ref_context_base&) = delete;

public:
    deconstructor_cb*  _deconstructor;
    atom<s64>          _ref_cnt;
    atom<s64>          _obs_cnt;
    atom<s64>          _obs_lock;
};

template<typename Ty>
class ref_context : ref_context_base
{
private:
    ref_context() = delete;

    ref_context(const ref_context&) = delete;

    ref_context(Ty* ptr);

    ref_context& operator =(const ref_context& rhs) = delete;

public:
    ~ref_context();

public:
    static ref_context<Ty>* new_instance_with_ptr(Ty* raw_ptr);

public:
    // ref<Ty> call add_ref() to copy another ref<Ty>
    void  add_ref();

    // ref<Ty> call remove_ref() when deconstruct, if return value is true, delete this context is expected
    boole remove_ref();

    // ref<Ty> or obs<Ty> call add_obs() to create an obs<Ty>
    void  add_obs();

    // obs<Ty> call remove_obs() when deconstruct, if return value is true, delete this context is expected
    boole remove_obs();

    // obs<Ty> call try_ref() when try to get a copy of ref(), if return value is true, try_ref() succeed, no need add_ref() again
    boole try_ref();

public:
    void debug_report() const;

private:
    static void deconstructor(void* p);

public:
    Ty* _ptr;
};

}} // _Internal::_Ref
