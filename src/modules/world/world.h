#ifndef TQ_MOD_WORLD_H
#define TQ_MOD_WORLD_H

#include "ecs/ecs.h"

//WorldData struct
//Used for storing... just fucking guess
//Just "global" data that does not belong neither here nor there basically
typedef struct
{
    float timeElapsed;
    float timeDelta;
    float timeFrameStart;

    
}worldData;

typedef struct
{
    Ecs ecs;
    worldData data;
}World;

World worldNew();

#endif