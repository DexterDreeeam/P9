
#include "../../Interface.hpp"
#include "../EnvironmentHeader.hpp"

static class shadow_class
{
public:
    HANDLE _event_handle;
};

event::event()
{
    assert(_mem_sz >= sizeof(shadow_class));

    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class*);
    shadow_self._event_handle = ::CreateEventA(NULL, FALSE, NULL, NULL);
}

event::~event()
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class*);

    ::CloseHandle(shadow_self._event_handle);
}

void event::wait()
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class*);

    ::WaitForSingleObject(shadow_self._event_handle, INFINITE);
}

void event::set()
{
    auto& shadow_self = *pointer_convert(_mem, 0, shadow_class*);

    ::SetEvent(shadow_self._event_handle);
}
