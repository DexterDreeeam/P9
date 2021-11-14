
#include "../EnvironmentHeader.hpp"
#include "../../Interface.hpp"

void yield()
{
	::SwitchToThread();
}

void barrier()
{
	MemoryBarrier();
}

u64 current_thread_id()
{
	return (u64)::GetCurrentThreadId();
}
