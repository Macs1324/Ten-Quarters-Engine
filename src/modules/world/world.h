#ifndef TQ_MOD_WORLD_H
#define TQ_MOD_WORLD_H

#include "ecs/ecs.h"

typedef struct
{
    Ecs ecs;
}World;

World worldNew();

#endif