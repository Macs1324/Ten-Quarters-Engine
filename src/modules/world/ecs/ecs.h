#ifndef TQ_MOD_WORLD_ECS_H
#define TQ_MOD_WORLD_ECS_H

#include "components/components.h"
#include "generationalIndex.h"

#define ECS_MAX_COMPONENTS 4096

enum ecsComponentsEnum
{
    ECS_COMPONENT_ZERO,

    ECS_COMPONENT_TRANSFORM,
    ECS_COMPONENT_MESH,
    ECS_COMPONENT_BOX_COLLIDER,
    ECS_COMPONENT_SPRITE,

    ECS_COMPONENTS_TOTAL
};

static size_t ECS_COMPONENTSIZE[ECS_COMPONENTS_TOTAL] = {
    0,

    sizeof(ecsTransform),
    sizeof(ecsMesh),
    sizeof(ecsBoxCollider),
    sizeof(ecsSprite)
};

//I'm using a generational array pattern to recycle memory
typedef GenerationalIndex Entity;

typedef struct
{
    unsigned int generation;
    unsigned int alive;
    GenerationalIndex componentIndices[ECS_COMPONENTS_TOTAL];
}EntityData;

/*
    todo: Update this comment when the ecs is at a decent state

    TO MAKE THE ECS WORK, JUST ALLOCATE A SHIT TON OF MEMORY
    LIKE THIS:
        (sizeof(cTransform) * nrTransform) + (sizeof(cMesh) * nrMesh)
        etc...

        with nrTransform and nrMesh being stored in one single array (as they're all unsigned integers)
        and both the component array and the counter array are being indexed by the ecsComponents enum
        like this

        components[ ecsComponentTransformSrart + index] (This is pseudocode, update it later plz)

    SO WHEN YOU WANT TO GET A COMPONENT, YOU CALL AN ecsGetComponent FUNCTION THAT RETURNS A VOID POINTER,
    AND THEN U CAST IT TO WHATEVER COMPONENT YOU NEED

    MEMORY IS GONNA LOOK SOMETHING LIKE THIS:
           ecsTransform         ecsMesh               ecsBoxCollider        ecsSprite
        |1234567-------------|12345---------------|123-----------------|1234----------------|  .  .  .  . 

        ofc each component type will most likely have a different size, so I'll have to multiply and add shit
        to get to the right piece of memory

    

    MAINTENANCE:
        -add a component ID to the ecsComponentsEnum
        -grow the size of the components array by sizeof(newComponent) * ECS_MAX_COMPONENTS
        -add a size value to the ECS_COMPONENTSIZE array
*/

typedef struct
{
    //Generational array of generational indices 
    EntityData entities[ECS_MAX_COMPONENTS];
    //Specifies how many entities there ACTUALLY are in the ecs
    unsigned int counterEntities;


    //This is of type char so that it's an array of bytes
    //When you cast the type to the needed type you should be alright
    char components[
        sizeof(ecsTransform) * ECS_MAX_COMPONENTS +
        sizeof(ecsMesh) * ECS_MAX_COMPONENTS + 
        sizeof(ecsBoxCollider) * ECS_MAX_COMPONENTS +
        sizeof(ecsSprite) * ECS_MAX_COMPONENTS
    ];

    //An array of counters, to keep track of how many of which
    //components are stored
    unsigned int counterComponents[ECS_COMPONENTS_TOTAL];

    //SUPER IMPORTANT: Gets computed in the ecs constructor, and 
    //saves the starting indices of each component
    //Used to use the components array as an array of different types
    unsigned int componentStartIndices[ECS_COMPONENTS_TOTAL];
}Ecs;

Ecs ecsNew();

//This one is important: it returns the 
//index (basically a pointer) to the first component of a certain type
//So you can treat the big component array as an array of arrays (each component type has a different size)
int ecsGetIndex(Ecs* ecs, enum ecsComponentsEnum componentID);
//
// More info on these in the ecs.c file
//
Entity ecsAddEntity(Ecs* ecs);
void ecsAddComponent(Ecs* ecs, Entity entity, enum ecsComponentsEnum componentID);

void* ecsGetComponent(Ecs* ecs, Entity entity, enum ecsComponentsEnum componentID);
EntityData* ecsGetEntityData(Ecs* ecs, Entity entity);

void ecsRemoveEntity(Ecs* ecs, Entity* entity);
void ecsRemoveComponent(Ecs* ecs, Entity entity, enum ecsComponentsEnum componentID);

#endif