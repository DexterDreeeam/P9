#pragma once

namespace P9
{

namespace Storage
{

class index_table_json_comparer
{
public:
    s64 operator ()(ref<json_base> j1, ref<json_base> j2)
    {
        assert(j1.has_value() && j2.has_value());
        assert(j1->type() == j2->type());
        assert(j1->type() != json_type::ARRAY && j1->type() != json_type::OBJECT);
        assert(j2->type() != json_type::ARRAY && j2->type() != json_type::OBJECT);
        switch (j1->type())
        {
        case json_type::NONE:
        case json_type::BOOLE:
        case json_type::STRING:
        {
            return str_compare(j1->value().data(), j2->value().data());
        }
        case json_type::NUMBER:
        {
            number f1 = j1.ref_of<json_number>()->as_number();
            number f2 = j2.ref_of<json_number>()->as_number();
            return f1 == f2 ? 0 : f1 < f2 ? -1 : 1;
        }
        default:
        break;
        }
        err("Unexpected compare index json type.");
        assert(0);
        return 0;
    }
};

class index_table_identifier_comparer
{
public:
    s64 operator ()(ref<document_identifier> i1, ref<document_identifier> i2)
    {
        assert(i1->_guid.data() && i2->_guid.data());
        return str_compare(i1->_guid.data(), i2->_guid.data());
    }
};

class index_document_table : object
{
public:
    index_document_table() = delete;

    index_document_table(const string& parent_location) :
        _op_lock()
    {
        _op_lock.init();
    }

    ~index_document_table()
    {
        _op_lock.uninit();
    }

    ref<document_identifier> get_document(const string& document_id);

    ref<document_identifier> try_insert_document(ref<document_identifier> r_doc);

    void insert_or_replace_document(ref<document_identifier> r_doc);

    boole remove_document(const string& document_id);

    template<typename Fn_Ty>
    boole find_or_insert_do(ref<document_identifier> r_doc, Fn_Ty fn);

public:
    rw_lock _op_lock;

private:
    map<string, ref<document_identifier>> _document_map;
};

class index_table : object
{
public:
    using Set_Ty = set<ref<document_identifier>, index_table_identifier_comparer>;
    using Map_Ty = map<ref<json_base>, Set_Ty, index_table_json_comparer>;

public:
    index_table() :
        _op_lock(),
        document_map()
    {
        _op_lock.init();
    }

    ~index_table()
    {
        assert(document_map.size() == 0);
        _op_lock.uninit();
    }

    Set_Ty get_documents(ref<json_base> key);

    boole insert_document(ref<json_base> key, ref<document_identifier> identifier);

    boole remove_document(ref<json_base> key, ref<document_identifier> identifier);

    Map_Ty& get_control()
    {
        _op_lock.wait_write();
        return document_map;
    }

    void put_control()
    {
        _op_lock.write_release();
    }

public:
    rw_lock _op_lock;

private:
    Map_Ty document_map;
};

}
}
