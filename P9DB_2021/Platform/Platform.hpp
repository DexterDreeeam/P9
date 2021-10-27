#pragma once

namespace P9
{

namespace Platform
{

class platform : object
{
public:
    platform() :
        _storage(nullptr),
        _interpreter(nullptr)
    {}

    ~platform()
    {
        if (_interpreter)
        {
            delete _interpreter;
        }
        if (_storage)
        {
            delete _storage;
        }
    }

    void load(const string& location);

    ref<Interpreter::query_operation> parse_operation_message(const string& str);

    string handle_operation(ref<Interpreter::query_operation> op);

private:
    string handle_upsert(ref<Interpreter::query_operation_upsert> op);

    string handle_retrieve(ref<Interpreter::query_operation_retrieve> op);

    string handle_hard_delete(ref<Interpreter::query_operation_hard_delete> op);

    string handle_search(ref<Interpreter::query_operation_search> op);

private:
    Storage::storage* _storage;
    Interpreter::base_interpreter* _interpreter;
};

}
}
