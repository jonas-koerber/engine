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
#include "engine/obj.h"

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

    struct Mesh cube;
    mesh_init(&cube);
    struct obj obj;
    struct File obj_file = file_read("object/alfa147.obj");
    if(obj_file.data == NULL)
    {
        fprintf(stderr, "Failed to read file: %s\n", obj_file.path);
        return 1;
    }
    file_to_obj(&obj, obj_file.data, obj_file.size);
    file_destroy(&obj_file);

    obj_to_mesh(&cube, &obj);
    
    free(obj.vertices);
    free(obj.texture_coords);
    free(obj.normals);
    free(obj.faces);

    size_t entity_capacity = 10; // Start with capacity for 10 entities
    size_t entity_count = 0;     // Initially, we have 0 entities
    struct Entity** entities_to_draw = malloc(entity_capacity * sizeof(struct Entity*)); // Array of pointers
    if (entities_to_draw == NULL) {
        fprintf(stderr, "Failed to allocate memory for entities.\n");
        return 1;
    }

    struct Transform transform;
    transform_init(&transform);
    transform.z = -60.0f;
    transform.y = -10.0f;
    transform.rotation_x = -100.0f;
    transform.scale_x = 0.3f;
    transform.scale_y = 0.3f;
    transform.scale_z = 0.3f;
    
    struct Entity* entity = entity_create(&cube, &transform);

    if (entity != NULL) {
        if (entity_count < entity_capacity) {
             entities_to_draw[entity_count] = entity;
             entity_count++;
        } else {
            fprintf(stderr, "Entity list full!\n");
        }
    } else {
         fprintf(stderr, "Failed to create entity!\n");
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
                current_entity->transform->rotation_z = (float)time * 0.5f;

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
    mesh_destroy(&cube);
    shader_destroy(&shader);
    display_destroy(&display);

    return 0;
}