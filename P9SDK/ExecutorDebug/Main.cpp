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

    auto f = input_file_create("C:/P9_Folder/test.txt");
    char buf[8000];
    s64 len = input_file_read(f, buf, 8000);
    buf[len] = 0;
    print("%d\n", len);
    str_pick_out_blank(buf);

    string str = buf;

    //log(buf);
    auto r = ref<int>::new_instance(1);

    auto j = json_deserialize(buf);

    auto cur = json_cursor(j);
    cur = cur["menu"]["popup"]["menuitem"][2]["onclick"];
    auto ele = *cur;

    print("\n%s\n", ele.data());

    // log("dexter-test %S", ele);

    // json_report(j);

    log("%s", j->type_name());

    delete j;

    return 0;
}
