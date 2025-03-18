#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#include "engine/display.h"
#include "engine/shader.h"
#include "engine/input.h"
#include "engine/file.h"

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
    
    file_destroy(&vertexShaderFile);
    file_destroy(&fragmentShaderFile);
    
    // Example 5: Star (using GL_TRIANGLES)
    float vertices[] = {
        // Center point
         0.0f,  0.0f, 0.0f,
        // Points of the star (5 triangles)
         0.0f,  0.5f, 0.0f,
         0.1f,  0.2f, 0.0f,
         
         0.0f,  0.0f, 0.0f,
         0.1f,  0.2f, 0.0f,
         0.5f,  0.2f, 0.0f,
         
         0.0f,  0.0f, 0.0f,
         0.5f,  0.2f, 0.0f,
         0.2f, -0.1f, 0.0f,
         
         0.0f,  0.0f, 0.0f,
         0.2f, -0.1f, 0.0f,
         0.3f, -0.5f, 0.0f,
         
         0.0f,  0.0f, 0.0f,
         0.3f, -0.5f, 0.0f, 
         0.0f, -0.2f, 0.0f,
         
         0.0f,  0.0f, 0.0f,
         0.0f, -0.2f, 0.0f,
        -0.3f, -0.5f, 0.0f,
         
         0.0f,  0.0f, 0.0f,
        -0.3f, -0.5f, 0.0f,
        -0.2f, -0.1f, 0.0f,
         
         0.0f,  0.0f, 0.0f,
        -0.2f, -0.1f, 0.0f,
        -0.5f,  0.2f, 0.0f,
         
         0.0f,  0.0f, 0.0f,
        -0.5f,  0.2f, 0.0f,
        -0.1f,  0.2f, 0.0f,
         
         0.0f,  0.0f, 0.0f,
        -0.1f,  0.2f, 0.0f,
         0.0f,  0.5f, 0.0f
    };
    // END OF VERTEX DATAS

    shader_bind_vertecies(&shader, vertices, sizeof(vertices), GL_TRIANGLES);

    // render loop
    while(!display_should_close(&display))
    {
        processInput(display.window);
        
        // draw background
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our triangle
        shader_draw(&shader); 
 
        display_update(&display);
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    shader_destroy(&shader);

    display_destroy(&display);
    return 0;
}