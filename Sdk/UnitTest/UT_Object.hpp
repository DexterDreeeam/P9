#pragma once

struct Pars
{
    s64 par;
};

//struct A : public obj<A>
//{
//    virtual const char* obj_type() const { return "A"; }
//
//    virtual u128 obj_type_id() const { return u128(123,456); }
//
//    s64 value;
//};
//
//struct A_has_build : public A
//{
//    A_has_build(const Pars& pars) :
//        A()
//    {
//        pars;
//        value = 2;
//    }
//
//    static ref<A> build(const Pars& pars)
//    {
//        pars;
//        auto r = ref<A>::new_instance();
//        r->value = 1;
//        return r;
//    }
//};
//
//struct A_no_build : public A
//{
//    A_no_build(const Pars& pars) :
//        A()
//    {
//        pars;
//        value = 3;
//    }
//};
//
//void UT_Object()
//{
//    auto o1 = obj<A_has_build>::build(Pars());
//    assert(o1->value == 1);
//
//    auto o2 = obj<A_no_build>::build(Pars());
//    assert(o2->value == 3);
//}

struct A : public obj<A>
{
    virtual const char* obj_type() const { return "A"; }

    virtual u128 obj_type_id() const { return u128(123,456); }

    virtual ~A() {}

    boole init(s64 in)
    {
        value = in + 10;
        return boole::True;
    }

    s64 value;
};

struct B1 : public obj<B1>
{
    virtual const char* obj_type() const { return "B1"; }

    virtual u128 obj_type_id() const { return u128(777, 888); }

    virtual ~B1() {}

    boole init(s64 in)
    {
        value = in + 10;
        return boole::True;
    }

    const char* B1_Func()
    {
        return "B1_Func";
    }

    virtual const char* Func()
    {
        return "Func 1";
    }

    s64 value;
};

struct B2 : public obj<B2>
{
    virtual const char* obj_type() const { return "B2"; }

    virtual u128 obj_type_id() const { return u128(888, 999); }

    virtual ~B2() {}

    boole init(s64 in)
    {
        value = in + 10;
        return boole::True;
    }

    const char* B2_Func()
    {
        return "B2_Func";
    }

    virtual const char* Func()
    {
        return "Func 2";
    }

    s64 value;
};

struct B3 : public obj<B3, has<B1, B2>>
{
    virtual const char* obj_type() const { return "B3"; }

    virtual u128 obj_type_id() const { return u128(000, 000); }

    virtual ~B3() {}

    boole init(s64 in)
    {
        value = in + 30;
        return boole::True;
    }

    const char* B3_Func()
    {
        return "B3_Func";
    }

    virtual const char* Func() override
    {
        return "Func 3";
    }

    s64 value;
};

void UT_Object()
{
    auto oA = A::build(25);
    assert(oA->obj_type() == "A");
    assert(oA->obj_type_id() == u128(123, 456));
    assert(oA->value == 35);

    auto oB = B3::build(25);
    assert(oB->obj_type() == "B3");
    assert(oB->obj_type_id() == u128(000, 000));
    assert(oB->B1_Func() == "B1_Func");
    assert(oB->B2_Func() == "B2_Func");
    assert(oB->Func() == "Func 3");
    assert(oB->value == 55);
}
