#ifndef TQ_MOD_WORLD_ECS_COMPONENTS_H
#define TQ_MOD_WORLD_ECS_COMPONENTS_H

#include <cglm/cglm.h>
#include "../generationalIndex.h"

//The component "abstract" struct
//Every component MUST have this as the FIRST element
//So that any component pointer can be type-agnostically casted to an ecsComponent pointer
typedef struct
{
    /*
        The generation. If it gets indexed by a previous generation, it means that the handle is outdated and
        the component has been deleted and it's a "use after free" case. So this is mostly to avoid using garbage memory
    */
    int generation;

    /*  
        When you try to get a component, if it's not alive (thus its parent doesn't exist anymore) 
        you'll get a big fat NULL instead
        (to avoid use after free)
        A component that isn't alive anymore is waiting to get overwritten by the addComponent function 
    */
    unsigned int alive; 

    /*  
        The (generational) index the component is attached to
        This creates a potentially unstable double link, but I'll try to make sure that it'll
        always get updated together with the handle (if the user a.k.a. me doesn't do anything stupid with handles)
    */
    GenerationalIndex entity;
}ecsComponent;

//Transform component
typedef struct
{
    ecsComponent component;
    
    vec3 position;
    vec3 rotation;
    vec3 scale;
}ecsTransform;

//Mesh component
typedef struct
{
    ecsComponent component;

    float* dataVertices;
    unsigned int* dataIndices;

    unsigned int bufferVertex;
    unsigned int bufferVertexArray;
    unsigned int bufferIndex;
    unsigned int counterIndex;
}ecsMesh;


//Box collider component
typedef struct
{
    ecsComponent component;

    vec3 center;
    vec3 size;
}ecsBoxCollider;

//Sprite component
typedef struct
{
    ecsComponent component;

    unsigned int texture;
}ecsSprite;

#endif