#include <stdlib.h>
#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <world/world.h>

const int SCREEN_WIDTH = 1920 / 2;
const int SCREEN_HEIGHT = 1080 / 2;

int main()
{
    if(glfwInit() != GLFW_TRUE)
    {
        printf("No glfw :(\n");
    }
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sburro", NULL, NULL);
    glfwMakeContextCurrent(window);

    if(glewInit() != GLEW_OK)
    {
        printf("No glew :(\n");
    }



    World world = worldNew();

    Entity block = ecsAddEntity(&world.ecs);
    Entity block2 = ecsAddEntity(&world.ecs);

    ecsAddComponent(&world.ecs, block, ECS_COMPONENT_TRANSFORM);
    ecsAddComponent(&world.ecs, block2, ECS_COMPONENT_TRANSFORM);

    ecsAddComponent(&world.ecs, block, ECS_COMPONENT_SPRITE);
    ecsAddComponent(&world.ecs, block2, ECS_COMPONENT_SPRITE);

    for(int i = 0; i < ECS_COMPONENTS_TOTAL; i++)
    {
        printf("%d %d\n", ((ecsComponent*)ecsGetComponent(&world.ecs, block, i))->generation, ((ecsComponent*)ecsGetComponent(&world.ecs, block, i))->entity.generation);
    }

    ecsRemoveComponent(&world.ecs, block, ECS_COMPONENT_TRANSFORM);
    ecsAddComponent(&world.ecs, block, ECS_COMPONENT_TRANSFORM);

    printf("\n");
    for(int i = 0; i < ECS_COMPONENTS_TOTAL; i++)
    {
        printf("%d %d\n", ((ecsComponent*)ecsGetComponent(&world.ecs, block, i))->generation, ((ecsComponent*)ecsGetComponent(&world.ecs, block, i))->entity.generation);
    }

    ecsTransform* sburro;
    ecsMesh* sbirro;



    while(!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.1, 0.1, 0.15, 1.0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}