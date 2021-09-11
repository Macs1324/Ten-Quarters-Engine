#ifndef TQ_MOD_WORLD_ECS_H
#define TQ_MOD_WORLD_ECS_H

#include "components/components.h"
#include "generationalIndex.h"

#define ECS_MAX_COMPONENTS 4096

enum ecsComponents
{
    ECS_COMPONENT_ZERO,

    ECS_COMPONENT_TRANSFORM,
    ECS_COMPONENT_MESH,

    ECS_COMPONENTS_TOTAL
};

//I'm using a generational array pattern to 
typedef GenerationalIndex Entity;

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

        ofc each component type will most likely have a different size, so I'll have to store an array of array start positions (int) that
        gets indexed by the enum. Kind of a dirty hack but it's probably worth it
*/

typedef struct
{
    //This is of type char so that it's an array of bytes
    //When you cast the type to the needed type you should be alright
    unsigned char components[
        sizeof(ecsTransform) * ECS_MAX_COMPONENTS +
        sizeof(ecsMesh) * ECS_MAX_COMPONENTS + 
        sizeof(ecsBoxCollider) * ECS_MAX_COMPONENTS
    ];

    //An array of counters, to keep track of how many of which
    //components are stored
    unsigned int counter[ECS_COMPONENTS_TOTAL];

    //The start positions array described above
    //It should optimally be calculated at compile time and stored as some global variable, 
    //but since I don't have a global init function or similar I think it's cleaner (and maybe more flexible?)
    //to keep it inside an ECS instance and calculate it inside the "constructor"
    unsigned int startPositions[ECS_COMPONENTS_TOTAL];
}Ecs;

#endif