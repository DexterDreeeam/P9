#pragma once

void UT_Random()
{
    const char* func_name = __FUNCTION__;
    print("\n>>>>>>>>> [%s] Start >>>>>>>>>\n\n", func_name);
    escape_function ef = [=]()
    {
        print("\n<<<<<<<<< [%s] End <<<<<<<<<\n\n", func_name);
    };

    for (u64 mod = 8; mod <= 256; mod += 3)
    {
        vector<s64> summary((sz_t)mod, 0);
        for (s64 i = 0; i < mod * 512; ++i)
        {
            u64 bucket = random::new_u64() % mod;
            ++summary[bucket];
        }

        u64 cnt_max = u64_min;
        u64 cnt_min = u64_max;
        for (auto c : summary)
        {
            cnt_max = cnt_max >= c ? cnt_max : c;
            cnt_min = cnt_min <= c ? cnt_min : c;
        }

        if (cnt_min * 3 < cnt_max)
        {
            print("mod: %llu, min: %llu, max: %llu\n", mod, cnt_min, cnt_max);
            assert(0);
        }
    }
}
