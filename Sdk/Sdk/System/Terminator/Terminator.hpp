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

namespace _InternalNs
{

#define SERVICE_TERMINATE_SYMBOL_FILE_EXTENSION  ".terminating"
const char* const service_terminate_symbol_file = P9_FOLDER SERVICE_NAME SERVICE_TERMINATE_SYMBOL_FILE_EXTENSION;

class terminator;

_INLINE_ terminator& get_terminator();

class terminator
{
public:
    terminator() :
        _check_running(0),
        _need_terminate(0),
        _thrd()
    {
        print("Terminate progress using \'%s\' symbol file.\n", service_terminate_symbol_file);
    }

    ~terminator()
    {
        if (_check_running.get())
        {
            _thrd.uninit();
            file::remove(service_terminate_symbol_file);
        }
    }

    static void check_loop(void* p)
    {
        p;
        while (1)
        {
            tick::sleep(500);
            if (file::exist(service_terminate_symbol_file))
            {
                get_terminator()._need_terminate.set(1);
                break;
            }
        }
    }

    boole check()
    {
        if (_check_running.exchange(1) == 1)
        {
            return _need_terminate.get() != 0;
        }
        else
        {
            _thrd.init(terminator::check_loop);
            _thrd.start(nullptr);
            return boole::False;
        }
    }

private:
    atom<s64> _check_running;
    atom<s64> _need_terminate;
    thread    _thrd;
};

_INLINE_ terminator& get_terminator()
{
    static terminator t;
    return t;
}

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
    return _InternalNs::get_terminator().check();
}
