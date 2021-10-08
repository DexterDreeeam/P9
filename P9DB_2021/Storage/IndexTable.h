#pragma once

namespace P9
{

namespace Storage
{

class index_table
{
public:
    index_table() = delete;

    index_table(const string& name, const string& parent_location) :
        _name(name),
        _location(parent_location + name + '/'),
        _op_lock(rw_lock_create())
    {}

    ~index_table()
    {
        assert(document_map.size() == 0);
        rw_lock_destroy(_op_lock);
    }

    ref<document_identifier> get_document(const string& document_id);

    ref<document_identifier> insert_document(const string& document_id);

    void insert_index(ref<document_identifier> r);

    void remove_document(const string& document_id);

public:
    string  _name;
    string  _location;
    rw_lock _op_lock;

private:
    map<string, ref<document_identifier>> document_map;
};

}
}
