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
        _op_lock(),
        _status(document_identifier_status::CREATING)
    {
        file f;
        f.init_output(_location.data(), boole::True);
        f.output(content.data());
        f.uninit();

        _op_lock.init();
    }

    ~document_identifier()
    {
        _op_lock.uninit();
    }

    string read();

    boole is_status(document_identifier_status s)
    {
        return _status == s;
    }

    document_identifier_transform_result transform(document_identifier_status from, document_identifier_status to)
    {
        document_identifier_transform_result rst = document_identifier_transform_result::WRONG_STATUS;
        _op_lock.wait_acquire();
        escape_function ef =
            [=]()
            {
                _op_lock.release();
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
        _op_lock.wait_acquire();
        escape_function ef =
            [=]()
            {
                _op_lock.release();
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
