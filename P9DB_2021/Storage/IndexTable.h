#pragma once

namespace P9
{

namespace Storage
{

class index_document_id
{
public:
    index_document_id() = delete;

    index_document_id(const string& parent_location) :
        _partition_location(parent_location),
        _op_lock(rw_lock_create())
    {}

    ~index_document_id()
    {
        assert(document_map.size() == 0);
        rw_lock_destroy(_op_lock);
    }

    ref<document_identifier> get_document(const string& document_id);

    boole insert_document(const string& document_id, ref<document_identifier> r_doc);

    boole remove_document(const string& document_id);

public:
    string  _partition_location;
    rw_lock _op_lock;

private:
    map<string, ref<document_identifier>> document_map;
};

class index_table
{
public:
    index_table() :
        _op_lock(rw_lock_create())
    {}

    ~index_table()
    {
        assert(document_map.size() == 0);
        rw_lock_destroy(_op_lock);
    }

    set<string> get_documents(ref<json_base> key);

    boole insert_document(ref<json_base> key, const string& document_id);

    boole remove_document(ref<json_base> key, const string& document_id);

public:
    rw_lock _op_lock;

private:
    map<ref<json_base>, set<string>> document_map;
};

}
}
