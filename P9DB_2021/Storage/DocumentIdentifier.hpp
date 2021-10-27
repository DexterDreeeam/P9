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
        _guid(guid::new_instance().as_string()),
        _location(),
        _op_lock(rw_lock_create()),
        _inited(boole::False)
    {
        _location = parent_location + _guid;
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
    string  _guid;
    string  _location;
    rw_lock _op_lock;
    boole   _inited;
};

}
}
