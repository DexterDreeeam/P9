#include "../Interface.hpp"

namespace P9
{

namespace Storage
{

boole document_identifier::init()
{
    if (create_file(_location.data()))
    {
        _inited = boole::True;
        return boole::True;
    }
    else
    {
        return boole::False;
    }
}

boole document_identifier::uninit()
{
    if (_inited)
    {
        if (delete_file(_location.data()))
        {
            _inited = boole::False;
        }
    }
    return _inited == boole::False;
}

string document_identifier::read()
{
    rw_lock_wait_read(_op_lock);
    escape_function ef =
        [=]() mutable
        {
            rw_lock_put_read(_op_lock);
        };

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

void document_identifier::overwrite(const string& str)
{
    rw_lock_wait_write(_op_lock);
    escape_function ef =
        [=]() mutable
        {
            rw_lock_put_write(_op_lock);
        };

    auto f = output_file_create(_location.data(), boole::True);
    output_file_write(f, str.data());
    output_file_destroy(f);
}

}
}
