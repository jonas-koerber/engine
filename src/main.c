#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

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
    struct File vertex_shader_file = file_read("shader/basic.vert");
    if(vertex_shader_file.data == NULL)
    {
        fprintf(stderr, "Failed to read file: %s\n", vertex_shader_file.path);
        return 1;
    }
    const char* vertex_shader_source = vertex_shader_file.data;

    struct File fragment_shader_file = file_read("shader/basic.frag");
    if(fragment_shader_file.data == NULL)
    {
        fprintf(stderr, "Failed to read file: %s\n", fragment_shader_file.path);
        return 1;
    }
    const char* fragment_shader_source = fragment_shader_file.data;


    struct Display display;
    display_init(&display, 800, 600, "engine");

    struct Shader shader;
    shader_init(&shader, vertex_shader_source, fragment_shader_source);
    unsigned int model_uniform_location = glGetUniformLocation(shader.program, "model");
    unsigned int view_uniform_location = glGetUniformLocation(shader.program, "view");
    unsigned int projection_uniform_location = glGetUniformLocation(shader.program, "projection");
    unsigned int color_uniform_location = glGetUniformLocation(shader.program, "color");
    
    file_destroy(&vertex_shader_file);
    file_destroy(&fragment_shader_file);

    struct Mesh triangle;
    mesh_init(&triangle);

    float vertices[] = {
        // Position           Normal
        // Front face (+Z)
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, // 0: Bottom left front
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, // 1: Bottom right front
         0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f, // 2: Top right front
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f, // 3: Top left front

        // Back face (-Z)
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,-1.0f, // 4: Bottom left back
         0.5f, -0.5f, -0.5f,  0.0f, 0.0f,-1.0f, // 5: Bottom right back
         0.5f,  0.5f, -0.5f,  0.0f, 0.0f,-1.0f, // 6: Top right back
        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,-1.0f, // 7: Top left back

        // Left face (-X)
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, // 8: Bottom left back (dup vert 4)
        -0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f, // 9: Bottom left front (dup vert 0)
        -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f, // 10: Top left front (dup vert 3)
        -0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f, // 11: Top left back (dup vert 7)

        // Right face (+X)
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, // 12: Bottom right front (dup vert 1)
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, // 13: Bottom right back (dup vert 5)
         0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f, // 14: Top right back (dup vert 6)
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, // 15: Top right front (dup vert 2)

        // Top face (+Y)
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, // 16: Top left front (dup vert 3)
         0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, // 17: Top right front (dup vert 2)
         0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, // 18: Top right back (dup vert 6)
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, // 19: Top left back (dup vert 7)

        // Bottom face (-Y)
        -0.5f, -0.5f, -0.5f,  0.0f,-1.0f, 0.0f, // 20: Bottom left back (dup vert 4)
         0.5f, -0.5f, -0.5f,  0.0f,-1.0f, 0.0f, // 21: Bottom right back (dup vert 5)
         0.5f, -0.5f,  0.5f,  0.0f,-1.0f, 0.0f, // 22: Bottom right front (dup vert 1)
        -0.5f, -0.5f,  0.5f,  0.0f,-1.0f, 0.0f  // 23: Bottom left front (dup vert 0)
    };
    mesh_set_vertices(&triangle, vertices, sizeof(vertices));

    unsigned int indices[] = {
        // Front face
        0, 1, 2,  2, 3, 0,
        // Back face
        5, 4, 7,  7, 6, 5,
        // Left face
        8, 9, 10, 10, 11, 8,
        // Right face
        12, 13, 14, 14, 15, 12,
        // Top face
        16, 17, 18, 18, 19, 16,
        // Bottom face
        20, 21, 22, 22, 23, 20
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
    transform.scale_z = 3.0f;
    transform.scale_y = 2.0f;
    transform.rotation_x = 0.5f;
    
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

    Mat4 view_matrix;
    matrix_create_camera(&view_matrix);

    Mat4 projection_matrix;
    float aspect_ratio = (float)display.width / (float)display.height;
    float fov_radians = 3.14159f / 4.0f; // Example: 45 degrees FOV
    float near_clip = 0.1f;
    float far_clip = 100.0f;

    matrix_create_perspective(&projection_matrix, fov_radians, aspect_ratio, near_clip, far_clip);


    // render loop
    while(!display_should_close(&display))
    {
        processInput(display.window);
        
        // draw background
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader_use(&shader);

        shader_set_uniform_mat4(view_uniform_location, &view_matrix);
        shader_set_uniform_mat4(projection_uniform_location, &projection_matrix);

        double time = glfwGetTime();

        // draw entities
        for (size_t i = 0; i < entity_count; ++i) {
            struct Entity* current_entity = entities_to_draw[i];
            
            if (current_entity && current_entity->mesh) {
                current_entity->transform->rotation_y = (float)time * 0.5f;

                Mat4 model_matrix;
                transform_to_matrix(&model_matrix, current_entity->transform);
                shader_set_uniform_mat4(model_uniform_location, &model_matrix);
                glBindVertexArray(current_entity->mesh->vao);
                glDrawElements(GL_TRIANGLES, current_entity->mesh->index_count, GL_UNSIGNED_INT, 0);
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