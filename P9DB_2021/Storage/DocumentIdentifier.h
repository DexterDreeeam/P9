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
        _location(parent_location + id),
        _op_lock(rw_lock_create()),
        _inited(boole::False)
    {
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
    string  _location;
    rw_lock _op_lock;
    boole   _inited;
};

}
}
