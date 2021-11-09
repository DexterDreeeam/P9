
#include "../../Interface.hpp"
#include "../EnvironmentHeader.hpp"

void yield()
{
	::SwitchToThread();
}

void barrier()
{
	MemoryBarrier();
}
