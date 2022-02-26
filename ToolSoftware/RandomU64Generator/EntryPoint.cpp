
#include "../../Sdk/Interface.hpp"

int main()
{
    for (s64 i = 0; i < 10; ++i)
    {
        char dec_chars[40] = {};
        char hex_chars[40] = {};
        u64 v = random::new_u64();
        u64_to_text(v, dec_chars);
        u64_to_text_hex(v, hex_chars);
        print("Dec: %s / Hex: %s.\n", dec_chars, hex_chars);
    }
    return 0;
}
