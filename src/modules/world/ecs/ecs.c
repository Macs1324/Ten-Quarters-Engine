#ifndef TQ_MOD_WORLD_ECS_C
#define TQ_MOD_WORLD_ECS_C

#include "ecs.h"

/*
    Again, super important function to help us get the index of the first component
    of a type in the components array
*/
int ecsGetComponentStartIndex(Ecs* ecs, enum ecsComponentsEnum componentID)
{
    int r = 0;
    for(enum ecsComponentsEnum id = componentID; id > ECS_COMPONENT_ZERO; id--)
    {
        r += ECS_COMPONENTSIZE[id] * ECS_MAX_COMPONENTS;
    }
    return r;
}

/*
    The constructor for an ECS struct
*/
Ecs ecsNew()
{
    Ecs r;

    for(int i = ECS_COMPONENT_ZERO; i< ECS_COMPONENTS_TOTAL; i++)
    {
        r.componentStartIndices[i] = ecsGetComponentStartIndex(&r, i);
    }

    return r;
}

/*
    Replaces a dead entity with a new one
    If there are no dead entities it just allocates a new entity
*/
Entity ecsAddEntity(Ecs* ecs)
{
    int index = -1;
    int generation = -1;
    for(int i = 0; i < ecs->counterEntities; i++)
    {
        if(!ecs->entities[i].alive)
        {
            index = i;
            generation = ++ecs->entities[i].generation;
            break;
        }
    }

    //Handles the case in which no dead entities were found
    //Thus index remained unchanged
    if(index == -1)
    {
        index = ecs->counterEntities++;
        generation = 0;
    }

    return (Entity){index, generation};
}

/*
    Works sort of like ecsAddEntity, but acts on the component list instead
*/
void ecsAddComponent(Ecs* ecs, Entity entity, enum ecsComponentsEnum componentID)
{
    int index = -1;
    int generation = -1;
    
    for(int i = ecs->componentStartIndices[componentID]; i < ecs->counterComponents[componentID]; i+=ECS_COMPONENTSIZE[componentID])
    {
        if( !((ecsComponent*)(ecs->components[i]))->alive )
        {
            index = i / ECS_COMPONENTSIZE[componentID];
            generation = ((ecsComponent*)(ecs->components[i]))->generation;
        }
    }
    if(index == -1)
    {
        index = ecs->counterComponents[componentID]++;
        generation = 0;
    }
    

    //This is how you get a component from the char array
    ((ecsComponent*)( ecs->components + (ecs->componentStartIndices[componentID] ) ) )->alive = true;
    ((ecsComponent*)( ecs->components + (ecs->componentStartIndices[componentID] ) ) )->entity = entity;
}


/* 
    Check whether component is alive and return a pointer to it
    example: cTransform* myTransform = ecsGetComponent(&world.ecs, player, ECS_TRANSFORM)
*/
void* ecsGetComponent(Ecs* ecs, Entity entity, enum ecsComponentsEnum componentID)
{
    if(entity.index == -1)
    {
        printf("Cannot get component %d\n\tEntity handle is dead\n", componentID);
        return NULL;
    }
    else if(!ecs->entities[entity.index].alive)
    {
        printf("Cannot get component %d\n\tEntity is DEAD\n", componentID);
        return NULL;
    }
    else if(ecs->entities[entity.index].generation == entity.generation)
    {
        printf("Cannot get component %d\n\tEntity generations don't match\n", componentID);
        return NULL;
    }
    else
    {
        char* component = ecs->components + 
            (componentID * ECS_COMPONENTSIZE[componentID] * ECS_MAX_COMPONENTS) + 
                (ecs->entities[entity.index].componentIndices[componentID].index * ECS_COMPONENTSIZE[componentID]);

        if(ecs->entities[entity.index].componentIndices[componentID].index == -1)
        {
            printf("Cannot get component %d\n\tComponent does not exist for given entity\n", componentID);
            return NULL;
        }
        else if(ecs->entities[entity.index].generation != ecs->entities[entity.index].componentIndices[componentID].generation)
        {
            printf("Cannot get component %d\n\tComponent generations don't match\n", componentID);
            return NULL;
        }
        else if( !((ecsComponent*)(component))->alive )
        {
            printf("Cannot get component %d\n\tComponent is DEAD\n", componentID);
            return NULL;
        }
        else
        {
            return (void*)(component);
        }
    }
}
EntityData* ecsGetEntityData(Ecs* ecs, Entity entity)
{
    if(entity.index == -1)
    {
        printf("Cannot get entity %d\n\tEntity handle is dead\n", entity.index);
        return NULL;
    }
    else if(!ecs->entities[entity.index].alive)
    {
        printf("Cannot get entity %d\n\tEntity is DEAD\n", entity.index);
        return NULL;
    }
    else if(ecs->entities[entity.index].generation == entity.generation)
    {
        printf("Cannot get entity %d\n\tEntity generations don't match\n", entity.index);
        return NULL;
    }
    else
    {
        return &ecs->entities[entity.index];
    }
}

void ecsRemoveEntity(Ecs* ecs, Entity* entity)
{
    for(int i = ECS_COMPONENT_ZERO; i < ECS_COMPONENTS_TOTAL; i++)
    {
        ecsRemoveComponent(ecs, *entity, i);
    }
    ecsGetEntityData(ecs, *entity)->alive = false;
    entity->index = -1;
}

void ecsRemoveComponent(Ecs* ecs, Entity entity, enum ecsComponentsEnum componentID)
{
    ((ecsComponent*)ecsGetComponent(ecs, entity, componentID))->alive = false;
}

#endif