#include "Resource.h"

#include <assert.h>
#include <climits>

Resource::Resource()
{
	instances = 1;
	keepResourceLoaded = false;
}

bool Resource::keepLoaded()
{
	return keepResourceLoaded;
}

void Resource::keepLoaded(bool active)
{
	keepResourceLoaded = active;
}