#pragma once

namespace P9
{

namespace Storage
{

class document_identifier
{
public:
    document_identifier() = delete;

    document_identifier(const string& id, const string& parent_location) :
        _id(id),
        _token(),
        _location(),
        _op_lock(rw_lock_create()),
        _inited(boole::False)
    {
        char buf[32] = {};
        s64 len = 0;
        random(u64_max, boole::True);
        for (s64 i = 0; i < 8; ++i)
        {
            len = u64_to_text_hex_padding(random(u64_max), buf, 4);
            buf[len] = 0;
            _token += buf;
        }

        _location = parent_location + _token;
    }

    ~document_identifier()
    {
        rw_lock_destroy(_op_lock);
    }

    boole init();

    boole uninit();

    string read();

    void overwrite(const string& str);

public:
    string  _id;
    string  _token;
    string  _location;
    rw_lock _op_lock;
    boole   _inited;
};

}
}
