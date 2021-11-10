#include "../../Interface.hpp"

namespace P9
{

namespace Storage
{

string document_identifier::read()
{
    AUTO_TRACE;

    file f;
    f.init_input(_location.data());
    char* buf = new char[8192];
    escape_function ef = [=]() mutable
    {
        delete[] buf;
    };

    s64 read_len;
    f.input(buf, 8192, read_len);
    if (read_len >= 8192)
    {
        return string();
    }
    buf[read_len] = 0;
    string rst = string(buf);
    return rst;
}

}
}
