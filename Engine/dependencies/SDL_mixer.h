//This is due to the SDL linux package being in a weird folder, might have been a more elegant solution
//But this works and doesn't require messing with the linux distro itself. Also its all handled compile
// time so no performance hit.
#ifdef __ANDROID__
#include <SDL_mixer.h>
#elif dsad
#include "SDL2/SDL_mixer.h"
#else
#include <SDL_mixer.h>
#endif