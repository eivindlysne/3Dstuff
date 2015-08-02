#include "timer.h"
// #ifdef OS_LINUX
    #include <sys/time.h>
    static const long NANOSECONDS_PER_SECOND = 1000000000L;
// #endif

// #ifdef OS_OTHER
//     #include <SDL2/SDL.h>
// #endif

// TODO: Working cross platform
double get_time() {

// #ifdef OS_LINUX
    struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
    return (double) (((long) ts.tv_sec * NANOSECONDS_PER_SECOND) + ts.tv_nsec)
        / ((double) (NANOSECONDS_PER_SECOND));
// #endif

// #ifdef OS_OTHER
//     return (double) SDL_GetTicks() / 1000.0;
// #endif
}
