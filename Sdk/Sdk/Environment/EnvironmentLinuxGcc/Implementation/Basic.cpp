
#include "../EnvironmentHeader.hpp"
#include "../../_Interface.hpp"

void yield()
{
	::pthread_yield();
}

void barrier()
{
	// todo
}

u64 current_thread_id()
{
	return (u64)::pthread_self();
}
