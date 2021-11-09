/* ********* * ********* * ********* * ********* * *********
 *
 * Environment_Base/Definition.h
 *
 * proj: Environment_Base
 * desc: precompile definitions
 * autr: dexterdreeeam
 * date: 20211001
 * tips:
 *
 ********* * ********* * ********* * ********* * ********* */

#pragma once

namespace TerminatorNs
{

#define SERVICE_TERMINATE_SYMBOL_FILE_EXTENSION  ".terminating"
const char* const service_terminate_symbol_file = P9_FOLDER SERVICE_NAME SERVICE_TERMINATE_SYMBOL_FILE_EXTENSION;


class terminator
{
public:
    terminator()
    {
    }

    ~terminator()
    {
        file::remove(service_terminate_symbol_file);
    }

};

_SELECTANY_ terminator global_terminator;

}

_INLINE_ void terminate(const char* service_name)
{
    s64 buf_len = 1;
    buf_len += str_len(P9_FOLDER);
    buf_len += str_len(service_name);
    buf_len += str_len(SERVICE_TERMINATE_SYMBOL_FILE_EXTENSION);

    buf_len = 0;
    char* buf = new char[buf_len];
    memory::copy(P9_FOLDER, buf + buf_len, str_len(P9_FOLDER));
    buf_len += str_len(P9_FOLDER);
    memory::copy(service_name, buf + buf_len, str_len(service_name));
    buf_len += str_len(service_name);
    memory::copy(SERVICE_TERMINATE_SYMBOL_FILE_EXTENSION, buf + buf_len, str_len(SERVICE_TERMINATE_SYMBOL_FILE_EXTENSION));
    buf_len += str_len(SERVICE_TERMINATE_SYMBOL_FILE_EXTENSION);
    buf[buf_len] = 0;
    file::create(buf);
    delete[] buf;
}

_INLINE_ boole am_i_terminated()
{
    return file::exist(TerminatorNs::service_terminate_symbol_file);
}
