#pragma once

namespace P9
{

namespace Storage
{

class partition : object
{
public:
    partition() = delete;

    partition(const string& name, const string& parent_location) :
        _name(name),
        _location(parent_location + name + '/'),
        _op_lock(),
        _document_map(ref<index_document_table>::new_instance(_location)),
        _index_table_map()
    {
        _op_lock.init();
    }

    ~partition()
    {
        _op_lock.uninit();
    }

    ref<index_document_table> get_document_table()
    {
        return _document_map;
    }

    string location() const
    {
        return _location;
    }

    ref<index_table> get_index_table(const string& index_table_name);

    ref<index_table> insert_index_table(const string& index_table_name);

    void remove_index_table(const string& index_table_name);

    boole upsert(ref<document_identifier> r_doc, const string& op_etag, ref<json_base> content);

    void build_document_index(ref<document_identifier> r_doc, ref<json_base> content);

    void clear_document_index(ref<document_identifier> r_doc);

    boole hard_delete(const string& document_id);

private:
    string       _name;
    string       _location;
    rw_lock      _op_lock;

private:
    ref<index_document_table> _document_map;
    unordered_map<string, ref<index_table>> _index_table_map;
};

}
}
