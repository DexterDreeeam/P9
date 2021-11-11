#pragma once

namespace _RefNs
{

typedef void deconstructor_cb(void*);

class context_base
{
public:
    context_base() = delete;

    context_base(const context_base&) = delete;

    context_base(deconstructor_cb* dc);

    ~context_base() = default;

    context_base& operator =(const context_base&) = delete;

public:
    deconstructor_cb*  _deconstructor;
    atom<s64>          _ref_cnt;
    atom<s64>          _obs_cnt;
    atom<s64>          _obs_lock;
};

template<typename Ty>
class context : context_base
{
private:
    context() = delete;

    context(const context&) = delete;

    context(Ty* ptr);

    context& operator =(const context& rhs) = delete;

public:
    ~context();

public:
    static context<Ty>* new_instance_with_ptr(Ty* raw_ptr);

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

private:
    static void deconstructor(void* p);

public:
    Ty* _ptr;
};

}
