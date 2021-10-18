#include "../Interface/Interface.hpp"

class TestClassA
{
public:
    s64 aa;

    TestClassA(s64 a) :
        aa(a)
    {}

    ~TestClassA()
    {
        s64 aaa = aa;
        ++aaa;
        print("%d\n", aaa);
    }
};


int main()
{
    boole test;
    observer<TestClassA> ob1;
    observer<TestClassA> ob3;
    {
        sizeof(ref<TestClassA>);
        ref<TestClassA> r3;
        {
            ref_base rb;
            {
                auto r = ref<TestClassA>::new_instance(3);
                rb = r;
            }
            r3 = rb.ref_of<TestClassA>();
            ob1 = r3;
        }
        auto ob2 = r3.observer();
        ob3 = ob2;
        ref<TestClassA> rr1(ob1);
        if (rr1)
        {
            print("111");
        }
    }
    ref<TestClassA> rr2(ob1);
    if (rr2)
    {
        print("222");
    }

    array<int, 3> ai = { 1, 2, 3 };
    vector<int> vi = { 1, 2, 3 };

    //network_server ns = network_server_create(39900);
    //network_connect cn = network_connect_create(ns);
    //while (1)
    //{
    //    tick_sleep(1000);
    //    print("Health.");
    //    while (1)
    //    {
    //        char buf[1024] = {};
    //        s64 msg_len = network_connect_wait_receive(cn, buf);
    //        if (msg_len <= 0)
    //        {
    //            print("Finish.");
    //            return 0;
    //        }
    //        print(buf);
    //        print(".\n=========================== receive msg.\n");
    //    }
    //}

    network_client nc = network_client_create("10.17.17.153", 39900);
    print("Health.\n");
    s64 i = 0;
    while (1)
    {
        tick_sleep(1000);
        char buf[16] = {};
        s64 len = s64_to_text(++i, buf);
        buf[len] = 0;
        if (!network_client_send(nc, buf, len + 1))
        {
            print("Finish.\n");
            return 0;
        }
        print("Send %d.\n", i);
    }

    return 0;
}
