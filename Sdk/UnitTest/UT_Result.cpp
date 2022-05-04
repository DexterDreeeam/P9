
#include "../_Interface.hpp"

sdk::result<int> UT_Result_SUCCESS_Int_678()
{
    return { sdk::result_type::Success, 678 };
}

sdk::result<> UT_Result_Fail_Void()
{
    sdk::result<> r = { sdk::result_type::Fail };
    return r;
}

void UT_Result()
{
    auto r1 = UT_Result_SUCCESS_Int_678();
    assert(r1.type() == sdk::result_type::Success);
    assert(r1.content() == 678);

    auto r2 = UT_Result_Fail_Void();
    assert(r2.type() == sdk::result_type::Fail);
}
