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

void UT_Object()
{
    auto o = A::build(25);
    assert(o->value == 35);
}
