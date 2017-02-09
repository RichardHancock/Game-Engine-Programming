#pragma once

#ifdef _MSC_VER
//This is done because bullet is full of warnings and Visual Studio pollutes my warnings with them. This only disables warning in bullet, not my code.
#pragma warning(push, 0) 
#include <btBulletDynamicsCommon.h>
#pragma warning(pop)

#else 

#include <btBulletDynamicsCommon.h>

#endif // _WIN32
