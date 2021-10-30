#pragma once

namespace P9
{

namespace Storage
{

enum class document_identifier_status : s64
{
    CREATING,
    NORMAL,
    UPDATING,
    IMMIGRATING,
    DELETING,
};

enum class document_identifier_transform_result : s64
{
    SUCCEED,
    WRONG_STATUS,
    ETAG_CONFLICT,
};

class document_identifier : object
{
public:
    document_identifier() = delete;

    document_identifier(const string& id, const string& parent_location, const string& content) :
        _guid(guid::new_instance().as_string()),
        _location(parent_location + _guid),
        _document_id(id),
        _etag(guid::new_instance().as_string()),
        _op_lock(lock_create()),
        _status(document_identifier_status::CREATING)
    {
        auto f = output_file_create(_location.data(), boole::True);
        output_file_write(f, content.data());
        output_file_destroy(f);
    }

    ~document_identifier()
    {
        lock_destroy(_op_lock);
    }

    string read();

    boole is_status(document_identifier_status s)
    {
        return _status == s;
    }

    document_identifier_transform_result transform(document_identifier_status from, document_identifier_status to)
    {
        document_identifier_transform_result rst = document_identifier_transform_result::WRONG_STATUS;
        lock_wait_get(_op_lock);
        escape_function ef =
            [=]()
            {
                lock_put(_op_lock);
            };

        if (_status == from)
        {
            _status = to;
            rst = document_identifier_transform_result::SUCCEED;
        }

        return rst;
    }

    document_identifier_transform_result transform_if_etag_identical(document_identifier_status from, document_identifier_status to, const string& etag)
    {
        document_identifier_transform_result rst = document_identifier_transform_result::WRONG_STATUS;
        lock_wait_get(_op_lock);
        escape_function ef =
            [=]()
            {
                lock_put(_op_lock);
            };

        if (_status == from)
        {
            if (_etag == etag)
            {
                _status = to;
                rst = document_identifier_transform_result::SUCCEED;
            }
            else
            {
                rst = document_identifier_transform_result::ETAG_CONFLICT;
            }
        }
        return rst;
    }

public:
    const string                _guid;
    const string                _location;
    const string                _document_id;
    const string                _etag;
    lock                        _op_lock;

private:
    document_identifier_status  _status;
};

}
}
