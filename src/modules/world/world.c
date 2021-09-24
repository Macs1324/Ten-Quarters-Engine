#ifndef TQ_MOD_WORLD_C
#define TQ_MOD_WORLD_C

#include "world.h"

World worldNew()
{
    World r;
    r.ecs = ecsNew();

    return r;
}


#endif