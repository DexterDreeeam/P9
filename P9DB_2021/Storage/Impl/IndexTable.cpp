#include "../Interface.hpp"

namespace P9
{

namespace Storage
{

ref<document_identifier> index_document_table::get_document(const string& document_id)
{
    AUTO_TRACE;

    ref<document_identifier> rst;
    rw_lock_wait_read(_op_lock);
    escape_function ef =
        [=]() mutable
        {
            rw_lock_put_read(_op_lock);
        };

    auto itr = _document_map.find(document_id);
    if (itr != _document_map.end())
    {
        rst = itr->second;
    }
    return rst;
}

ref<document_identifier> index_document_table::insert_document(
    const string& document_id, ref<document_identifier> r_doc)
{
    AUTO_TRACE;

    boole rst;
    rw_lock_wait_write(_op_lock);
    escape_function ef =
        [=]() mutable
        {
            rw_lock_put_write(_op_lock);
        };

    auto itr = _document_map.find(document_id);
    if (itr != _document_map.end())
    {
        return itr->second;
    }
    else
    {
        _document_map[document_id] = r_doc;
        return r_doc;
    }
}

void index_document_table::insert_or_replace_document(const string& document_id, ref<document_identifier> r_doc)
{
    AUTO_TRACE;

    boole rst;
    rw_lock_wait_write(_op_lock);
    escape_function ef =
        [=]() mutable
    {
        rw_lock_put_write(_op_lock);
    };

    _document_map[document_id] = r_doc;
}

boole index_document_table::remove_document(const string& document_id)
{
    AUTO_TRACE;

    rw_lock_wait_write(_op_lock);
    escape_function ef =
        [=]() mutable
        {
            rw_lock_put_write(_op_lock);
        };

    auto itr = _document_map.find(document_id);
    if (itr != _document_map.end())
    {
        _document_map.erase(itr);
        return boole::True;
    }
    else
    {
        return boole::False;
    }
}

template<typename Fn_Ty>
boole index_document_table::find_or_insert_do(const string& document_id, ref<document_identifier> r_doc, Fn_Ty fn)
{
    AUTO_TRACE;

    boole test_rst;

    rw_lock_wait_write(_op_lock);
    escape_function ef =
        [=]() mutable
    {
        rw_lock_put_write(_op_lock);
    };

    auto itr = _document_map.find(document_id);
    if (itr != _document_map.end())
    {
        fn(itr->second);
        return boole::True;
    }
    else
    {
        _document_map[document_id] = r_doc;
        fn(r_doc);
        return boole::False;
    }
}

index_table::Set_Ty index_table::get_documents(ref<json_base> key)
{
    AUTO_TRACE;

    Set_Ty rst;

    rw_lock_wait_read(_op_lock);
    escape_function ef =
        [=]() mutable
        {
            rw_lock_put_read(_op_lock);
        };

    auto itr = document_map.find(key);
    if (itr != document_map.end())
    {
        rst = itr->second;
    }
    return rst;
}

boole index_table::insert_document(ref<json_base> key, ref<document_identifier> identifier)
{
    AUTO_TRACE;

    rw_lock_wait_write(_op_lock);
    escape_function ef =
        [=]() mutable
        {
            rw_lock_put_write(_op_lock);
        };

    auto& document_set = document_map[key];
    if (document_set.size() == 0 || document_set.find(identifier) == document_set.end())
    {
        document_set.insert(identifier);
        return boole::True;
    }
    else
    {
        return boole::False;
    }
}

boole index_table::remove_document(ref<json_base> key, ref<document_identifier> identifier)
{
    AUTO_TRACE;

    boole ret = boole::False;
    rw_lock_wait_write(_op_lock);
    escape_function ef =
        [=]() mutable
        {
            rw_lock_put_write(_op_lock);
        };

    auto itr = document_map.find(key);
    if (itr != document_map.end())
    {
        auto& document_set = itr->second;
        if (document_set.erase(identifier))
        {
            return boole::True;
        }
    }
    return boole::False;
}

}
}
