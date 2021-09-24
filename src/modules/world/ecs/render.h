#ifndef TQ_MOD_SYS_RENDER_H
#define TQ_MOD_SYS_RENDER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>


#include "../world.h"

void render(World* world)
{
    Ecs* ecs = &world->ecs;

    //Iterate through each sprite
    //In the ecs
    for(int i = 0; i < ecs -> counterComponents[ECS_COMPONENT_SPRITE]; i++)
    {
        //Do some sprite rendering stuff

        ecsSprite* sprite = (ecsSprite*)ecs->components + 
            ecs->componentStartIndices[ECS_COMPONENT_SPRITE] + (i * ECS_COMPONENTSIZE[ECS_COMPONENT_SPRITE]);

        if(
            ecsGetEntityData(ecs, sprite->component.entity)->componentIndices[ECS_COMPONENT_SPRITE].index != -1
        )
        {
            ecsMesh* mesh = (ecsMesh*)ecsGetComponent(ecs, sprite->component.entity, ECS_COMPONENT_MESH);
            glBindVertexArray(mesh->bufferVertex);
            glBindBuffer(GL_VERTEX_ARRAY, mesh->bufferVertex);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->bufferIndex);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, sprite->texture);

            glDrawElements(GL_TRIANGLES, mesh->counterIndex, GL_UNSIGNED_INT, 0);
        }
        else
        {
            printf("Cannot draw sprite attached to an entity without a mesh!\n");
        }
    }
}

#endif