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
    // unsigned int model_uniform_location = glGetUniformLocation(shader.program, "model");
    // unsigned int view_uniform_location = glGetUniformLocation(shader.program, "view");
    // unsigned int projection_uniform_location = glGetUniformLocation(shader.program, "projection");
    // unsigned int color_uniform_location = glGetUniformLocation(shader.program, "color");
    
    file_destroy(&vertex_shader_file);
    file_destroy(&fragment_shader_file);

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
        
        // draw entities
 
        display_update(&display);
    }

    // Cleanup
    shader_destroy(&shader);
    display_destroy(&display);

    return 0;
}