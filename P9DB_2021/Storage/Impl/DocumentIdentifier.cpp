#include "../Interface.hpp"

namespace P9
{

namespace Storage
{

string document_identifier::read()
{
    auto f = input_file_create(_location.data());
    char* buf = new char[8192];
    s64 len = input_file_read(f, buf, 8192);
    if (len >= 8192)
    {
        return string();
    }
    buf[len] = 0;
    string rst = string(buf);
    delete[] buf;
    return rst;
}

}
}
