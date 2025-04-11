#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

#include "engine/display.h"
#include "engine/shader.h"
#include "engine/input.h"
#include "engine/file.h"
#include "engine/mesh.h"
#include "engine/entity.h"
#include "engine/transform.h"
#include "engine/matrix.h"

int main(void)
{
    struct File vertexShaderFile = file_read("shader/basic.vert");
    if(vertexShaderFile.data == NULL)
    {
        fprintf(stderr, "Failed to read file: %s\n", vertexShaderFile.path);
        return 1;
    }
    const char* vertexShaderSource = vertexShaderFile.data;

    struct File fragmentShaderFile = file_read("shader/basic.frag");
    if(fragmentShaderFile.data == NULL)
    {
        fprintf(stderr, "Failed to read file: %s\n", fragmentShaderFile.path);
        return 1;
    }
    const char* fragmentShaderSource = fragmentShaderFile.data;


    struct Display display;
    display_init(&display, 800, 600, "engine");

    struct Shader shader;
    shader_init(&shader, vertexShaderSource, fragmentShaderSource);
    unsigned int modelUniformLocation = glGetUniformLocation(shader.program, "model");
    unsigned int viewUniformLocation = glGetUniformLocation(shader.program, "view");
    unsigned int projectionUniformLocation = glGetUniformLocation(shader.program, "projection");
    
    file_destroy(&vertexShaderFile);
    file_destroy(&fragmentShaderFile);

    struct Mesh triangle;
    mesh_init(&triangle);

    float vertices[] = {
        // positions
        -0.5f, -0.5f, 0.0f,  // bottom left
         0.5f, -0.5f, 0.0f,  // bottom right
         0.0f,  0.5f, 0.0f   // top
    };
    mesh_set_vertices(&triangle, vertices, sizeof(vertices));

    int indices[] = {
        0, 1, 2  // Triangle
    };
    mesh_set_indices(&triangle, indices, sizeof(indices) / sizeof(indices[0]));

    size_t entity_capacity = 10; // Start with capacity for 10 entities
    size_t entity_count = 0;     // Initially, we have 0 entities
    struct Entity** entities_to_draw = malloc(entity_capacity * sizeof(struct Entity*)); // Array of pointers
    if (entities_to_draw == NULL) {
        fprintf(stderr, "Failed to allocate memory for entities.\n");
        return 1;
    }

    struct Transform transform;
    transform_init(&transform);
    transform.x = 2.0f;
    
    struct Entity* entity = entity_create(&triangle, &transform);

    if (entity != NULL) { // Check if entity creation succeeded
        if (entity_count < entity_capacity) {
             entities_to_draw[entity_count] = entity;
             entity_count++;
        } else {
            // Optional: Handle resizing the array here later if needed using realloc
            fprintf(stderr, "Entity list full! (Need to implement resizing)\n");
        }
    } else {
         fprintf(stderr, "Failed to create entity1!\n");
    }

    Mat4 viewMatrix;
    matrix_create_camera(&viewMatrix);

    Mat4 projectionMatrix;
    float aspectRatio = (float)display.width / (float)display.height;
    float fovRadians = 3.14159f / 4.0f; // Example: 45 degrees FOV
    float nearClip = 0.1f;
    float farClip = 100.0f;

    matrix_create_perspective(&projectionMatrix, fovRadians, aspectRatio, nearClip, farClip);


    // render loop
    while(!display_should_close(&display))
    {
        processInput(display.window);
        
        // draw background
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader_use(&shader);

        shader_set_uniform_mat4(viewUniformLocation, &viewMatrix);
        shader_set_uniform_mat4(projectionUniformLocation, &projectionMatrix);

        // draw entities
        for (size_t i = 0; i < entity_count; ++i) {
            struct Entity* current_entity = entities_to_draw[i];
            
            if (current_entity && current_entity->mesh) {
                Mat4 modelMatrix;
                transform_to_matrix(&modelMatrix, current_entity->transform);
                shader_set_uniform_mat4(modelUniformLocation, &modelMatrix);
                glBindVertexArray(current_entity->mesh->VAO);
                glDrawElements(GL_TRIANGLES, current_entity->mesh->indexCount, GL_UNSIGNED_INT, 0);
                glBindVertexArray(0);
            }
        }
 
        display_update(&display);
    }

    for (size_t i = 0; i < entity_count; ++i) {
        entity_destroy(entities_to_draw[i]);
    }

    free(entities_to_draw);

    // optional: de-allocate all resources once they've outlived their purpose:
    mesh_destroy(&triangle);
    shader_destroy(&shader);
    display_destroy(&display);

    return 0;
}